SRC = $(wildcard *.c)
EXE = $(patsubst %.c,%,$(SRC))
FLAGS = -g -ansi -Wall -pedantic

all: $(EXE)

$(EXE):%:%.c
	echo $(SRC)
	echo $(EXE)
	gcc -o $@ $^
