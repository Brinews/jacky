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


#endif

