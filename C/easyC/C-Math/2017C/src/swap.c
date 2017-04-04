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


void display(int ticker, int pid, int pnum, int hnum, int usage)
{
    printf("time %d, %d loaded, numprocesses=%d, numholes=%d, memusage=%d%%\n",
            ticker, pid, pnum, hnum, usage);
}

void Swap(struct queue *loadqueue, struct queue *rr, struct pool *pol,
        int ticker, int quant, struct pcb *expired, FitOpt opt)
{
    struct pcb temp;

    struct pcb *pc = queue_front(loadqueue);

    int addr = pool_alloc(pol, pc->memsize, opt);
    if (addr != -1) {
        // swap in new process
        temp = dequeue(loadqueue);
        temp.timeslice = quant;
        temp.memaddr = addr;
        temp.starttime =  ticker; // update loaded time
        enqueue(rr, &temp);
        if (expired != NULL) {
            enqueue(rr, expired);
        }

        //printf("$$\n");
        //print_queue(rr);

        display(ticker, temp.pid, queue_size(rr), hole_count(pol), mem_usage(pol));
    } else {
        // Swap(): swap out and swap in

        //printf("^^\n");
        //print_queue(rr);

        if (expired != NULL) {
            enqueue(rr, expired);
        }

        do {
            temp = queue_swapout(rr, ticker);
            //printf("--");
            //print_pcb(&temp);
            pool_free(pol, temp.memaddr);

            //pool_print_available(pol);
            //pool_print_active(pol);

            temp.starttime = ticker;
            queue_insert(loadqueue, temp);
        } while ((addr = pool_alloc(pol, pc->memsize, opt)) == -1);


        // swap in
        temp = dequeue(loadqueue);

        temp.timeslice = quant;
        temp.memaddr = addr;
        temp.starttime =  ticker; // update loaded time

        enqueue(rr, &temp);

        //pool_print_available(pol);
        //pool_print_active(pol);

        display(ticker, temp.pid, queue_size(rr), hole_count(pol), mem_usage(pol));

        //printf("++\n");
        //print_queue(loadqueue);
        //exit(0);
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
            Swap(loadqueue, rr, pol, ticker, quant, NULL, opt);
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

        //printf("&&& %d: ", ticker);
        //print_pcb(cur);

        if (cur->timeslice <= 0) {
            // E2: time slice expired
            if (!isempty(loadqueue)) {
                // swap in
                temp = dequeue(rr);
                temp.timeslice = quant;
                Swap(loadqueue, rr, pol, ticker, quant, &temp, opt);
                //enqueue(rr, &temp);
            } else {
                // Schedule():
                if (cur->jobtime > 0) {
                    temp = dequeue(rr);
                    temp.timeslice = quant;
                    enqueue(rr, &temp);
                }
            }
        }

        if (cur->jobtime <= 0) {
            // E3: job exit

            // free space
            pool_free(pol, cur->memaddr);
            dequeue(rr);
            
            if (!isempty(loadqueue)) {
                // Swap():
                Swap(loadqueue, rr, pol, ticker, quant, NULL, opt);
            } else {
                // Schedule()
                if (!isempty(rr)) {
                    cur = queue_front(rr);
                    cur->timeslice = quant;
                }
            }
        }
    }

    printf("time %d, simulation finished.\n", ticker);

    // free all memory
    queue_destroy(rr);
    queue_destroy(loadqueue);
    pool_destroy(pol);

	
    return 0;
}
