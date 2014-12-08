all: ssdc

FLAGS= -g # -Wall  -pedantic

ssdc: ssdc.o
	gcc *.c -o ssdc $(LIBS) $(FLAGS)

ssdc.o: ssdc.c
	gcc -c $^ $(FLAGS)

LIBS = -L.. -lssl -lm -lcrypto -ldl

clean:
	-rm *.o ssdc
	-rm ./token/*
