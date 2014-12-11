#ifndef WEBSEARCH_H_
#define WEBSEARCH_H_

#include "crawler.h"
#include "index.h"
#include "scoring.h"

#include<stdio.h>
#include<time.h>

char *getQuery();
void search(pageIndex *pages, char *query);

#endif

