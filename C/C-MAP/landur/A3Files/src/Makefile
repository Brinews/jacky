lander: lander.o helper.o
	gcc -Wall -std=c99 -o lander lander.o helper.o -lm -l curses
lander.o: lander.c lander.h
	gcc -Wall -std=c99 -c lander.c
helper.o: helper.c lander.h
	gcc -Wall -std=c99 -c helper.c
clean:
	-rm -f *.o lander core
tar:
	tar -cvf submit.tar lander.c lander.h helper.c Makefile

run:
	./lander -g 1 -f landscape.txt -t -3 -i
