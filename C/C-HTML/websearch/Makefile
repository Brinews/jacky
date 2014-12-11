CC=gcc
CFLAGS= -g
all: webSearch

webSearch: crawler.o index.o scoring.o webSearch.o
	$(CC) $(CFLAGS) $^ -o $@

%.o : %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o *~ webSearch 
    
