CC=gcc
CFLAGS=-Wall -pedantic -std=c99 -g
all: hostd
.PHONEY: clean

hostd: hostd.o pcb.o mab.o
hostd.o : hostd.c
mab.o : mab.c
pcb.o : pcb.c

clean:
	rm *.o hostd
