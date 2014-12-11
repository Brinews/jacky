#ifndef SCORING_H_
#define SCORING_H_

int targetOccursInNumDocs(pageIndex *i, char *target);
int targetOccursNumTimes(page *pageToCheck, char *target);

double computerTFIDF(double t, double i);
double computeIDF(int numIndexedDocs, int numContainedTarget);
double computeTF(int tOccurs, int wordsInDoc);
void qsIndexedPages(pageIndex* indexedPages,int low,int high);

#endif
