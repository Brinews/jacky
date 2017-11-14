'''
    Simple RDT3.0
'''


import socket
import sys
from check import ip_checksum
import random
import time

# delay 70-130 msg
def rand_sleep():
    delay = 0.07 + random.random() * 0.05
    time.sleep(delay)

def sendwithcheck(sock, msg, to):
    checksum = ip_checksum(msg)
    tosend = checksum+msg

    rand = random.randint(1, 20)
    if rand >= 1 and rand <= 5:
        print("Dropped")
        return

    rand_sleep()
    sock.sendto(tosend, to)

HOST = ''    # Symbolic name meaning all available interfaces
PORT = 8888    # Arbitrary non-privileged port

# Datagram (udp) socket
try :
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    print('Socket created')
except socket.error as msg :
    print('Failed to create socket. Error Code : ' + str(msg[0]) + ' Message ' + msg[1])
    sys.exit()


# Bind socket to local host and port
try:
    s.bind((HOST, PORT))
except socket.error as msg:
    print('Bind failed. Error Code : ' + str(msg[0]) + ' Message ' + msg[1])
    sys.exit()
    
print('Socket bind complete')

expecting_seq = 0

#now keep talking with the client
while 1:
    # receive data from client (data, addr)
    msg, addr = s.recvfrom(1024)

    #msg = msg.decode('utf-8')

    checksum = msg[:2]
    seq = msg[2]
    cont = msg[3:]
    
    if not msg: 
        break

    if ip_checksum(cont) == checksum:
        sendwithcheck(s, "ACK" + seq, addr)
        if seq == str(expecting_seq):
            print('Message[' + addr[0] + ':' + str(addr[1]) + '] - ' + cont)
            expecting_seq = 1 - expecting_seq
            #reply = 'OK...' + data
            #s.sendto(reply.encode('utf-8') , addr)
    else:
        neg_seq = str(1 - expecting_seq)
        sendwithcheck(s, "ACK" + neg_seq, addr)
    
s.close()
