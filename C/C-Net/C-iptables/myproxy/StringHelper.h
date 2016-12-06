
#ifndef _StringHelper_h
#define _StringHelper_h

#include "defs.h"

// Duplicate a string.
char *stringDuplicate(const char *str);

// Test the equality of two strings.
bool stringEquality(const char *s1, const char *s2);

// Compare two ascii strings and ignoring case.
int strcaseCmp(const char *s1, const char *s2, size_t n);

// Create a substring.
char *substring(const char *begin, const char *end);

// Split a string into two with a string separator.
bool splitStringAtString(const char *fullString, const char *delimiter, char **firstString, char **secondString);

#endif
