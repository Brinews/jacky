#include "webSearch.h"

#include "string.h"
#include "stdlib.h"

#define BUFLEN	1024
char buf[BUFLEN];

char *getQuery()
{
	printf("Enter a web Query:\n");
	memset(buf, 0, sizeof(buf));

	fgets(buf, sizeof(buf), stdin);

	return buf;
}

void search(pageIndex* pages, char *query)
{
	const char delim[] = " ";
	char *result[20] = {NULL};
	int i = 0, k = 0, j;
	int ocm[20] = {0};

	result[i] = strtok(query, result[i]);

	while (result[i] != NULL)
	{
		ocm[i] = targetOccursInNumDocs(pages, result[i]);
		if (ocm[i] > 0) k++;
		i++;


		result[i] = strtok(NULL, delim);
	}

	printf("Query is \"");
	for (j = 0; j < i; j++)
	{
		if (ocm[j] > 0)
			printf(" %s", result[j]);
	}
	printf("\".\n");
	
	printf("IDF scores are\n");
	for (j = 0; j < i; j++)
	{
		if (ocm[j] > 0)
		{
			printf("IDF(%s): %.7lf\n", result[j], 
					computeIDF(pages->numPages, ocm[j]));
		}
	}

	printf("Web Pages:\n");
	/* todo*/
}

int main(int argc, char** argv){

	int i;
	long seed;
	long maxPages;
	pageIndex* pages;
	char* query;
	int valid;

	if(argc < 3 || argc > 4){
		fprintf(stderr, "USAGE: %s fileOfPages maxPagesToIndex [rand seed]\n", argv[0]);
		return -1;
	}

	/* initialization */
	if(argc >= 4){
		seed = atol(argv[3]);
	}
	else{
		seed = time(NULL);
	}

	srand(seed);
	maxPages = atol(argv[2]);

	/********************PHASE I: INDEXED ******************************/

	printf("In Indexing...\n"); 

	/* index the page, need free */
	pages = indexPagesFromFile(argv[1], maxPages); 
	if(pages == NULL){ // ERROR CHECKING
		fprintf(stderr, "ERROR: INDEX NOT CREATED\n");
		return -1;
	}

	/********************PHASE II: QUERY ******************************/

	/* input query string, null for end */
	while((query = getQuery()))
	{ 
		if (strlen(query) == 0)
		{
			printf("Exiting the program");
			break;
		}

		valid = 0;
		for (i = 0; i < strlen(query); i++)
		{
			if (query[i] >= 'z' || query[i] <= 'a' || query[i] != ' ')
			{
				valid = 1;
				break;
			}
		}
		
		if (valid) {
			printf("Please enter a query containing only lower-case letters. ");
			continue;
		}

		search(pages, query); // Search the index for the query
	}

	destroyPageIndex(pages); // memory cleanup

	return 0;
}
