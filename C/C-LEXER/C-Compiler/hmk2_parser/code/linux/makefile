all: lin_parser

clean:
	rm   main.o util.o parse_util.o parse.o parse_print.o lin_parser

main.o: ../main.c
	gcc -c ../main.c

parse.o: ../parse.c
	gcc -c ../parse.c

parse_util.o: ../parse_util.c
	gcc -c ../parse_util.c

util.o: ../util.c
	gcc -c ../util.c

parse_print.o: ../parse_print.c
	gcc -c ../parse_print.c
	
lin_parser: main.o util.o parse_util.o parse.o scan.o parse_print.o
	gcc -o lin_parser main.o util.o parse_util.o parse_print.o parse.o scan.o

