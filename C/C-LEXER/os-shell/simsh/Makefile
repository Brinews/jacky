#
# makefile for simsh
# Date: 2014-03-10
# Create: for make
#

CFLAGS = -g

OBJS=simsh.o splitline.o execute.o process.o builtin.o

simsh: simsh.o splitline.o execute.o process.o builtin.o 
	cc -o simsh simsh.o splitline.o execute.o process.o builtin.o -g


clean:
	rm *.o simsh
