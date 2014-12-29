/****************************************************/
/* File: util.c                                     */
/* general tools for the  C-Minus compiler          */          
/* Compiler Construction: Principles and Practice   */
/* Book author:  Kenneth C. Louden                  */
/* Programming by Zhiyao Liang                      */
/* MUST FIT  2014 Fall                    */
/****************************************************/

#include "libs.h"

/* Function copyString allocates space and makes a new
 * copy of an existing string
 * Caller should check if the return value is NULL
 * which means out of memory
 */
char * copy_string(const char * s)
{ int n;
  char * t;  /* note that if using const char *t, then the strcpy(t, s) below will have a warning. */
  if (s==NULL) return NULL;
  n = strlen(s)+1;  
  t = (char *)malloc(n*sizeof(char)); 
  // malloc returns void *, change the type to char *. Size of a char is always a byte
  if (t != NULL)
    strcpy(t,s);  // Including the terminating null byte of s, s is copied to t.
  else
    printf("in copy_string(), malloc return NULL\n");
  return t;
}

/* Use this function wisely, do not make the program surprisingly suspend.*/
void my_pause(void){
  while(getchar() != '\n');
}
 

// size_t is defined in stddef.h. NULL is also defined in this file. Seems like no need to include this file,
// safe_malloc is the same as malloc, except that if the result is NULL, an error message is printed and the program is exited.
void * safe_malloc(size_t t){
  void * result;
  if (t <= 0) {
    printf("wrong argument to safe_malloc()");
    exit(0);
  }
  result = malloc(t);
  if(result == NULL){
    printf("!!! malloc() failed, maybe out of memory. Exit the program now\n");
    exit(1);
  }
  else
    return result;
}
