// KIT107 Assignment 2
/*
 * Implementation for Sample ADT
 * Author Julian Dermoudy and <<YOUR NAME>>
 * Version 2015-09-01
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sample.h"

/*
 * Definition of sample_int type
 */
struct sample_int {
	char *name;			// individual name or location name
	char *sequence;
	int match;			// not used for individuals but used for unidentified samples
};


/*
 * init_sample()
 * Precondition: s, name, and samp are defined
 * Postcondition: s points to a new sample with copies of name and samp in the name and sequence fields (respectively) and 0 in the match field
 * Informally: 'Constructor' for sample
 */
void init_sample(sample *s, char *name, char *samp)
{
	*s = (sample) malloc(sizeof(struct sample_int));

	(*s)->name = name;
	(*s)->sequence = samp;
	(*s)->match = 0;
}


/*
 * getName()
 * Precondition: s is defined and not NULL
 * Postcondition: the name field's value is returned
 * Informally: Return name field
 */
char *getName(sample s)
{
	return s->name;
}


/*
 * getSequence()
 * Precondition: s is defined and not NULL
 * Postcondition: the sequence field's value is returned
 * Informally: Return sequence field
 */
char *getSequence(sample s)
{
	return s->sequence;
}


/*
 * getMatch()
 * Precondition: s is defined and not NULL
 * Postcondition: the match field's value is returned
 * Informally: Return match field
 */
int getMatch(sample s)
{
	return s->match;
}


/*
 * setName()
 * Precondition: s and n are defined and not NULL
 * Postcondition: s's name field contains copy of n
 * Informally: Set the name field
 */
void setName(sample s, char *n)
{
	s->name = (char *)malloc((strlen(n) + 1)*sizeof(char));
	strcpy(s->name, n);
}


/*
 * setSequence()
 * Precondition: s and n are defined and not NULL
 * Postcondition: s's sequence field contains copy of n
 * Informally: Set the sequence field
 */
void setSequence(sample s, char *n)
{
	s->sequence = (char *)malloc((strlen(n) + 1)*sizeof(char));
	strcpy(s->sequence, n);
}


/*
 * setMatch()
 * Precondition: s is defined and not NULL
 * Postcondition: s's match field contains value of n
 * Informally: Set the match field
 */
void setMatch(sample s, int m)
{
	s->match = m;
}


/*
 * toString()
 * Precondition: s is defined and not NULL
 * Postcondition: a printable version of s is returned
 * Informally: Convert a sample to a string
 */
char *toString(sample s)
{
	char *buf = (char *) malloc((strlen(s->name) + strlen(s->sequence) 
			+ 2 + 64)*sizeof(char));

	sprintf(buf, "<name:%s,sequece:%s,match:%d>", s->name,
			s->sequence, s->match);

	return buf;
}


/*
 * clone()
 * Precondition: s is defined and not NULL
 * Postcondition: a copy of s has been returned
 * Informally: Clone a sample
 */
sample clone(sample s)
{
	sample c;

	c = (sample)malloc(sizeof(struct sample_int));

	c->name = (char *)malloc((strlen(s->name)+1)*sizeof(char));
	strcpy(c->name, s->name);

	c->sequence = (char *)malloc((strlen(s->sequence)+1)*sizeof(char));
	strcpy(c->sequence, s->sequence);

	c->match = s->match;

	return c;
}

