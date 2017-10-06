CC=g++ 
all: proj2
CXXFLAGS=-g

proj2: project2.o lexer.o inputbuf.o
	g++ -g -o $@ $^

clean:
	rm *.o *.exe
