
all: server_a server_b

server_% : server_%.o rsa.o
	g++ -Wall -o $@ $^ -lm

rsa.o : rsa.h rsa.cpp

server_%.o : server_%.cpp

clean:
	-rm server_a server_b *.o
