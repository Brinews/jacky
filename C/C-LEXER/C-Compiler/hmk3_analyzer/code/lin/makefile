all: lin_analyzer

clean:
	rm   main.o util.o analyze.o symbtab.o lin_analyzer

main.o: ../main.c
	gcc -c ../main.c

analyze.o: ../analyze.c
	gcc -c ../analyze.c

symbtab.o: ../symbtab.c
	gcc -c ../symbtab.c

util.o: ../util.c
	gcc -c ../util.c
	
lin_analyzer:  parse_util.o parse.o scan.o main.o  util.o symbtab.o analyze.o parse_print.o
	gcc -o lin_analyzer parse_util.o parse.o scan.o main.o util.o  analyze.o symbtab.o parse_print.o

