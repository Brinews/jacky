#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mempool.h"
#include "processqueue.h"

/* Extern declarations: */

const int MAXQSIZE = 100;

extern  int     optind;
extern  char    *optarg;

typedef enum { FIRST, BEST, WORST } FitOpt;

void display(int ticker, int pid, int pnum, int hnum, int usage)
{
    printf("time %d, %d loaded, numprocesses=%d, numholes=%d, memusage=%d%%\n",
            ticker, pid, pnum, hnum, usage);
}

// adder = 1 means need re-push a process into rr queue later
void Swap(struct queue *loadqueue, struct queue *rr, struct pool *pol,
        int ticker, int quant, int adder)
{
    struct pcb temp;

    struct pcb *pc = queue_front(loadqueue);

    char *addr = pool_alloc(pol, pc->memsize);
    if (addr != NULL) {
        // swap in new process
        temp = dequeue(loadqueue);
        temp.timeslice = quant;
        temp.memaddr = addr;
        temp.starttime =  ticker; // update loaded time
        enqueue(rr, &temp);
        display(ticker, temp.pid, queue_size(rr)+adder, hole_count(pol), mem_usage(pol));
    } else {
        // Swap(): swap out and swap in

        do {
            temp = queue_swapout(rr, ticker);
            //printf("--");
            //print_pcb(&temp);
            pool_free(pol, temp.memaddr);

            //pool_print_available(pol);
            //pool_print_active(pol);

            temp.starttime = ticker;
            queue_insert(loadqueue, temp);
        } while ((addr = pool_alloc(pol, pc->memsize)) == NULL);


        // swap in
        temp = dequeue(loadqueue);

        temp.timeslice = quant;
        temp.memaddr = addr;
        temp.starttime =  ticker; // update loaded time

        enqueue(rr, &temp);

        //pool_print_available(pol);
        //pool_print_active(pol);

        display(ticker, temp.pid, queue_size(rr)+adder, hole_count(pol), mem_usage(pol));

        //print_queue(loadqueue);
        //exit(0);
    }
}

void Schedule()
{
}

// get_let() reads in a lowercase letter from input and returns
//   it as an index from 0...25
// effects: if unable to read in a letter, prints message and exits
int get_let(void) {
    char c = '\0';
    int result = scanf(" %c", &c);
  if (result != 1 || c < 'a' || c > 'z') {
    printf("exit: invalid letter\n");
    exit(1);
  }
  return c - 'a';
}

// get_int() reads in an int from input
// effects: if unable to read in a number, prints message and exits
int get_int(void) {
  int i;
  if (scanf("%d", &i) != 1) {
    printf("exit: invalid number\n");
    exit(1);
  }
  return i;
}

int test_memory(void) {
  struct pool *p = NULL;
  char *letters[26] = {0};
  while (1) {
    char cmd;
    if (scanf(" %c", &cmd) != 1) break;
    if (cmd == 'q') break;
    if (cmd == 'c') {
      int size = get_int();
      if (p) {
        printf("create: fail (already created)\n");
      } else {
        p = pool_create(size);
      }
    } else if (cmd == 'd') {
      bool result = pool_destroy(p);
      if (result) {
        p = NULL;
      } else {
        printf("destroy: fail\n");
      }
    } else if (cmd == 'a') {
      int let = get_let();      
      int size = get_int();
      if (letters[let]) {
        printf("malloc %c %d: fail (already allocated)\n", 'a' + let, size);
      } else {
        char *ptr = pool_alloc(p, size);
        if (ptr) {
          letters[let] = ptr;
        } else {
          printf("malloc %c %d: fail\n", 'a' + let, size);
        }
      }
    } else if (cmd == 'f') {
      int let = get_let(); 
      if (pool_free(p, letters[let])) {
        letters[let] = NULL;
      } else {
        printf("free %c: fail\n", 'a' + let);
      }
    } else if (cmd == 'r') {
      int let = get_let();      
      int size = get_int();
      char *ptr = pool_realloc(p, letters[let], size);
      if (ptr) {
        letters[let] = ptr;
      } else {
        printf("realloc %c %d: fail\n", 'a' + let, size);
      } 
    } else if (cmd == 'm') {
      pool_print_active(p);
    } else if (cmd == 'n') {
      pool_print_available(p);
    } else {
      printf("Invalid command (%c).\n",cmd);
      break;
    }
  }
  if (p) {
    if (pool_destroy(p)) {
      printf("ERROR: did not destroy pool\n");
    } else {
      printf("ERROR: did not free all allocations\n");
    }
  }
}


int main(int argc, char **argv)
{
	char input;
	char *inputfile = "input.txt";	// default input file
    FitOpt opt = FIRST;
	int size = 1000;			// default size
    int quant = 7;
	
	while ((input = getopt(argc, argv, "f:a:m:q:")) != EOF)
	{
		switch ( input )
		{
			case 'a':
				if(strcmp(optarg, "first") == 0) 
					opt = FIRST;
				else if(strcmp(optarg, "best") == 0) 
					opt  = BEST;
				else if(strcmp(optarg, "worst") == 0) 
                    opt = WORST;
 				break;
            case 'f':
                inputfile = optarg;
                break;
			case 'm':
				// set the value of size (int value) based on optarg
				size = atoi(optarg);
				break;
            case 'q':
                quant = atoi(optarg);
                break;
			default:
				// do something
				break;
		} 
	}

    FILE *fp = fopen(inputfile, "r");
    if (fp == NULL) {
        printf("%s open failed\n", inputfile);
        return 0;
    }

    struct queue *loadqueue = queue_create(MAXQSIZE);

    int starttime, pid, memsize, jobtime;

    // load process into queue
    while (fscanf(fp, "%d%d%d%d", &starttime, &pid, 
                &memsize, &jobtime) > 0) {
        struct pcb *p = create_pcb(pid, starttime, memsize, jobtime);
        enqueue(loadqueue, p);
        destroy_pcb(p);
    }

    fclose(fp);

    // prepare memory pool
    struct pool *pol = pool_create(size);
    struct queue *rr = queue_create(MAXQSIZE);

    sort_queue(loadqueue);
    //print_queue(loadqueue);

    // round-robin
    int ticker = 0;
    struct pcb temp;
    struct pcb *cur = NULL;

    while (!isempty(loadqueue) || !isempty(rr)) {
        if (pool_empty(pol)) {
            //E1: first load
            Swap(loadqueue, rr, pol, ticker, quant, 0);
        }

        cur = queue_front(rr);
        /*
        printf("***\n");
        print_queue(rr);
        printf("***\n");
        */

        // update time
        cur->timeslice--;
        cur->jobtime--;
        ticker++;

        //printf("%d: ", ticker);
        //print_pcb(cur);

        if (cur->timeslice <= 0) {
            // E2: time slice expired
            if (!isempty(loadqueue)) {
                // swap in
                temp = dequeue(rr);
                temp.timeslice = quant;
                Swap(loadqueue, rr, pol, ticker, quant, 1);
                enqueue(rr, &temp);
            } else {
                // Schedule():
                temp = dequeue(rr);
                temp.timeslice = quant;
                enqueue(rr, &temp);
            }
        }

        if (cur->jobtime <= 0) {
            // E3: job exit

            // free space
            pool_free(pol, cur->memaddr);
            dequeue(rr);
            
            if (!isempty(loadqueue)) {
                // Swap():
                Swap(loadqueue, rr, pol, ticker, quant, 0);
            } else {
                // Schedule()
                cur = queue_front(rr);
                cur->timeslice = quant;
            }
        }
    }

    printf("time %d, simulation finished.\n");

    // free all memory
    queue_destroy(rr);
    queue_destroy(loadqueue);
    pool_destroy(pol);

	
    return 0;
}
