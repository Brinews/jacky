#include "crawler.h"

/*
 * returns 1 if the list starting at pNode contains the address addr,
 *    and returns 0 otherwise
 */
int contains(const listNode *pNode, const char *addr)
{
	// Can use strcmp(one, the other) (returns zero if equal)
	if(pNode->link == NULL){
		if(!strcmp(addr,pNode->addr)){
			return 1;
		}
		else{
			return 0;
		}
	}
	else{
		if(!strcmp(addr,pNode->addr)){
			return 1;
		}
		else{
			return contains(pNode->link,addr);
		}
	}
}

/*
 * inserts the address addr as a new listNode at the end of
 *    the list
 */
void insertBack(listNode *pNode, const char *addr)
{
	if(pNode->link == NULL){ // Base case at end of the list
		listNode *newNode;
		newNode = malloc(sizeof(listNode)); // makes the new node on the heap (pointer for convince)
		if(newNode == NULL){
			return;
		}
		pNode->link = newNode; // links the current node to the new one
		strncpy(newNode->addr, addr, MAX_ADDR_LENGTH); // copies the address into the address field of the new node
		newNode->link = NULL; // links this new node to NULL (if not done so already)
	}
	else{
		insertBack(pNode->link,addr); // Recurses (not at end of list no action taken)
	}
}

/*
 * prints the addresses from pNode to the end of the list,
 *   one on each line
 */
void printAddresses(const struct listNode *pNode){
	printf("%s\n",pNode->addr); 
	if(pNode->link != NULL){
		printAddresses(pNode->link);    
	}
}

/*
 * frees the memory associated with this node and all subsequent nodes
 */
void destroyList(listNode *pNode){
	if(pNode->link == NULL){ // Base Case of end of the list null
		free(pNode); // frees the last node and will return
	}
	else{
		listNode *next = pNode->link; // Stores the next variable since it will disappear when we free pNode from the heap
		free(pNode); // freeing pNode
		destroyList(next); // Going to the next element
	}
}

#define bufSize 1000

/* Credit: Prof. Briton Wolfe */
int getLink(const char* srcAddr, char* link, const int maxLinkLength){
	//const int bufSize = 1000;
	char buffer[bufSize];

	int numLinks = 0;

	FILE *pipe;

	snprintf(buffer, bufSize, "curl -s \"%s\" | python getLinks.py", srcAddr);

	pipe = popen(buffer, "r");
	if(pipe == NULL){
		fprintf(stderr, "ERROR: could not open the pipe for command %s\n",
				buffer);
		return 0;
	}

	fscanf(pipe, "%d\n", &numLinks);

	if(numLinks > 0){
		int linkNum;
		double r = (double)rand() / ((double)RAND_MAX + 1.0);

		for(linkNum=0; linkNum<numLinks; linkNum++){
			fgets(buffer, bufSize, pipe);

			if(r < (linkNum + 1.0) / numLinks){
				break;
			}
		}

		/* copy the address from buffer to link */
		strncpy(link, buffer, maxLinkLength);
		link[maxLinkLength-1] = '\0';

		/* get rid of the newline */
		{
			char* pNewline = strchr(link, '\n');
			if(pNewline != NULL){
				*pNewline = '\0';
			}
		}
	}

	pclose(pipe);

	if(numLinks > 0){
		return 1;
	}
	else{
		return 0;
	}
}
