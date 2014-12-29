
/* ~~~~~~~~~~~~~~~Put Your Information Here ~~~~~~~~~~~~~~
 * Student Name:
 * Student ID:
 * Date:
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */


/***************************************************
 File: symtab.c                                   
 Symbol table implementation for the C-Minus compiler
 Symbol table is implemented as a chained   hash table 
 Symbol tables are linked together according to    scope information     
 Based on the textbook code of Kennth Louden's    
 Compiler Construction: Principles and Practice   
 Provided by Zhiyao Liang
 MUST FIT  Fall 2014
 ****************************************************/


#include "libs.h"
#include "globals.h"
#include "util.h"
#include "parse.h"
#include "symbtab.h"

/* SHIFT is the power of two used as multiplier in hash function  */
#define SHIFT 4


/* hash()
   [computation]: The hash function. It generates an integer according to the key, which is a string.
 */
static int hash( const char * key ){
	int temp = 0;
	int i = 0;
	while (key[i] != '\0'){
		temp = ((temp << SHIFT) + key[i]) % ST_SIZE;
		++i;
	}
	return temp;
}



/* The st field in a tree node has the following meaning: 
  - for a declaration node, st is a pointer to the bucket-list-record in the symbol table. 
  - for a reference node (where a name is used), st is a pointer to the line-list-record int he symbol table. 
  - for other kind of nodes, the st field is meaningless and should not be used 
 */

/* st_insert_dcl():
   [computation]:
   Make a BucketListRec according to dclNd, then insert it into the symbol table st. Associate the node and the record with each other. 
   [Preconditions]: 
   --  st is not NULL, 
   --  dclNd is not NULL 
   --  dclNd  must be a declaration node or parameter node.
   --  st must correspond to the scope where the declaration or parameter of dclNd belongs to. 
   [Implementation notes]:
   insert the record at the beginning of the list, not the end. Since the order of records in a bucket list do not correspond to their appearance order in the scource file, the order of the buck list records does not matter.
   ----- */ 
void st_insert_dcl(TreeNode *dclNd, SymbTab *st){
	int h;
	const char * name ;
	BucketList l;
	struct BucketListRec * b;
	name = dclNd->attr.dclAttr.name;
	h = hash(name);
	l = st->hashTable[h];
	// Insert a bucket list record at the front of the list
	b = (struct BucketListRec *) safe_malloc(sizeof(struct BucketListRec));
	b->st = st;
	b->nd = dclNd; // associate with each other
	/* dclNd-> st =  b; */
	dclNd->something  =  b;
	b->lines = NULL;
	b->next = l;
	b->prev = NULL;
	if(l != NULL)
		l -> prev = b;
	st->hashTable[h] = b;
	return ;
}

/* st_insert_ref()
   [computation]:
   - Make a line-list-rec according to refNd, and insert it into the end of the line list of a  Bucket list record bk.
   - Associate the tree node and the line-list-record with each other.
   - The order of the records in the line list may correspond to the order of their appearance in the program.
   [Preconditions]:
   - refNd is not NULL, and it is a reference to the declaration of bk. 
   - bk is not NULL, and it should correspond to the declaration for the reference of refND.  
   - The above two conditions  mean that bk should be the result of looking up refNd. 
 */
void  st_insert_ref(TreeNode *refNd, struct BucketListRec* bk){
	LineList list = bk->lines;
	struct LineListRec * b = (struct LineListRec *) safe_malloc(sizeof(struct LineListRec));
	b->bk = bk;
	b->nd = refNd;
	refNd->something = b; /* associate with each other */
	b->next = NULL;
	b->prev = NULL;
	if (list == NULL) // still an empty list
		bk->lines = b;
	else { /* insert b to the last place of list */
		while(list->next != NULL )
			list = list->next;
		list->next = b;
		b->prev = list;
	}
	return;
}


/* 
   st_lookup()
   [computation]: 
   Find the bucket list record of the declaration associated with the name.  The search of the name starts from st. If the name is not found in st, then continue the search in the upper level table of st. .  returns NULL if not found. 
   [parameters]:
   - name is the search key, the name of a variable, array, or function. 
   - st is the starting point of searching. When the function is called st should the symbol table corresponding to the block where the node appears. 
   [preconditions]:
   - st should not be NULL. 
   - name should not be NULL.
 */
struct BucketListRec*  st_lookup (SymbTab* st,  const char * name ){
/* !!!!!!!!! Put your code here !!!!!!!!!!!!! */
	int h = hash(name);

	while (st) {
		BucketList bl = st->hashTable[h];

		while (bl != NULL) {

			if (strcmp(bl->nd->attr.dclAttr.name, name) == 0)
				return bl;

			bl = bl->next;
		}

		st = st->upper;
	}

	return NULL;
}

/* st_print():
   [computation]: 
   - prints formatted symbol table contents to the listing file. 
   - Table head is printed on top.
   - Prints out error message if the symbol table is wrongly built.
   [implementation]:
   - uses a static variable to avoid printing the table head in recursive calls. 
 */
void st_print(FILE * listing, SymbTab* st){
	int i;
	static int flag = 0; /* flag is used to control that the table head is only printed once. */
	if (st==NULL) return ;
	if(flag == 0) { /* only print the head of the table once, at the top of the table.*/
		fprintf(listing,"%-6s%-15s%-12s%-5s%-9s\n","Table", "Name","Kind","Dcl","Ref");
		fprintf(listing,"%-6s%-15s%-12s%-5s%-9s\n","ID", "","","line","lines");
		fprintf(listing,"%-6s%-15s%-12s%-5s%-9s\n","----","----","----","----","----");
	}
	for (i=0;i<ST_SIZE;++i){
		BucketList bl = st->hashTable[i];
		while (bl != NULL){
			LineList lines;
			TreeNode * nd = bl->nd;
			fprintf(listing, "%-6d", st->id);
			/* both parameter and declaration store name in attr.dclAttr.name */
			fprintf(listing,"%-15s",nd->attr.dclAttr.name);
			if(nd->nodeKind == DCL_ND) /* a declaration node */
				switch(nd->kind.dcl){
				case VAR_DCL: fprintf(listing,"%-12s","Var"); break;
				case ARRAY_DCL: fprintf(listing,"%-12s","Array"); break;
				case FUN_DCL: fprintf(listing,"%-12s","Func"); break;
				default:
					fprintf(listing,"\n%s\n","st_print(): wrong declaration type, the symbol table records a wrong node.");
					errorFound = TRUE;
					/*exit(0); */
					break;
				}
			else if(nd->nodeKind == PARAM_ND)
				switch(nd->kind.param){
				case VAR_PARAM: fprintf(listing,"%-12s","Var_Param"); break;
				case ARRAY_PARAM: fprintf(listing,"%-12s","Array_Param"); break;
				case VOID_PARAM: fprintf(listing, "%-12s", "Void_Param"); break;
				default:
					fprintf(listing,"\n%s\n","st_print(): wrong parameter type, the symbol table records a wrong node.");
					errorFound = TRUE;
					//exit(0);
					break;
				}
			else { // not a declaration node or parameter node
				fprintf(listing,"\n%s\n","st_print(): wrong node type, the symbol table records a wrong node.");
				errorFound = TRUE;
			}
			fprintf(listing,"%-5d",nd->lineNum);
			lines = bl->lines;
			while (lines != NULL){
				fprintf(listing,"%d ",lines->nd->lineNum);
				lines = lines->next;
			}
			fprintf(listing,"\n");
			bl = bl->next;
		}
	}
	/* now print the lower level scope tables.*/
	flag++;
	st_print(listing, st->lower); /* print tables of  nested scope */
	st_print(listing, st->next);  /* print tables of sibling scopes */
	flag--;
}

/* st_initialize()
   [computation]:  
   Returns the pointer to an initialized empty symbol table. 
 */
SymbTab*  st_initialize(void){
	int i;
	SymbTab* tab;
	/* A counter of the tables. This number will increase each time a table is created. */
	static int tabId = 0;
	/*  sizeof(SymbTab),  not:  sizeof SymbTab*/
	tab = (SymbTab*) safe_malloc(sizeof(SymbTab));
	tab->id = tabId++;
	tab->nd = NULL;
	tab->upper = NULL;
	tab->lower = NULL;
	tab->next = NULL;
	tab->prev = NULL;
	for(i = 0; i<ST_SIZE; i++)
		tab->hashTable[i] = NULL;
	return tab;
}

/*  st_attach()
    [computation]: 
    - Attach an initialized empty symbol table at the end of st->lower
    - Returns the pointer to the newly added empty symbol table.     
    [Precondition]: st is not NULL
 */
SymbTab * st_attach(SymbTab* st){
	SymbTab* newSt = st_initialize();
	SymbTab* last = st->lower;
	newSt->upper = st;
	if (last == NULL)
		st->lower = newSt;
	else{
		while(last->next != NULL) /* move lower to the last record in the list.*/
			last = last->next;
		last->next = newSt; /* attach newSt to the end of the list.*/
		newSt->prev = last;
	}
	return newSt;
}
