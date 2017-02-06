CC=gcc
CFLAGS= -g 
LDFLAGS= -lpthread  -lmysqlclient

all: client epoll

epoll: epoll.o threadpool.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm client epoll *.o
