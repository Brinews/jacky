CC=gcc
CFLAGS=-Wall -ansi -pedantic -g -O2
SUBDIR = client server

MAKE = make

all:
	@for dir in $(SUBDIR); \
		do $(MAKE) -C $$dir || exit 1; \
	done

clean:
	@for dir in $(SUBDIR); \
		do $(MAKE) -C $$dir clean || exit 1; \
	done

.PHONEY: all clean

