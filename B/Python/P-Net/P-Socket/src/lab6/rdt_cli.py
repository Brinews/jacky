'''
  simple rdt 3.0 client
  date: 2017-11-11
'''

import socket    #for sockets
import sys    #for exit
from check import ip_checksum
import random

def corrupt(pkt):
    idx = random.randint(0, len(pkt)-1)
    # maybe the same as before
    pkt = pkt[:idx] + str(unichr(random.randint(48, 95))) + pkt[idx+1:]
    return pkt


# create dgram udp socket
try:
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
except socket.error:
    print('Failed to create socket')
    sys.exit()

host = 'localhost';
port = 8888;

s.settimeout(1) # set time out

while(1) :
    
    seq = 0

    try :
        while True:
            msg = raw_input('Enter message to send(Q for quit): ')

            if msg == 'Q' or msg == 'q':
                sys.exit()

            ack_received = False
            while not ack_received:

                #Set the whole string
                tosend = ip_checksum(msg) + str(seq) + msg

                rand = random.randint(1, 20)
                if rand >= 1 and rand <= 5:
                    print("Corrupted")
                    tosend = corrupt(tosend)

                s.sendto(tosend, (host, port))
                
                # receive data from client (data, addr)
                try:
                    reply, addr = s.recvfrom(1024)
                except socket.timeout:
                    print('Timeout')
                else:

                    checksum = reply[:2]
                    ack_seq = reply[5]

                    print reply[2:5]

                    if ip_checksum(reply[2:]) == checksum and ack_seq == str(seq):
                        ack_received = True

            seq = 1 - seq
                    
                
    except socket.error as msg:
        print('Error Code : ' + str(msg[0]) + ' Message ' + msg[1])
        sys.exit()
