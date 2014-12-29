all: mac_analyzer

clean:
	rm   main.o util.o analyze.o symbtab.o mac_analyzer

main.o: ../main.c
	gcc -c ../main.c

analyze.o: ../analyze.c
	gcc -c ../analyze.c

symbtab.o: ../symbtab.c
	gcc -c ../symbtab.c

util.o: ../util.c
	gcc -c ../util.c
	
mac_analyzer:  parse_util.o parse.o scan.o main.o  util.o symbtab.o analyze.o parse_print.o
	gcc -o mac_analyzer parse_util.o parse.o scan.o main.o util.o  analyze.o symbtab.o parse_print.o

