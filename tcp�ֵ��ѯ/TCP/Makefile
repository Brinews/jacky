all: client server
client: client.o
	gcc -g -o client client.o
server: inetserver.o
	gcc -g -o server server.o
inetclient.o: inetclient.c
	gcc -Wall -g -c client.c
inetserver.o: inetserver.c
	gcc -Wall -g -c server.c
