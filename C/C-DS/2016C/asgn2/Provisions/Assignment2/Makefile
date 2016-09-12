all: main

main: node.o my_time.o firing_solution.o assig_two216.o
	gcc -o $@ $^

%.o:%.c
	gcc -c $^
