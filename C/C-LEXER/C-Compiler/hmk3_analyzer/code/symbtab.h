/***********************************
 File: symtab.h      
 Symbol table interface for the C-Minus compiler                             
 cs106 MUST FIT 2014 FALL
 provided by Zhiyao Liang
************************************/



#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "parse.h"

/* ST_SIZE is the size of the hash table, usually a prime number */
/*#define SIZE 211 */
/* assuming commonly there are no more than 71 declarations in a block of the program.
 *
 */
#define ST_SIZE 71


/* 
 - Each LineListRec corresponds to a reference of a name
 - It is an element of a list attached to a BucketListRec 
 - LineList is the list of the tree nodes of the syntax tree. Each node in this list is a reference of the SAME declaration.
 - The treeNode records all the needed information of the reference, including lineNum

*/
typedef struct LineListRec
{ 
  TreeNode * nd;      // pointing to the node where the name is referenced
  struct LineListRec * prev ; // links to left neighbor
  struct LineListRec * next; // links to right neighbor
  struct BucketListRec * bk; // pointing back to the containing bucket record, which corresponds to the declaration
} * LineList;

/* 
 - Each BucketListRec corresponds to a declaration, 
 - Declarations are distinguished by the block they appear in, even with the same name.
 - A declaration can be a variable, array, or function, or a parameter (with a name, non-void)
 - No need for the assigned memory location (as in the TINY compiler), since the sequence of variables on the top-level provides these information.
 - No need to provide the name, since the tree node has it
   //char * name;
*/
typedef struct BucketListRec
{   
  struct SymbolTable * st; /* pointer to the containing symbol table.*/
  TreeNode * nd;  /*pointer to the declaration node or parameter node in the syntax tree.*/
  LineList lines;  /* pointer to the list of records of reference (line-list-record) of the declaration.*/
  struct BucketListRec* prev; 
  struct BucketListRec* next; 
  void * something;
  /* something is added for possible usage in code generation. For semantic analysis it is not used.
   * For function declaration: it is the address of the function in the code memory
   * For global data: it is the offset to the beginning of global data (in the data memory), which is the highest address of data memory, and is pointed by register gp
   * For parameters and local data: it is the offset to the fp (frame pointer), in the data memory.
   * */
} * BucketList;

// Symbol table storing the declarations appearing in the block.
/*  
- int id;  the unique id of the table (and the block)
-  nd; The node that represents the block corresponding to this symbol table. The node is a compound statement. It is NULL for the top level symbol table.
-  upper:  Link to the symbol table of the upper containing block. It is NULL for the top level.
-lower: a list of symbol tables of the blocks that are contained in this block
- next; // link to the tables of the sibling scope.
- hash_table: the hash table, which stores the declarations in the block
*/
typedef struct SymbolTable{
  int id; 
  TreeNode * nd; 
  struct SymbolTable * upper; 
  struct SymbolTable * lower; 
  struct SymbolTable * prev; 
  struct SymbolTable * next; 
  BucketList hashTable[ST_SIZE];
}SymbTab;

/*
* The st field in a tree node has the following meaning: 
  - for a declaration node, st is a pointer to the bucket-list-record in the symbol table. 
  - for a reference node (where a name is used), st is a pointer to the line-list-record in the symbol table.
  - for other kind of nodes, the st field is meaningless and should not be used 
  - The st field of a tree node will be assigned with a value when symbol tables are being built.
 */

/* st_insert_dcl():
   [computation]:
   Make a BucketListRec according to a declaration or parameter tree node, then insert the BucketListRec into the symbol table st. Associate the tree node and the Bucket List record with each other.
   [Preconditions]: 
   --  st is not NULL, 
   --  dclNd is not NULL 
   --  dclNd  must be a declaration node or parameter node.
   --  st is a (pointer to) symbol table, which must correspond to the scope where the declaration or parameter of dclNd belongs to.
   ----- */ 
void st_insert_dcl(TreeNode * dclNd, SymbTab * st);

/* st_insert_ref()
   [computation]:
   Make a line list rec according to refNd, and insert it into the end of the line list of a  Bucket list record bk. The order of the records in the line list may correspond to the order of their appearance in the program.
   [Preconditions]:
   - refNd is not NULL, and it is a reference to the declaration of bk. 
   - bk is not NULL, and it should correspond to the declaration for the reference of refND.  
   - The above two conditions  mean that bk should be the result of looking up refNd. 
*/
void st_insert_ref(TreeNode *refNd, struct BucketListRec* bk );



/* st_lookup()
   [computation]: 
   Find the bucket list record of the declaration associated with the name.  The search of the name starts from st. If the name is not found in st, then continue the search in the upper level table of st. It returns NULL if not found.
   [parameters]:
   - name is the search key, the name of a variable, parameter, array, or function.
   - st is the starting point of searching. When the function is called st should the symbol table corresponding to the block where the node appears. 
   [preconditions]:
   - st should not be NULL. 
   - name should not be NULL.
*/
struct BucketListRec*  st_lookup (SymbTab* st, const char * name );


/* st_print():
   [computation]: 
   - prints formatted symbol table contents to the listing file. 
   - Table head is printed on top.
   - Prints out error message if the symbol table is wrongly built.
 */
void st_print(FILE * listing, SymbTab * st);

/* st_initialize()
   [computation]:  
   Returns the pointer to an initialized empty symbol table, which is newly created.
*/
SymbTab * st_initialize(void);


/*  st_attach()
    [computation]: 
    - Attach an initialized empty symbol table at the end of st->lower
    - Returns the pointer to the newly added empty symbol table.     
    [Precondition]: st is not NULL
*/
SymbTab * st_attach(SymbTab* st);

#endif
