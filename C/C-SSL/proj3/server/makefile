all: ssdr

FLAGS= -g #-Wall  -pedantic

ssdr: ssdr.o helper.o 
	gcc  *.c -o ssdr $(LIBS) $(FLAGS)
	
ssdr.o: ssdr.c helper.c 
	gcc -c $^ $(FLAGS)

#encryption.o: encryption.c
#	gcc -c $^ $(FLAGS)

LIBS = -L.. -lssl -lm -lcrypto -ldl -lpthread

clean:
	-rm *.o ssdr
	-rm repo/* token/*
