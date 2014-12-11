#include "crawler.h"
#include "index.h"

pageIndex* indexPagesFromFile(const char* filename, int maxPages)
{
	/* Variable Declarations */
	page** pages; 
	pageIndex* indexedPages;

	FILE* indexFile;
	char url[MAX_ADDR_LENGTH];
	int maxHops, hopNum;
	listNode* listStart;
	int i;

	//The File to Index
	indexFile = fopen(filename, "r");
	if(indexFile == NULL){
		fprintf(stderr, "ERROR: Could not open the file\n");
		return NULL;
	}

	//Page index (array) 
	pages = malloc(sizeof(page*)*(maxPages+1));
	if(pages == NULL){
		fprintf(stderr, "ERROR: Could not allocate memory\n");
		fclose(indexFile);
		return NULL;
	}

	// Page index structure
	indexedPages = malloc(sizeof(pageIndex));
	if(indexedPages == NULL){
		fprintf(stderr, "ERROR: COULD NOT ALLOCATE MEMORY\n");
		fclose(indexFile);
		free(pages);
		return NULL;
	}

	// Point the pages convenient pointer to this
	indexedPages->pages = pages; 

	// Make the list start
	listStart = malloc(sizeof(listNode));
	if(listStart == NULL){
		fprintf(stderr, "ERROR: Could not allocate memory\n");
		fclose(indexFile);
		free(pages);
		free(indexedPages);
		return NULL;
	}
	listStart->link = NULL;

	// Indexed pages Initialization
	i = 0;
	while((fscanf(indexFile, "%1000s %d",url,&maxHops) == 2) 
			&& i<maxPages)
	{
		hopNum = 0; // Reset hop-number since this is a new hop
		while(1){ // Wrapper loop for jumping in this URL
			// This runs if we are on the first URL
			if(i == 0){
				// Copy the string to the address of the start list
				strncpy(listStart->addr,url,MAX_ADDR_LENGTH);
				// put the built page into the pages at the index
				pages[i] = buildPage(url);
				// If the pages at i is NULL then we have an error, we need to clean up and exit
				if(pages[i] == NULL){
					destroyList(listStart);
					fclose(indexFile);
					free(pages);
					destroyPageIndex(indexedPages);
					return NULL;
				}
				i++; // Iterate the index
			}
			// This is the next URL (IDENTICAL TO ABOVE EXCEPT FOR THE CONTAINS)
			else{
				if(!contains(listStart,url)){
					insertBack(listStart,url);
					pages[i] = buildPage(url);
					if(pages[i] == NULL){
						destroyList(listStart);
						fclose(indexFile);
						free(pages);
						destroyPageIndex(indexedPages);
						return NULL;
					}
					i++;
				}
			}
			hopNum++; // Increase the number of hops
			if(hopNum<=maxHops && i<maxPages){
				char nextURL[MAX_ADDR_LENGTH];
				if(getLink(url,nextURL,MAX_ADDR_LENGTH)){
					strncpy(url,nextURL,MAX_ADDR_LENGTH);
				}
				else{
					break;
				}
			}
			else{
				break;
			}
		}
	}

	// numPages is the i variable 
	indexedPages->numPages = i;
	//Close the file
	fclose(indexFile);

	// Free the list we made because we don't need it any more
	destroyList(listStart);

	return indexedPages;
}

/*
 * Builds a page with information from the url, 
 * see the header file for more information
 */
page* buildPage(char* url)
{
	page* thePage; // The page that we are building

	int numWords = 0; // A holder for the number of words in the page
	thePage = malloc(sizeof(page)); //  Allocate space for the page
	// Error checking for malloc
	if(thePage == NULL){
		fprintf(stderr,"ERROR: COULD NOT ALLOCATE MEMORY");
		return NULL;
	}
	// Index the page with pageIndexer function indexPage, also retrieve the numWords from the page
	thePage->trie = indexPage(url, &numWords);
	// Write the numb pages to the variable total terms in the page
	thePage->totalTerms = numWords;
	strncpy(thePage->url,url,MAX_ADDR_LENGTH); // Copy the URL into the url field of the page
	return thePage;
}


// Indexes page returns the head of the trie
tnode* indexPage(const char* url,int* numWords)
{
	char wordBuffer[MAX_WORDSIZE]; // Creates a word buffer
	int bIndex = 0; // buffer index
	int wbIndex = 0; // word buffer index
	char buffer[MAX_BUFFERSIZE]; // initializes the buffer that will have all text
	tnode *head;

	fprintf(stdout,"%s\n",url);

	getText(url,buffer,MAX_BUFFERSIZE); // Gets the text, puts it in the buffer (up to max size)

	head = initializeNewNode('\0'); // initialize the head node (initialize new node uses malloc)(null char, we can ignore when printing)
	if(head == NULL){
		printf("Unable to create head node of page: %s",url);
		return NULL;
	}
	while(buffer[bIndex] != '\0'){
		/*
		   If the letter in question is between a 'a' and 'z' it can be placed in the buffer
		 */
		if(buffer[bIndex] >= 'a' && buffer[bIndex] <= 'z'){
			wordBuffer[wbIndex] = buffer[bIndex]; 
			wbIndex++;
		}
		/*
		   If the letter is capital the letter is converted to lower case then placed in the buffer
		 */
		else if(buffer[bIndex] >= 'A' && buffer[bIndex] <= 'Z'){
			wordBuffer[wbIndex] = tolower(buffer[bIndex]);
			wbIndex++;
		}
		/*
		   This either indicates the end of the word, or needs to continue on
		 */
		else{
			if(wbIndex > 0){ // if wbIndex>0 that means there is a word on the buffer
				wordBuffer[wbIndex] = '\0'; // Null-terminate the word buffer
				printf("\t%s\n",wordBuffer); // Print the word (as per req.)
				// Adds the word to the trie, does error checking
				if(addWordOccurrence(wordBuffer,strlen(wordBuffer),head)){
					printf("%s could not be added",wordBuffer);
				}
				(*numWords)++;
				wbIndex = 0; // resets the word buffer index
			}
			// Otherwise nothing needs to be done, so that if is skipped
		}
		bIndex++; // Iterate through the buffer
	}

	return head; // Returns the head of the trie
}

// This adds a single word to the trie (given the head of the trie the word  and the word length
int addWordOccurrence(const char* word, const int wordLength, tnode* node)
{
	if(wordLength == 0){ // end of the word (the node we at is the right node)
		node->frequency = node->frequency + 1; // We are at the end of the word, the previous node was the last letter
		return 0; // Returns 0 because there was no error
	}
	else{
		int linkPos = word[0] - 'a'; // This is the index we want for the link(all of the text should be lower case by the time it gets to this function)
		if(node->links[linkPos] == NULL){ // No node exists yet
			tnode* newNode = initializeNewNode(word[0]); // creates a new node with the value of the word
			if(newNode == NULL){
				printf("Node could not be allocated");
				return -1;
			}
			node->links[linkPos] = newNode; // links this node to the new node
			return addWordOccurrence(&word[1],wordLength-1,newNode); // recurses to look for the end of the word
		}
		else{ // The node already exists
			// oh look, we have a link so just go down the rabbit hole of recursion to look for end of the word
			return addWordOccurrence(&word[1],wordLength-1,node->links[linkPos]); 
		}
	}

}

// Destroys the trie given the root node
int destroyTree(tnode* root)
{
	int lIndex; // link index
	for(lIndex=0;lIndex<ALPHA_LENGTH;lIndex++){
		if(root->links[lIndex] != NULL){ // There is a node to be free here
			destroyTree(root->links[lIndex]); // Free that node and all others below it
		}
	}
	free(root); // Free the root
}

// Initializes a new tnode
tnode* initializeNewNode(char value)
{
	int i;
	tnode* newNode = malloc(sizeof(tnode)); // malloc a new node
	if(newNode == NULL){
		printf("New tnode could not be allocated");
		return NULL;
	}
	newNode -> frequency = 0;
	newNode -> value = value;

	for(i=0;i<ALPHA_LENGTH;i++){
		newNode->links[i] = NULL;
	}

	return newNode;
}

/*
 * Frees all associates memory with the page index
 */
void destroyPageIndex(pageIndex* pages){
	//printf("In Destroy Page Index\n");
	int i;
	for(i = 0;i<pages->numPages;i++){
		//printf("Killing page %d\n",i);
		page* thisPage = pages->pages[i];
		if(thisPage == NULL){
			fprintf(stderr,"ERROR: Page does not exist\n");
			return;
		}
		freePage(thisPage);
	}
	free(pages->pages);
	free(pages);
}

// Frees the page (destroys all the stuff under it)
void freePage(page* thisPage){
	destroyTree(thisPage->trie);
	free(thisPage);
}

/* You should not need to modify this function */
int getText(const char* srcAddr, char* buffer, const int bufSize){
	FILE *pipe;
	int bytesRead;

	snprintf(buffer, bufSize, "curl -s \"%s\" | python getText.py", srcAddr);

	pipe = popen(buffer, "r");
	if(pipe == NULL){
		fprintf(stderr, "ERROR: could not open the pipe for command %s\n",
				buffer);
		return 0;
	}

	bytesRead = fread(buffer, sizeof(char), bufSize-1, pipe);
	buffer[bytesRead] = '\0';

	pclose(pipe);

	return bytesRead;
}
