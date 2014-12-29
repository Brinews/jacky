all: win_gcc_analyzer

clean:
	rm  win_gcc_analyzer_.exe  main.o util.o symbtab.o analyze.o

win_gcc_analyzer: main.o  util.o symbtab.o analyze.o  scan.o util.o parse.o parse_util.o parse_print.o
	gcc -o win_gcc_analyzer.exe main.o scan.o util.o  parse.o symbtab.o analyze.o parse_util.o parse_print.o -g

main.o: ../../main.c
	gcc -c ../../main.c

util.o: ../../util.c
	gcc -c ../../util.c

symbtab.o: ../../symbtab.c
	gcc -c ../../symbtab.c
	
analyze.o: ../../analyze.c
	gcc -c ../../analyze.c -g
