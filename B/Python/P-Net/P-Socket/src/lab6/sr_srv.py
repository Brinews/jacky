import sys
import random
import socket
from struct import *

from check import ip_checksum

def checksum_valid(chk, seq, header, data):
  checksum = pack('IH'+str(len(data))+'s', seq, header, data)

  if ip_checksum(checksum) == chk:
    return True
  else:
    return False

# send ack
def send_ack(seqNum, clientAddr, sock):
  allZeros = int('0000000000000000', 2)
  header = int('1111111100000000', 2)
  packet = pack('IHH', seqNum, allZeros, header)
  sock.sendto(packet, clientAddr)

def main():
  port = 37802
  host = '127.0.0.1'
  windowSize = 5
  protocol = 'SR'

  print "Host: " + host
  print "Port: " + str(port)
  print "Protocol: " + protocol
  print "Window size: " + str(windowSize)

  srvsock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
  srvsock.bind((host, port))

  seqNum = 0
  firstInWindow = 0
  lastInWindow = firstInWindow + windowSize - 1

  prob = 0.1
  lastReceived = -1

  received = []
  recvBuf = []

  for i in range(windowSize):
    received.append(0)
    recvBuf.append(None)

  while True:
    message, addr = srvsock.recvfrom(1024)
    packet = unpack('I2sH'+str(len(message) - 8)+'s', message)

    if packet[2] == int('1000000000000000', 2):
      break

    if prob < random.random():
      seqNum = packet[0]
      checksum = packet[1]
      header = packet[2]
      data = packet[3]
      print "Packet received for P" + str(seqNum)

      # Protocol = Selective repeat
      if protocol == "SR":
        if seqNum < firstInWindow:
          print "Old packet received: P" + str(seqNum)
          send_ack(seqNum, addr, srvsock)
        else:
          if checksum_valid(checksum, seqNum, header, data):
            if seqNum >= firstInWindow and seqNum <= lastInWindow:
              if seqNum == firstInWindow:
                recvBuf[firstInWindow % windowSize] = None
                received[firstInWindow % windowSize] = 0
                firstInWindow += 1
                lastInWindow += 1
              elif received[seqNum % windowSize] == 0:
                recvBuf[seqNum % windowSize] = packet
                received[seqNum % windowSize] = 1

            print "ACK sent for P" + str(seqNum)
            send_ack(seqNum, addr, srvsock)

          else:
            print "Packet discarded, checksum mismatching."
    else:
      print "Packet P" + str(packet[0]) + " lost."

if __name__ == '__main__':
  main()
