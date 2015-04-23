CC = g++
CFLAGS = -g

all : rct_main

rct_main : rct_user.o rct_project.o rct_time.o rct_task.o rct_main.o
	g++ -o $@ $^ -g

%.o : %.cpp
	g++ -c -g $^

clean:
	rm *.exe *.o
