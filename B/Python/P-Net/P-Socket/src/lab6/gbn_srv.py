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

def send_ack(seqno, cliaddr, sock):
  dumpy = int('0000000000000000', 2)
  header = int('1010101010101010', 2)
  packet = pack('IHH', seqno, dumpy, header)
  sock.sendto(packet, cliaddr)

def main():
  port = 37801
  host = '127.0.0.1'
  windowSize = 0

  print "Host: " + host
  print "Port: " + str(port)
  print "Protocol: GBN"

  srvsock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
  srvsock.bind((host, port))

  seqno = 0
  firstInWindow = 0
  lastInWindow = firstInWindow + windowSize - 1

  prob = 0.1
  curseq = -1

  while True:
    message, addr = srvsock.recvfrom(1024)
    packet = unpack('I2sH'+str(len(message) - 8)+'s', message)

    # end tag
    if packet[2] == int('1000000000000000', 2):
      break

    if prob < random.random():
      seqno = packet[0]
      checksum = packet[1]
      header = packet[2]
      data = packet[3]
      print("Packet received for P" + str(seqno))

      # Protocol = Go back N
      if checksum_valid(checksum, seqno, header, data):
        if seqno == curseq + 1:
          print "ACK sent for P" + str(seqno)
          send_ack(seqno, addr, srvsock)
          curseq = seqno
        elif seqno > curseq + 1:
          print "(Out of order): last received packet seq: " \
                + str(curseq)
        else:
          print "ACK sent for P" + str(seqno)
          send_ack(seqno, addr, srvsock)
          curseq = seqno

      else:
        print "Packet corrupted, checksum mismatch."

    else:
      print "Packet P" + str(packet[0]) + " lost."

if __name__ == '__main__':
  main()
