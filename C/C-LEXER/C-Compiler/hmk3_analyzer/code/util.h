/****************************************************/
/* File: util.h                                     */
/* The interface of general tools for the           */
/* C-Minus compiler                                 */
/* Compiler Construction: Principles and Practice   */
/* Book author:  Kenneth C. Louden                  */
/* Programming by Zhiyao Liang                      */
/* MUST  2014 Fall                                  */
/****************************************************/

#ifndef _UTIL_H_
#define _UTIL_H_

typedef enum{
  FALSE,
  TRUE
}Boolean;

void my_pause(void);


char * copy_string(const char * );

// safe_malloc is the same as malloc, except that if the result is NULL, an error message is printed and the program is exited.
void * safe_malloc(size_t t);

#endif
