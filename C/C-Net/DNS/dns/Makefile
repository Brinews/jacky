CC = gcc
CFLAGS =
OBJS = dnshelper.o

all:	dnshelper.o
	${CC} ${CFLAGS} -o ndnscli ndnscli.c ${OBJS}
	${CC} ${CFLAGS} -o ndnsrv ndnsrv.c ${OBJS}

dnshelper.o:	dnshelper.c
	${CC} ${CFLAGS} -c -o dnshelper.o dnshelper.c

clean:
	rm -f *.o ndnscli ndnsrv
