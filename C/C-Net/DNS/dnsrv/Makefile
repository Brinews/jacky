CC = gcc
CFLAGS = -g -finput-charset=utf-8 
OBJS = dnshelper.o dnsdb.o

all:	dnshelper.o dnsdb.o
	${CC} ${CFLAGS} -o ndnscli ndnscli.c ${OBJS}
	${CC} ${CFLAGS} -o ndnsrv ndnsrv.c ${OBJS}
	${CC} ${CFLAGS} -o rootdns rootdns.c ${OBJS}

dnshelper.o:	dnshelper.c
	${CC} ${CFLAGS} -c -o dnshelper.o dnshelper.c

dnsdb.o: dnsdb.c
	${CC} ${CFLAGS} -c -o dnsdb.o dnsdb.c

clean:
	rm -f *.o ndnscli ndnsrv rootdns rootcli testdb
