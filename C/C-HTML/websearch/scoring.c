#include "index.h"
#include "scoring.h"
#include <math.h>
#include <string.h>

int targetOccursNumTimes(page* pageToCheck, char* target)
{
	// Variable declaration
	int targetLen, index;
	tnode* targetTrie;
	tnode* cursor;
	char buffer[MAX_WORDSIZE];

	// Variable Initialization
	targetLen = strlen(target);
	targetTrie = pageToCheck->trie;
	cursor = targetTrie;
	index = 0;

	for(index = 0;index<MAX_WORDSIZE;index++){
		buffer[index] = '\0';
	}

	index=0;
	// While loop to figure out the path to the target
	while(cursor->links[target[index]-'a'] != NULL && index<targetLen){
		buffer[index] = target[index];
		cursor = cursor->links[target[index]-'a'];
		index++;
	}

	if(!strcmp(buffer,target)){
		return cursor->frequency;
	}

	return 0; // The Cursor and the buffer are not the same so there are no occurrences in this
}

// Returns the number of docs that the target occurs in
int targetOccursInNumDocs(pageIndex* indexedPages, char* target){
	// Declarations
	int sum, numDocs, index;

	// Initialization
	sum = 0;
	numDocs = indexedPages->numPages;
	index = 0;
	for(index = 0; index<numDocs; index++){
		if(targetOccursNumTimes(indexedPages->pages[index],target)){
			sum++;
		}
	}

	return sum;
}

// Compute the TF-IDF value
double computeTFIDF(double tf,double idf){
	return tf*idf;
}

// Computes the IDF
double computeIDF(int numIndexedDocs, int numContainedTarget){
	return log((1.0+numIndexedDocs)/(1.0+numContainedTarget));
}

// Computers the TF
double computeTF(int tOccurs, int wordsInDoc){
	if(wordsInDoc == 0.0){
		return 0;
	}
	return ( (double) tOccurs ) / ( (double) wordsInDoc );
}

// Sorts the indexed pages by their score
void qsIndexedPages(pageIndex* indexedPages,int low,int high)
{
	int pivot,index1,index2;
	double pivotScore;
	page* temp;
	if(low<high){ // Base case
		pivot=low;
		pivotScore = indexedPages->pages[pivot]->pageScore;
		index1 = low;
		index2 = high;
		while(index1<index2){
			// Find the indexes to swap
			while(indexedPages->pages[index1]->pageScore <= pivotScore 
					&& index1<high){
				index1++;
			}

			while(indexedPages->pages[index2]->pageScore > pivotScore 
					&& index2>=low){
				index2--;
			}
			// When index 1 is lower than index 2, 
			//swap the values @ index 1 and index 2.
			if(index1<index2){
				temp = indexedPages->pages[index1];
				indexedPages->pages[index1] = 
					indexedPages->pages[index2];
				indexedPages->pages[index2] = temp;
			}
		}
		// Swap at the pivot to index 2
		temp = indexedPages->pages[pivot];
		indexedPages->pages[pivot] = indexedPages->pages[index2];
		indexedPages->pages[index2] = temp;

		// recursively sort the lower portion
		qsIndexedPages(indexedPages,low,index2-1); 

		// recursively sort the top portion
		qsIndexedPages(indexedPages,index2+1,high); 
	}
}
