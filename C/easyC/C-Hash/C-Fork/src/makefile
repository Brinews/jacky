CC=gcc
CFLAGS=-Wall -Werror

all: pcbsim

pcbsim: main.o pcb.o mlist.o
	$(CC) -o $@ $^

clean:
	rm *.o pcbsim
