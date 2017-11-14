import sys
import time
import random
import signal
import threading
import socket
from struct import *

from check import ip_checksum

# to send bytes
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

def message_to_send():
  global sendOver
  global MTU

  message = ''
  while len(message) < MTU and not sendOver:
    message += next_byte()
  return message

host = '127.0.0.1'
winSize = 8
TIMEOUT = 10 # 10*0.01s
MTU = 30

port = 37801 
pktTotal = 10

CORRUPTED = 0.1 # packet corrupt
ACKLOST = 0.05  # ACK lost

msg = "THEFOXJUMPOVERTHEDOGEASILY"
content = msg * pktTotal 

print "host: " + host
print "Port: " + str(port)
print "protocol: GBN"
print "Window size: " + str(winSize)
print "Timeout: " + str(TIMEOUT)
print "MTU: " + str(MTU)
print "Packets to send: " + str((len(content) / MTU) + 1)

seqno = 0
winHead = -1
winTail = -1
lastAcked = -1

sendOver = False
ackOver = False

winPkts = []
tickers = []

clisock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
mylock = threading.Lock()

## Resend packets in the window
def resend_win_packets():
  global winPkts
  global clisock
  global TIMEOUT
  global tickers
  global winTail
  global winHead
  global host
  global port
  global winSize

  iterator = winHead
  while iterator <= winTail:
    if winPkts[iterator % winSize] != None:
      packet = winPkts[iterator % winSize]
      print "Re-sending packet: P" + str(iterator)
      clisock.sendto(packet, (host, port))
      tickers[iterator % winSize] = TIMEOUT
    iterator += 1

# end packet
def last_packet():
  header = int('1000000000000000', 2)
  checksum = "aa"
  return pack('I2sH', seqno, checksum, header)

# timer for resend
def timer_for_ack(signum, _):
  global winHead
  global winTail
  global winPkts
  global mylock
  global tickers
  global winSize
  global ackOver

  if ackOver: return

  for i, eachtimer in enumerate(tickers):
    tickers[i] = eachtimer - 1

  if len(tickers) > (winHead % winSize) and tickers[winHead % winSize] == 0:
    print "Timeout, seq =", winHead
    mylock.acquire()
    resend_win_packets()
    mylock.release()

# read ack
def ack_recv_thread():
  global winHead
  global winPkts
  global winSize
  global clisock
  global seqno
  global ackOver
  global sendOver
  global lastAcked
  global winTail

  # Go back N
  while not ackOver:
    packet, addr = clisock.recvfrom(8)

    ack = unpack('IHH', packet)
    ackNum = ack[0]

    if ACKLOST < random.random():

      if ackNum == lastAcked + 1:
        print "Recv ACK: ", ackNum
        mylock.acquire()
        winPkts[ackNum % winSize] = None
        tickers[ackNum % winSize] = 0
        lastAcked = lastAcked + 1
        winHead = winHead + 1
        mylock.release()

      if sendOver and lastAcked >= winTail:
        ackOver = True
    else:
      print "ACK " + str(ackNum) + " lost."


ackThread = threading.Thread(target=ack_recv_thread, args=())
ackThread.start()

signal.signal(signal.SIGALRM, timer_for_ack)
signal.setitimer(signal.ITIMER_REAL, 0.01, 0.01)

winHead = 0

# Send packets
while not sendOver:
  winCur = winTail + 1

  # wait for ack recv
  if winCur >= winSize and winHead != winCur and winCur%winSize == winHead%winSize:
    continue

  data = message_to_send()

  header = int('1111111000000000', 2)
  cs = pack('IH' + str(len(data)) + 's', seqno, header, data)
  checksum = ip_checksum(cs)

  packet = pack('I2sH' + str(len(data)) + 's', seqno, checksum, header, data)

  if winCur < winSize:
    winPkts.append(packet)
    tickers.append(TIMEOUT)
  else:
    winPkts[winCur % winSize] = packet
    tickers[winCur % winSize] = TIMEOUT

  print "Send P" + str(seqno)

  if CORRUPTED > random.random():
    error_data = "31234567890120456782345678012345678"
    packet = pack('I2sH' + str(len(error_data)) + 's', seqno, checksum, header, error_data)

  clisock.sendto(packet, (host, port))

  winTail = winTail + 1
  seqno = seqno + 1

while not ackOver:
  pass

clisock.sendto(last_packet(), (host, port))
clisock.close()
