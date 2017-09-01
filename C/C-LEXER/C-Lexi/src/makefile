all: lexi

lexi: lexer.o inputbuf.o
	g++ -o $@ $^

clean:
	rm *.o *.exe
