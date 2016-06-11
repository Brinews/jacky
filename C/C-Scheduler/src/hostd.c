/*** NO CHANGES TO THIS FILE ARE ALLOWED EXCEPT WHERE INDICATED ***/

/*** START OF SECTION MARKER ***/
/*** PLEASE UPDATE THIS SECTION. NO CODE INCLUDING INCLUDE STATEMENTS MAY BE ADDED. ***/
/*
    HOSTD - Hypothetical Operating System Testbed Dispatcher

    usage:

        hostd <TESTFILE>
        where <TESTFILE> is the name of a job list
*/

/************************************************************************************************************************

    ** Revision history **

    Current version: 1.1 bis
    Date: 7 April 2016

    1.1: Added instructions to allow additional functions in hostd.c and pcb.c
    1.0: Original version

    Contributors:
    1. COMP3520 teaching staff
       Centre for Distributed and High Performance Computing
       School of Information Technologies
       The University of Sydney
       NSW 2006
       Australia

    2. Dr Ian G Graham

    Copyright of this code and associated material is vested in the original contributors.

    This code is NOT in the Public Domain. Unauthorised posting of this code or derivatives thereof is not permitted.

    ** DO NOT REMOVE THIS NOTICE. Just update the revision history whenever you make changes to the code. **

 ***********************************************************************************************************************/
/*** END OF SECTION MARKER ***/

/* Include files */
#include "hostd.h"

/*** START OF SECTION MARKER ***/
/*** ADDITIONAL FUNCTION PROTOTYPES MAY BE ADDED HERE ***/

PcbPtr deq_hrrn_Pcb(PcbPtr *, int timer);

/*** END OF SECTION MARKER ***/

int main (int argc, char *argv[])
{
    /*** START OF SECTION MARKER ***/
    /*** You may add variable declarations in this section. ***/

    FILE * input_list_stream = NULL;
    PcbPtr input_queue = NULL;
    PcbPtr real_time_queue = NULL;
    PcbPtr user_job_queue = NULL;
    PcbPtr prio1_queue = NULL;
    PcbPtr prio2_queue = NULL;
    PcbPtr hrrn_queue = NULL;
    PcbPtr current_process = NULL;
    PcbPtr process = NULL;
    PcbPtr ptr = NULL;
    int timer = 0;
    int priority = 0;

    /* user job mem */
    MabPtr mem = (MabPtr) malloc(sizeof(Mab));
    mem->offset = 0; 
    mem->size = MEMORY_SIZE;
    mem->allocated = 0;
    mem->prev = mem->next = NULL;
	mem->parent = mem->left_child = mem->right_child = NULL;

    /* real-time job mem */
    MabPtr realm = memAlloc(&mem, RT_MEMORY_SIZE);

    MabPtr memPtr;

/*** END OF SECTION MARKER ***/

/*** START OF SECTION MARKER ***/
/*** COMMENTS MAY BE ADDED, MODIFIED OR DELETED IN THIS SECTION ***/

//  1. Populate the FCFS queue

    if (argc == 0)
    {
        fprintf(stderr, "FATAL: Bad arguments array\n");
    }
    else if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <TESTFILE>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (!(input_list_stream = fopen(argv[1], "r")))
    {
        fprintf(stderr, "ERROR: Could not open \"%s\"\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        int val_check;
        process = createnullPcb();
        if ((val_check = fscanf(input_list_stream, "%d, %d, %d, %d",
          &(process->arrival_time), &(process->priority),
          &(process->scheduled_service_time), &(process->mbytes))) == 4)
        {
            process->remaining_cpu_time = process->scheduled_service_time;
            process->status = PCB_INITIALIZED;
            input_queue = enqPcb(input_queue, process);
        }
        else if (val_check >= 0)
        {
            free(process);
            fprintf(stderr, "ERROR: Invalid input file \"%s\"\n", argv[1]);
            exit(EXIT_FAILURE);
        }
        else if (ferror(input_list_stream))
        {
            free(process);
            fprintf(stderr, "ERROR: Could not read input file \"%s\"\n", argv[1]);
            exit(EXIT_FAILURE);
        }
        else {
            free(process);
            break;
        }
    }
/*** END OF SECTION MARKER ***/

/*** START OF SECTION MARKER ***/
/*** CODE AND COMMENTS MAY BE ADDED, DELETED OR MODIFIED IN THIS SECTION ***/

/*** COMMENTS THAT BEGIN WITH // ARE FOR EXERCISE 4 ONLY ***/
/*** YOU NEED TO WRITE YOUR OWN COMMENTS ***/

//  2. Whenever there is a running process or the FCFS queue is not empty:

    while (current_process != NULL || input_queue != NULL 
            || prio1_queue != NULL || prio2_queue != NULL
            || hrrn_queue != NULL || user_job_queue != NULL
            || real_time_queue != NULL) 
    {
//        unload pending processes from the input queue
        while (input_queue != NULL && input_queue->arrival_time <= timer) {

            /* if mem requirement illegal, delete it */
            if (input_queue->priority == 0 
                    && (input_queue->mbytes > RT_MEMORY_SIZE ||
                        input_queue->mbytes < 0)) {
                ptr = deqPcb(&input_queue);
                free(ptr);
            } else if (input_queue->mbytes > USER_MEMORY_SIZE ||
                    input_queue->mbytes < 0) {
                ptr = deqPcb(&input_queue);
                free(ptr);
            } else {
                if (input_queue->priority == 0) {
                    real_time_queue = 
                        enqPcb(real_time_queue, deqPcb(&input_queue));
                } else {
                    user_job_queue = 
                        enqPcb(user_job_queue, deqPcb(&input_queue));
                }
            }
        }

        while (user_job_queue != NULL 
                && (memPtr = memAlloc(&mem, user_job_queue->mbytes)) 
                != NULL) {

            user_job_queue->mem_block = memPtr;

            if (user_job_queue->priority == 1) {
                prio1_queue = enqPcb(prio1_queue, deqPcb(&user_job_queue));
            } else if (user_job_queue->priority == 2) {
                prio2_queue = enqPcb(prio2_queue, deqPcb(&user_job_queue));
            } else if (user_job_queue->priority == 3) {
                hrrn_queue = enqPcb(hrrn_queue, deqPcb(&user_job_queue));
            }
        }

//      i. If there is a currently running process;
        if (current_process != NULL) 
        {
//          a. Decrement the process's remaining_cpu_time variable;
            current_process->remaining_cpu_time--;
            priority = current_process->priority;
            
//          b. If the process's allocated time has expired:
            if (current_process->remaining_cpu_time <= 0) 
            {
//              A. Terminate the process;
                terminatePcb(current_process);
                
//              B. Deallocate the PCB (process control block)'s memory
                if (current_process->priority != 0)
                    memFree(&mem, current_process->mem_block);

                free(current_process);
                current_process = NULL;
            } else if (priority == 1) {
                current_process->priority++;
                if (prio1_queue != NULL || prio2_queue != NULL) {
                    suspendPcb(current_process);
                    //kill(current_process->pid, SIGTSTP);
                }
                prio2_queue = enqPcb(prio2_queue, current_process);
                current_process = NULL;
            } else if (priority == 2) {
                current_process->priority++;
                if (prio1_queue != NULL || prio2_queue != NULL
                        || hrrn_queue != NULL) {
                    suspendPcb(current_process);
                    //kill(current_process, SIGTSTP);
                }
                hrrn_queue = enqPcb(hrrn_queue, current_process);
                current_process = NULL;
            } else if (priority == 3) {
                suspendPcb(current_process);
                //kill(current_process, SIGTSTP);
                hrrn_queue = enqPcb(hrrn_queue, current_process);
                current_process = NULL;
            }
        }

//      ii. If there is no running process and there is a process ready to run:
        if (current_process == NULL 
             && (hrrn_queue != NULL
                || prio1_queue != NULL
                || prio2_queue != NULL
                || real_time_queue != NULL)) 
        {
//          Dequeue the process at the head of the queue, set it as currently running and start it
            if (real_time_queue != NULL) {
                current_process = deqPcb(&real_time_queue);
                current_process->mem_block = realm;
            } else if (prio1_queue != NULL || prio2_queue != NULL) {
                if (prio1_queue != NULL) {
                    current_process = deqPcb(&prio1_queue);
                } else {
                    current_process = deqPcb(&prio2_queue);
                }
            } else {
                current_process = deq_hrrn_Pcb(&hrrn_queue, timer);
            }

            if (current_process != NULL) {
                startPcb(current_process);
            }
//
        }
        
//      iii. Let the dispatcher sleep for one second;
        //printf("%d\n", timer); // REPLACE THIS LINE WITH YOUR CODE (ONE LINE ONLY)
        sleep(1);
        
//      iv. Increment the dispatcher's timer;
        timer++;
        
//      v. Go back to 4.
    }

	memFree(&mem, realm);
    if (mem != NULL) free(mem);
    
//  3. Terminate the HOST dispatcher
    exit(EXIT_SUCCESS);
}

/*** END OF SECTION MARKER ***/

/*** START OF SECTION MARKER ***/
/*** ADDITIONAL FUNCTION IMPLEMENTATIONS MAY BE ADDED HERE ***/
/*** END OF SECTION MARKER ***/
/*** END OF CODE MARKER; DO NOT ADD MATERIAL BEYOND THIS POINT ***/
