cc=gcc
flags= -Wall -Werror -g -O0 -std=gnu11
ldflags = -lz

.PHONY : all test check clean

all: client

client:client.c
	$(cc) $(flags) -o $@ $^ $(ldflags)

test: client
	./client ./test dir.txt

check: client
	valgrind --leak-check=full --show-leak-kinds=all ./client ./test test.txt

clean:
	rm client dir.txt test.txt
