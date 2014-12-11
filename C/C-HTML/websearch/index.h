#ifndef INDEX_H
#define INDEX_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CHILD_NUM 		26
#define MAX_WORDSIZE	128
#define MAX_BUFFERSIZE	2<<16
#define ALPHA_LENGTH	26

typedef struct tnode
{
	int frequency;
	char value;
	struct tnode *links[CHILD_NUM];
} tnode;

typedef struct page
{
	int totalTerms;
	char url[1000];
	int pageScore;

	tnode *trie;
} page;

typedef struct pageIndex
{
	int numPages;
	page **pages; /* point to page list */
} pageIndex;

pageIndex* indexPagesFromFile(const char *fn, int mp);
page* buildPage(char *url);
tnode *indexPage(const char *url, int *numWords);
void destroyPageIndex(pageIndex *pages);
void freePage(page *p);
int destroyTree(tnode *root);

tnode* initializeNewNode(char v);
int addWordOccurrence(const char *w, const int wl, tnode* n);

int getText(const char* srcAddr, char* buffer, const int bufSize);

#endif
