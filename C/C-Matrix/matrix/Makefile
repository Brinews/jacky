CC = clang
#CC = gcc
CFLAGS = -O1 -std=gnu11 -Wall -Werror
#CFLAGS = -O1 -std=c99 -Wall 
LDFLAGS = -pthread
#LDFLAGS = -lpthread

.PHONY: all clean

all: matrix

matrix: main.c matrix.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	-rm -f *.o
	-rm -f matrix
