CC=g++
all: main

%.o:%.cpp
	g++ -c $^

main: node.o parser.o main.o printer.o util.o
