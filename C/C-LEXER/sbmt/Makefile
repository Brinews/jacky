HEADER=$(wildcard *.h)
SRC = $(wildcard *.c)

FLAGS = -g -Wall -ansi -pedantic
all: main
main: $(SRC) $(HEADER)
	gcc -o $@ $^
clean:
	-rm *~
