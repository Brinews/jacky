all: main

main: *.c
	gcc -std=c99 -D_GNU_SOURCE -o $@ $^ -lpthread

%.o:%.c

clean:
	rm main
