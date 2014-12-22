all: mac_parser

clean:
	rm   main.o util.o parse_util.o parse_print.o parse.o mac_parser

main.o: ../main.c
	gcc -c ../main.c

parse.o: ../parse.c
	gcc -c ../parse.c

parse_util.o: ../parse_util.c
	gcc -c ../parse_util.c

parse_print.o: ../parse_print.c
	gcc -c ../parse_print.c

util.o: ../util.c
	gcc -c ../util.c
	
mac_parser: main.o util.o parse_util.o parse_print.o parse.o scan.o
	gcc -o mac_parser main.o util.o parse_util.o parse_print.o parse.o scan.o

