all: rtn

rtn: Server.o Client.o select.o
	gcc -o $@ $^
clean:
	rm *.o rtn
