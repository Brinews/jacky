// KIT107 Assignment 2
/*
 * Implementation for driver
 * Author Julian Dermoudy and <<YOUR NAME>>
 * Version 2015-09-01
 */

#include <stdio.h>
#include <stdlib.h>
//#include <stdbool.h>
#include <string.h>
#include "sample.h"
#include "node.h"


node individuals;	// 'table' (implemented as linked list of linked lists) with individuals as column headings and rooms as rows


/*
 * addIndividual()
 * Precondition: s is defined and not NULL
 * Postcondition: the individual's details held in s are added to the linked list maintaining alphabetical order
 * Informally: Add an individual to the table
 */
void addIndividual(sample s)
{
	node itr = individuals; /* node iterator */

	node onenode; /* used for new */
	init_node(&onenode, s);

	if (itr == NULL) 
	{
		/* first people */
		itr = onenode;
		individuals = onenode;
	}
	else 
	{
		node pitr = itr; /* pitr: previous node, itr: next node */
		int findLocation = 0;
		/* find location and insert */
		while (itr != NULL && !findLocation) 
		{
			int r = strcmp(getName(getData(itr)), getName(s));

			if (r < 0)
			{
				pitr = itr;
				itr = getNextIndividual(itr);
			}
			else
			{
				findLocation = 1;
			}
		}

		/* add to last */
		if (!findLocation) 
		{
			setNextIndividual(pitr, onenode);
		}
		else 
		{
			if (pitr == itr) /* add to first */
			{
				setNextIndividual(onenode, itr);
				individuals = onenode; /* update first node ptr */
			}
			else  /* add to middle location */
			{
				setNextIndividual(pitr, onenode);
				setNextIndividual(onenode, itr);
			}
		}
	}
}


/*
 * readInPeople()
 * Precondition: the required file is available
 * Postcondition: all individuals contained in the file have been added to the top row of the table (linked list)
 * Informally: Add all individuals to the table
 */
void readInPeople()
{
	const char *FILENAME="identified.txt";

	FILE *fp;
	char *sequence, *name;
	sample onesample;

	fp=fopen(FILENAME,"r");
	if (fp==NULL)
	{
		fprintf(stderr,"Cannot open file.\n");
		exit(1);
	}
	else
	{
		sequence = (char *)malloc(100 * sizeof(char));
		name = (char *)malloc(100 * sizeof(char));
		while (fscanf(fp,"%[^,],%[^\n]\n",sequence,name) != EOF)
		{
			init_sample(&onesample,name,sequence);
			addIndividual(onesample);

			sequence = (char *)malloc(100 * sizeof(char));
			name = (char *)malloc(100 * sizeof(char));
		}
	}
	fclose(fp);
}


/*
 * match()
 * Precondition: both s1 and s2 are defined and not NULL
 * Postcondition: the percentage of commonality in s1 and s2 is returned
 * Informally: determine how closely two samples match
 */
int match(char *s1, char *s2)
{
	int i; /* iterator */
	int sameones = 0; /* both same code in s1 and s2 */
	int percent; /* same percentage */
	int minLen, maxLen; /* length of s1, s2 */

	minLen = strlen(s1);
	maxLen = strlen(s2);

	if (minLen > maxLen) 
	{
		int temp = minLen;
		minLen = maxLen;
		maxLen = temp;
	}

	for (i = 0; i < minLen; i++) 
	{
		if (s1[i] == s2[i])
		{
			sameones ++;
		}
	}

	percent = (int)(100.0*sameones/maxLen);

	return percent;
}


/*
 * processMatches()
 * Precondition: s is defined and not NULL
 * Postcondition: if the given sample is at least a 40% match for anyone and it is the highest match for that person in the
 *                specified room, then add it to the table (as a new entry if there is none for that person in that room, or
 *                as an updated entry if there is already an entry for that person in that room)
 * Informally: if this is the best match for an individual (and is 'significant') record it in the table
 */
void processMatches(sample s)
{
	node ptr; /* node iterator */
	ptr = individuals;

	while (ptr != NULL)
	{
		int m = match(getSequence(getData(ptr)), getSequence(s));

		setMatch(s, m);

		if (m > 40)
		{
			/* find same location node */
			node locItr = getNextLocation(ptr);
			int findLoc = 0;

			while (locItr != NULL && !findLoc) 
			{
				if (strcmp(getName(getData(locItr)), getName(s)) == 0) 
				{
					findLoc = 1;
				}
				else 
				{
					locItr = getNextLocation(locItr);
				}
			}

			if (!findLoc)
			{
				/* add to people node's location */
				node onenode;
				init_node(&onenode, s);

				if (getNextLocation(ptr) == NULL)
				{
					/* first location node */
					setNextLocation(ptr, onenode);
				}
				else 
				{
					/* add to tail node */
					setNextLocation(onenode, getNextLocation(ptr));
					setNextLocation(ptr, onenode);
				}
			}
			else 
			{
				if (getMatch(getData(locItr)) < m)
				{
					/* update location */
					setData(locItr, s);
				}
			}
		}

		ptr = getNextIndividual(ptr);
	}
}


/*
 * readInSamples()
 * Precondition: the required file is available
 * Postcondition: all samples contained in the file which are identifiable (significant) have been compared to those in the 
 *                table with the best of these included
 * Informally: Obtain, identify, and record all samples
 */
void readInSamples()
{
	const char *FILENAME = "unidentified.txt";

	char *sequence, *location;
	sample onesample;
	FILE *fp;

	fp = fopen(FILENAME, "r");
	if (fp == NULL)
	{
		fprintf(stderr,"Cannot open file.\n");
		exit(1);
	}
	else
	{
		sequence = (char *)malloc(100 * sizeof(char));
		location = (char *)malloc(100 * sizeof(char));
		while (fscanf(fp, "%[^,],%[^\n]\n", sequence, location) != EOF)
		{
			init_sample(&onesample, location, sequence);
			processMatches(onesample);

			sequence = (char *)malloc(100 * sizeof(char));
			location = (char *)malloc(100 * sizeof(char));
		}
	}
	fclose(fp);
}


/*
 * showGraph()
 * Precondition: none
 * Postcondition: after asking the user which room to report on, a graph (one "*" for every 5%) is displayed of the 
 *                greatest match of each individual with the located samples for that room
 * Informally: Report on the highest matches for each person in the chosen room
 */
void showGraph(char *room)
{
	int m, s; /* for output */
	node ptr; /* people node iterator */
	int findFlag = 0;
	node locPtr; /* location iterator */

	ptr = individuals;

	printf("Location report for %s\n", room);
	printf("======================================\n");

	while (ptr != NULL)
	{
		locPtr = getNextLocation(ptr);

		findFlag = 0;

		while (locPtr != NULL && !findFlag)
		{
			if (strcmp(getName(getData(locPtr)), room) == 0)
			{
				findFlag = 1;
				printf("%17s | ", getName(getData(ptr)));

				m = getMatch(getData(locPtr));
				s = 0;

				while (s <= m)
				{
					printf("*");
					s = s + 5;
				}

				printf(" %d\n", m);
			}
			else 
			{
				locPtr = getNextLocation(locPtr);
			}
		}

		ptr = getNextIndividual(ptr);
	}

	system("pause");
}


/*
* main()
* Precondition: none
* Postcondition: all identified samples have been read in, all unidentified samples have been compared, and a report
*                has been given on the room chosen by the user
* Informally: entry-point
*/
int main(int argc, char *argv[])
{
	char buf[1024];

	individuals = NULL;
	readInPeople();
	readInSamples();

	while (1) {
		printf("Which location would you like to report on? ");
		fgets(buf, sizeof(buf), stdin);
		buf[strlen(buf)-1] = '\0';

		if (strlen(buf) == 0) return 0;

		showGraph(buf);
	}

	return 0;
}
