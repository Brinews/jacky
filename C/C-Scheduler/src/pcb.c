/*** NO CHANGES TO THIS FILE ARE ALLOWED EXCEPT WHERE INDICATED ***/
/* PCB management functions for HOSTD */

/* Include Files */
#include "pcb.h"

/*** CODE COMMENTS MAY BE ADDED BEYOND THIS POINT ***/

/*** START OF SECTION MARKER ***/
/*** ADDITIONAL FUNCTION PROTOTYPES MAY BE ADDED HERE ***/
/*** END OF SECTION MARKER ***/

/*******************************************************
 * PcbPtr startPcb(PcbPtr process) - start (or restart)
 *    a process
 * returns:
 *    PcbPtr of process
 *    NULL if start (restart) failed
 ******************************************************/
PcbPtr startPcb (PcbPtr p)
{
    if (p->pid == 0)
    {
        switch (p->pid = fork())
        {
            case -1:
                fprintf(stderr, "FATAL: Could not fork process!\n");
                exit(EXIT_FAILURE);
            case 0:
                p->pid = getpid();
                p->status = PCB_RUNNING;
                printPcbHdr();
                printPcb(p);
                fflush(stdout);
                execv(p->args[0], p->args);
                fprintf(stderr, "Could not run process!\n");
                exit(EXIT_FAILURE);
        }
    }
    else
    {
        kill(p->pid, SIGCONT);
    }
    p->status = PCB_RUNNING;
    return p;
}

/*******************************************************
 * PcbPtr suspendPcb(PcbPtr process) - suspend
 *    a process
 * returns:
 *    PcbPtr of process
 *    NULL if suspension failed
 ******************************************************/
PcbPtr suspendPcb(PcbPtr p)
{
    if (!p)
    {
        fprintf(stderr, "ERROR: Can not suspend a NULL process\n");
        return NULL;
    }
    else
    {
        kill(p->pid, SIGTSTP);
        waitpid(p->pid, NULL, WUNTRACED);
        p->status = PCB_SUSPENDED;
        return p;
    }
}

/*******************************************************
 * PcbPtr terminatePcb(PcbPtr process) - terminate
 *    a process
 * returns:
 *    PcbPtr of process
 *    NULL if termination failed
 ******************************************************/
PcbPtr terminatePcb(PcbPtr p)
{
    if (!p)
    {
        fprintf(stderr, "ERROR: Can not terminate a NULL process\n");
        return NULL;
    }
    else
    {
        kill(p->pid, SIGINT);
        waitpid(p->pid, NULL, WUNTRACED);
        p->status = PCB_TERMINATED;
        return p;
    }
}

/*******************************************************
 * PcbPtr printPcb(PcbPtr process)
 *  - print process attributes on stdout
 *  returns:
 *    PcbPtr of process
 ******************************************************/
PcbPtr printPcb(PcbPtr p)
{
    if (!p)
    {
        fprintf(stderr, "ERROR: Could not print process control block parameters\n");
        return NULL;
    }
    else
    {
        printf("%5d %12d %10d %16d %22d %20d\n", 
                p->pid, p->arrival_time, p->priority,
                p->scheduled_service_time, p->mbytes, 
                p->mem_block->offset);
        return p;
    }
}

/*******************************************************
 * void printPcbHdr() - print header for printPcb
 *  returns:
 *    void
 ******************************************************/
void printPcbHdr()
{
    /*** WRITE YOUR CODE HERE ***/
    printf ("<PID> <Arrival Time> <Priority> <Scheduled Running Time> <Allocated Memory (MB)> <Memory Offset (MB)>\n");
}

/*******************************************************
 * PcbPtr createnullPcb() - create inactive Pcb.
 *
 * returns:
 *    PcbPtr of newly initialised Pcb
 *    NULL if malloc failed
 ******************************************************/
PcbPtr createnullPcb()
{
    PcbPtr new_process_Ptr;
    if (!(new_process_Ptr = (PcbPtr)malloc(sizeof(Pcb))))
    {
        fprintf(stderr, "ERROR: Could not create new process control block\n");
        return NULL;
    }
    new_process_Ptr->pid = 0;
    new_process_Ptr->args[0] = "./process";
    new_process_Ptr->args[1] = NULL;
    new_process_Ptr->arrival_time = 0;
    new_process_Ptr->priority = 0;
    new_process_Ptr->remaining_cpu_time = 0;
    new_process_Ptr->mbytes = 0;
    new_process_Ptr->mem_block = NULL;
    new_process_Ptr->status = PCB_UNINITIALIZED;
    new_process_Ptr->next = NULL;
    
    return new_process_Ptr;
}

/*******************************************************
 * PcbPtr enqPcb (PcbPtr headofQ, PcbPtr process)
 *    - queue process (or join queues) at end of queue
 *
 * returns head of queue
 ******************************************************/
PcbPtr enqPcb(PcbPtr q, PcbPtr p)
{
    if (q == NULL) {
        q = p;
        q->next = NULL;
    } else {
        PcbPtr ptr = q;
        while (ptr->next != NULL) ptr = ptr->next; /* move to tail node */

        ptr->next = p;
        ptr->next->next = NULL;
    }

    return q; /*** REPLACE THIS LINE WITH YOUR CODE ***/
}

/*******************************************************
 * PcbPtr deqPcb (PcbPtr * headofQ);
 *    - dequeue process - take Pcb from head of queue.
 *
 * returns:
 *    PcbPtr if dequeued,
 *    NULL if queue was empty
 *    & sets new head of Q pointer in adrs at 1st arg
 *******************************************************/
PcbPtr deqPcb(PcbPtr * hPtr)
{
    PcbPtr ret = *hPtr;

    if (ret != NULL)
        *hPtr = (*hPtr)->next;

    return ret;
}

/*** START OF SECTION MARKER ***/
/*** ADDITIONAL FUNCTION IMPLEMENTATIONS MAY BE ADDED HERE ***/

/*******************************************************
 * PcbPtr deq_hrrn_Pcb (PcbPtr * headofQ);
 *    - dequeue process - take Pcb from head of HRRN queue.
 *
 * returns:
 *    PcbPtr if dequeued,
 *    NULL if queue was empty
 *    & sets new head of Q pointer in adrs at 1st arg
 *******************************************************/
PcbPtr deq_hrrn_Pcb(PcbPtr * hPtr, int timer)
{
    PcbPtr ptr = *hPtr;

    if (ptr == NULL) return NULL;

    /* select max response ratio */
    PcbPtr maxResRat = NULL, prevMaxPtr = NULL;
    PcbPtr prevPtr = NULL;
    double maxRat = -1000000.0;

    /* find max ratio */
    while (ptr != NULL) {
        double rat = 1.0 + (timer-ptr->arrival_time-
                ptr->scheduled_service_time+ptr->remaining_cpu_time)
                /ptr->remaining_cpu_time;
        if (rat > maxRat) {
            maxRat = rat;
            maxResRat = ptr;
            prevMaxPtr = prevPtr;
        }
        prevPtr = ptr;
        ptr = ptr->next;
    }

    /* dequeue maxResRat from queue */

    if (maxResRat == *hPtr) { // first node
        *hPtr = (*hPtr)->next;
    } else { // middle node
        prevMaxPtr->next = maxResRat->next;
    }

    return maxResRat;
}


/*** END OF SECTION MARKER ***/
/*** END OF CODE; DO NOT ADD MATERIAL BEYOND THIS POINT ***/
