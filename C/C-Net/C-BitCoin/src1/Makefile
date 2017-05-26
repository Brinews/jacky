CC = gcc -std=gnu99

all: server client

server: server.c sha256.h sha256.c uint256.h
	$(CC) -g -o $@ $^ -lpthread

client: client.c
	$(CC) -g -o $@ $^

clean:
	rm -rf  *.o server client

.PHONY = clean server
