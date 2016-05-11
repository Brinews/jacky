all: tsize main cleanTest test

main: main.c
	gcc -g -o $@ $^
test: test.c
	gcc -g -o $@ $^

clean:
	rm tsize.exe main.exe cleanTest.exe test.exe
