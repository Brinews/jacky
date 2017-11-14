import sys
import time
import random
import signal
import threading
import socket
from struct import *

from check import ip_checksum

host = '127.0.0.1'
port = 37802

protocol = 'SR'
winSize = 8
TIMEOUT = 10
MTU = 30

pktNum = 10

CORRUPT = 0.1 
LOST = 0.05 

msg = "THEFOXJUMPOVERTHEDOGEASILY" 
content = msg * pktNum 

print "Host: " + host
print "protocol: " + protocol
print "Window size: " + str(winSize)
print "Timeout: " + str(TIMEOUT)
print "MTU: " + str(MTU)
print "Port: " + str(port)
print "Packets to send: " + str((len(content) / MTU) + 1)

seqno = 0
winhead = -1
wintail = -1
lastAcked = -1
numAcked = -1

sendOver = False
ackOver = False

winPkts = []
tickers = []

clisock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
lock = threading.Lock()

def next_byte():
  global sendOver
  global content

  if content:
    nextByte = content[0]
    content = content[1:len(content)]
  else:
    nextByte = ''
    sendOver = True
  return nextByte

def msg_to_send():
  global sendOver
  global MTU

  msg = ''
  while len(msg) < MTU and not sendOver:
    msg += next_byte()
  return msg

## Resend packets in the window
def ResendPackets():
  global winPkts
  global clisock
  global TIMEOUT
  global tickers
  global wintail
  global winhead
  global host
  global port
  global winSize

  pit = winhead
  while pit <= wintail:
    if winPkts[pit % winSize] != None:
      packet = winPkts[pit % winSize]
      print "Resending packet: P" + str(pit) + "; Timer started"
      clisock.sendto(packet, (host, port))
      tickers[pit % winSize] = TIMEOUT
    pit += 1

def last_packet():
  header = int('1000000000000000', 2)
  checksum = 'aa'
  return pack('I2sH', seqno, checksum, header)

def timer_for_ack(signum, _):
  global winhead
  global wintail
  global winPkts
  global lock
  global tickers
  global winSize

  if ackOver:
    return

  if protocol == "SR":
    pit = winhead
    while pit <= wintail:
      tickers[pit % winSize] = tickers[pit % winSize] - 1
      lock.acquire()
      if tickers[pit % winSize] < 1 and winPkts[pit % winSize] != None:
        print "Timeout, sequence number =", pit
        packet = winPkts[pit % winSize]
        print "Resending packet: P" + str(pit) + "; Timer started"
        clisock.sendto(packet, (host, port))
        tickers[pit % winSize] = TIMEOUT
      lock.release()
      pit = pit + 1

## Look for acknowledgements from the server
def ack_recv_thread():
  global winhead
  global winPkts
  global winSize
  global clisock
  global numAcked
  global seqno
  global ackOver
  global sendOver
  global lastAcked
  global wintail

  # Protocol = Selective repeat
  if protocol == "SR":
    while not ackOver:
      packet, addr = clisock.recvfrom(8)

      ack = unpack('IHH', packet)
      ackNum = ack[0]

      if LOST < random.random():
        print "Received ACK: ", ackNum
        if ackNum == winhead:
          lock.acquire()
          winPkts[winhead % winSize] = None
          tickers[winhead % winSize] = 0
          lock.release()
          numAcked = numAcked + 1
          winhead = winhead + 1
        elif ackNum >= winhead and ackNum <= wintail:
          winPkts[ackNum % winSize] = None
          tickers[ackNum % winSize] = 0
          numAcked += 1

        if sendOver:
          ackOver = True
          for pi in range(winhead, wintail+1):
            if winPkts[pi%winSize] != None:
              ackOver = False
      else:
        print "Ack " + str(ackNum) + " lost (Info for simulation)."

threadack = threading.Thread(target=ack_recv_thread, args=())
threadack.start()

signal.signal(signal.SIGALRM, timer_for_ack)
signal.setitimer(signal.ITIMER_REAL, 0.01, 0.01)

winhead = 0

while not sendOver:
  wincur = wintail + 1

  if wincur >= winSize and winhead != wincur and wincur%winSize == winhead%winSize:
    continue

  data = msg_to_send()

  header = int('1111111100000000', 2)
  cs = pack('IH' + str(len(data)) + 's', seqno, header, data)
  checksum = ip_checksum(cs)

  packet = pack('I2sH' + str(len(data)) + 's', seqno, checksum, header, data)

  if wincur < winSize:
    winPkts.append(packet)
    tickers.append(TIMEOUT)
  else:
    winPkts[wincur % winSize] = packet
    tickers[wincur % winSize] = TIMEOUT

  print "Sending P" + str(seqno) + ", timer started"

  if CORRUPT > random.random():
    error_data = "01234567012345678012345678012345678"
    packet = pack('I2sH' + str(len(error_data)) + 's', seqno, checksum, header, error_data)

  clisock.sendto(packet, (host, port))

  wintail = wintail + 1
  seqno = seqno + 1

while not ackOver:
  pass

clisock.sendto(last_packet(), (host, port))
clisock.close()
