all : spim

CC=gcc

spim: spimcore.o component.o
	$(CC) -o $@ $^ -g

%.o : %.c
	$(CC) -c $^ -g

.PHONY: clean

clean:
	-rm *.o spim.exe
