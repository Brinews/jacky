// KIT107 Assignment 2
/*
 * Specification for the Sample ADT
 * Author Julian Dermoudy
 * Version 20/8/15
 */

#ifndef SAMPLE_H
#define SAMPLE_H

struct sample_int;
typedef struct sample_int *sample;

void init_sample(sample *s, char *name, char *sequence);
char *getName(sample s);
char *getSequence(sample s);
int getMatch(sample s);
void setName(sample s, char *name);
void setSequence(sample s, char *name);
void setMatch(sample s, int m);
sample clone(sample s);
char *toString(sample s);

#endif
