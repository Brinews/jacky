all: win_gcc_parser

clean:
	rm  win_gcc_parser.exe  main.o util.o parse.o parse_util.o parse_print.o

win_gcc_parser: main.o scan.o util.o parse.o parse_util.o parse_print.o
	gcc -o win_gcc_parser main.o scan.o util.o  parse.o parse_util.o parse_print.o

main.o: ../../main.c
	gcc -c ../../main.c

util.o: ../../util.c
	gcc -c ../../util.c

parse.o: ../../parse.c
	gcc -c ../../parse.c
	
parse_util.o: ../../parse_util.c
	gcc -c ../../parse_util.c

parse_print.o: ../../parse_print.c
	gcc -c ../../parse_print.c