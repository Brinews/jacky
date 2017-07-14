#include "pcb.h"
#include "mlist.h"

#include <stdio.h>

int comp(void *p, void *q)
{
    PCB *p1 = (PCB *) p;
    PCB *q1 = (PCB *) q;

    if (p1 != NULL && q1 == NULL) return 1;
    if (p1 == NULL && q1 != NULL) return -1;

    if (p1->pid == q1->pid) return 0;

    if (p1->pid < q1->pid) return -1;
    else return 1;
}

int compMsg(void *p, void *q)
{
    MSG *p1 = (MSG *)p;
    int *pid = (int *)q;

    if (p1 != NULL && p1->to_pid == *pid) return 0;

    return 1;
}

void DumpPcbList(LIST *queue)
{
	PCB *p = ListFirst(queue);
	if (p != NULL) {
		printf("pid=%d,", p->pid);
		while ((p = ListNext(queue)) != NULL) {
			printf("pid=%d,", p->pid);
		}
		printf("\n");
	}

	ListFirst(queue); // return to the head of queue
}

int SearchPcbList(LIST *queue, int pid)
{
	PCB *p = ListFirst(queue);
	if (p != NULL) {
		if (p->pid == pid) {
			printf("pid=%d, priority=%d\n", pid, p->priority);
			return 0;
		} else {
			while ((p = ListNext(queue)) != NULL) {
				if (p->pid == pid) {
					printf("pid=%d, priority=%d\n", pid, p->priority);
					return 0;
				}
			}
		}
	}

	return -1;
}

int main()
{
    char line[1024]; // command line
    int prio; // priority
    int pid; // process id

    LIST *pcbQ0 = ListCreate(); // process with priority = 0
    LIST *pcbQ1 = ListCreate(); // process with priority = 1
    LIST *pcbQ2 = ListCreate(); // process with priority = 2

    LIST *msgQ = ListCreate(); // message queue
    LIST *sndBlkQ = ListCreate(); // block queue of sender
    LIST *rcvBlkQ = ListCreate(); // block queue of receiver

    PCB *current = NULL;
    LIST *curPcbQ = pcbQ0;

    PCB *IdlePcb = create_pcb(0); 
    int flag = 1;

	PCB *p, *copy, *first;
	PCB tokill, torecv;
	LIST *from;

    MSG *m;
    char *ptr;

	int sid, val;
    SEM *s;
    int i;
    LIST *list;

	current = IdlePcb;

    while (flag) {
        fgets(line, 1024, stdin);

        if (line[0] == 'C') {
            sscanf(line+1, "%d", &prio);

            p = create_pcb(prio);
            printf("PID:%d created\n", p->pid);

            // add a new process
            if (prio == 0) {
                ListAppend(pcbQ0, p);
            } else if (prio == 1) {
                ListAppend(pcbQ1, p);
            } else if (prio == 2) {
                ListAppend(pcbQ2, p);
            }

        } else if (line[0] == 'F') {
            copy = fork_pcb(current);
            printf("PID:%d forked\n", copy->pid);

            // add a forked process
            prio = copy->priority;
            if (prio == 0) {
                ListAppend(pcbQ0, copy);
            } else if (prio == 1) {
                ListAppend(pcbQ1, copy);
            } else if (prio == 2) {
                ListAppend(pcbQ2, copy);
            }
        } else if (line[0] == 'K') {
            sscanf(line+1, "%d", &pid);
            tokill.pid = pid;

            // search the process to kill
            from = pcbQ0;
			ListFirst(pcbQ0);
            p = ListSearch(pcbQ0, comp, (void *)&tokill);

            if (p == NULL) {
                from = pcbQ1;
				ListFirst(pcbQ1);
                p = ListSearch(pcbQ1, comp, (void *)&tokill);
            }
            if (p == NULL) {
                from = pcbQ2;
				ListFirst(pcbQ2);
                p = ListSearch(pcbQ2, comp, (void *)&tokill);
            }

            if (p != NULL) { // find the to killed process
                printf("PID:%d killed\n", pid);
                ListRemove(from);
            } else {
				printf("No process killed\n");
			}

            if (comp(p, current) == 0) { // killed the running process
                if (comp(current, IdlePcb) == 0) {
                    flag = 0; // killed the init process
                } else {
                    //current = schedule();
                    destroy_pcb(p);
                    if (ListFirst(pcbQ0) != NULL) {
                        current = ListFirst(pcbQ0);
                        curPcbQ = pcbQ0;
                    } else if (ListFirst(pcbQ1) != NULL) {
                        current = ListFirst(pcbQ1);
                        curPcbQ = pcbQ1;
                    } else if (ListFirst(pcbQ2) != NULL) {
                        current = ListFirst(pcbQ2);
                        curPcbQ = pcbQ2;
                    } else {
                        current = IdlePcb;
                        curPcbQ = NULL;
                    }
                }
            }
        } else if (line[0] == 'E' || line[0] == 'Q') { // end the running process

            if (curPcbQ != NULL) {
                first = ListFirst(curPcbQ);
            } else {
                first = NULL;
            }

            if (first != NULL && comp(current, first) == 0) { // need re-schedule

				printf("PID:%d exit\n", current->pid);

                ListRemove(curPcbQ);
                destroy_pcb(first);

                //current = schedule();
                if (ListFirst(pcbQ0) != NULL) {
                    current = ListFirst(pcbQ0);
                    curPcbQ = pcbQ0;
                } else if (ListFirst(pcbQ1) != NULL) {
                    current = ListFirst(pcbQ1);
                    curPcbQ = pcbQ1;
                } else if (ListFirst(pcbQ2) != NULL) {
                    current = ListFirst(pcbQ2);
                    curPcbQ = pcbQ2;
                } else {
                    current = IdlePcb;
                    curPcbQ = NULL;
                }
            } else if (comp(current, IdlePcb) == 0) {
                flag = 0; // killed the init process
				printf("init process exit\n");
            } else {
				//printf("error\n");
			}
        } else if (line[0] == 'S') {
            sscanf(line+1, "%d", &pid);
            ptr = line+2;
            while (*ptr != ' ' && *ptr != '\n') ptr++;
            ptr++;

            if (comp(current, IdlePcb) != 0) { // non-idle process
                // enqueue sender into block queue
                first = ListFirst(curPcbQ);
                if (comp(first, current) == 0) {
					m = create_msg(current->pid, pid, ptr);
                    ListRemove(curPcbQ);
                    ListAppend(sndBlkQ, current);
					printf("Msg send from %d to %d\n", current->pid, pid);
                    current = IdlePcb; // need re-schedule
					ListAppend(msgQ, m);
                }

                // unblock the receiver if exist
                torecv.pid = pid;
                ListFirst(rcvBlkQ);

                p = ListSearch(rcvBlkQ, comp, (void *)&torecv);
                if (p != NULL) {
                    ListRemove(rcvBlkQ);
                    p->hasMsg = 1; // need print recv message
                    if (p->priority == 0) {
                        ListAppend(pcbQ0, p);
                    } else if (p->priority == 1) {
                        ListAppend(pcbQ1, p);
                    } else if (p->priority == 2) {
                        ListAppend(pcbQ2, p);
                    }
					printf("Recv pid %d unblocked\n", pid);
                }
            }
        } else if (line[0] == 'R') {
            if (comp(current, IdlePcb) != 0) {
                // check message queue
                ListFirst(msgQ);
                m = ListSearch(msgQ, compMsg, (void *)&(current->pid));
                if (m != NULL) {
                    // consume message
                    printf("Recv from %d msg:%s", m->from_pid, m->mesg);
                    ListRemove(msgQ); // can send reply-msg
                    destroy_msg(m);
                } else {
                    // block the receiver process
                    ListAppend(rcvBlkQ, current);

                    ListFirst(curPcbQ);
                    ListRemove(curPcbQ);
                }
            }
        } else if (line[0] == 'Y') {
            if (comp(current, IdlePcb) != 0) {
                sscanf(line+1, "%d", &pid);
                ptr = line+2;
                while (*ptr != ' ' && *ptr != '\n') ptr++;
                ptr++;
                // check the sender block queue
				torecv.pid = pid;
				ListFirst(sndBlkQ);
				p = ListSearch(sndBlkQ, comp, (void *)&torecv);
				if (p != NULL) {
					p->hasMsg = 1;
					// unblock the sender

					p = ListRemove(sndBlkQ);

					if (p->priority == 0) {
						ListAppend(pcbQ0, p);
					} else if (p->priority == 1) {
						ListAppend(pcbQ1, p);
					} else if (p->priority == 2) {
						ListAppend(pcbQ2, p);
					}

					// enqueue the reply message
					m = create_msg(current->pid, pid, ptr);
					ListAppend(msgQ, m);

					printf("Msg reply from %d to %d\n", current->pid, pid);

				} else {
					printf("reply error: not found the sender\n");
				}
            }
        } else if (line[0] == 'Z') {
			printf("current pid=%d\n", current->pid);
		} else if (line[0] == 'T') {
			printf("Priority 0 process queue:\n");
			DumpPcbList(pcbQ0);
			printf("Priority 1 process queue:\n");
			DumpPcbList(pcbQ1);
			printf("Priority 2 process queue:\n");
			DumpPcbList(pcbQ2);
			printf("sender block process queue:\n");
			DumpPcbList(sndBlkQ);
			printf("recver block process queue:\n");
			DumpPcbList(rcvBlkQ);
            for (i = 0; i < 5; i++) {
                list = get_semQ(i);
                if (list != NULL) {
                    printf("%d semaphore block process queue:\n", i);
                    DumpPcbList(list);
                }
            }
		} else if (line[0] == 'I') {
			sscanf(line+1, "%d", &pid);
			if (SearchPcbList(pcbQ0, pid) == -1) {
				if (SearchPcbList(pcbQ1, pid) == -1) {
					if (SearchPcbList(pcbQ2, pid) == -1) {
						if (SearchPcbList(sndBlkQ, pid) == -1) {
							if (SearchPcbList(rcvBlkQ, pid) == -1) {
                                for (i = 0; i < 5; i++) {
                                    list = get_semQ(i);
                                    if (list != NULL 
                                            && SearchPcbList(list, pid) != -1)
                                        break;
                                }
                                if (i == 5) {
                                    printf("No found pid=%d process\n", pid);
                                }
							}
						}
					}
				}
			}
		} else if (line[0] == 'N') {
			sscanf(line+1, "%d%d", &sid, &val);
            if (sid > 4 || sid < 0) {
                printf("Invalid semaphore ID\n");
            } else {
                s = get_sem(sid);
                s->val = val;
            }
		} else if (line[0] == 'P') {
			sscanf(line+1, "%d", &sid);
            if (sid > 4 || sid < 0) {
                printf("Invalid semaphore ID\n");
            } else {
                s = get_sem(sid);
                s->val--;
                if (s->val < 0) {
                    // block self process
                    ListFirst(curPcbQ);
                    ListRemove(curPcbQ);

                    ListAppend(s->semQ, current);

                    printf("PID: %d blocked for semaphore\n", 
                            current->pid);

                    current = IdlePcb; // re-schedule
                }
            }
		} else if (line[0] == 'V') {
			sscanf(line+1, "%d", &sid);
            if (sid > 4 || sid < 0) {
                printf("Invalid semaphore ID\n");
            } else {
                s = get_sem(sid);
                s->val++;
                if (ListFirst(s->semQ) != NULL) {
                    // unblock one waiting process
                    p = ListRemove(s->semQ);

                    printf("PID: %d unblocked for semaphore\n", p->pid);

                    if (p->priority == 0) {
                        ListAppend(pcbQ0, p);
                    } else if (p->priority == 1) {
                        ListAppend(pcbQ1, p);
                    } else if (p->priority == 2) {
                        ListAppend(pcbQ2, p);
                    }
                }
            }
		}

		if (comp(current, IdlePcb) == 0) {
			// re-schedule a ready process
			if (ListFirst(pcbQ0) != NULL) {
				current = ListFirst(pcbQ0);
				curPcbQ = pcbQ0;
			} else if (ListFirst(pcbQ1) != NULL) {
				current = ListFirst(pcbQ1);
				curPcbQ = pcbQ1;
			} else if (ListFirst(pcbQ2) != NULL) {
				current = ListFirst(pcbQ2);
				curPcbQ = pcbQ2;
			} 
		} else {
			if (current->hasMsg == 1) {
				// has message to receive
				ListFirst(msgQ);
				m = ListSearch(msgQ, compMsg, (void *)&(current->pid));
				if (m != NULL) {
					printf("Recv from %d msg:%s", m->from_pid, m->mesg);
					ListRemove(msgQ);
					destroy_msg(m);
				}
			}
		}
    }

    destroy_pcb(IdlePcb);

	// free all message left
	while (ListFirst(msgQ)) {
		m = ListRemove(msgQ);
		printf("debug: msg=%d,%d,%s", m->from_pid, m->to_pid, m->mesg);
		destroy_msg(m);
	}

	// free all blocked pcbs
	while (ListFirst(sndBlkQ)) {
		p = ListRemove(sndBlkQ);
		printf("debug: pid=%d\n", p->pid);
		destroy_pcb(p);
	}

	while (ListFirst(rcvBlkQ)) {
		p = ListRemove(rcvBlkQ);
		printf("debug: pid=%d\n", p->pid);
		destroy_pcb(p);
	}

    free_sem();

    return 0;
}
