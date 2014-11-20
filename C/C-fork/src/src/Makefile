header = common.h
objs = people.o elevator.o board.o

TARGET = people elevator board

all: $(TARGET)

$(objs):$(header)
%.o : %.c
	gcc -c $^

$(TARGET):%:%.o
	gcc -o $@ $^ -g -Wall -ansi -lpthread

clean:
	-rm *.o $(TARGET) *.gch
