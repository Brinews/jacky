#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "defs.h"

char *stringDuplicate(const char *str)
{
	unsigned long n = strlen(str) + 1;
	char *dup = malloc(sizeof(char) * n);
	if ( dup ) {
		strcpy(dup, str);
	}
	return dup;
}

bool stringEquality(const char *s1, const char *s2)
{
	if ( !s1 || !s2 ) {
		return false;
	}
	
	return strcmp(s1, s2) == 0;
}

char *lowerCaseVersionOfString(const char *s)
{
	char *dup = stringDuplicate(s);
	for ( char *p = dup; *p != '\0'; ++p) *p = tolower(*p);
	return dup;
}

int strcaseCmp(const char *s1, const char *s2, size_t n)
{
	// Convert to lower-case.
	char *s3 = lowerCaseVersionOfString(s1);
	char *s4 = lowerCaseVersionOfString(s2);
	
	int result = strncmp(s3, s4, n);
	
	free(s3);
	free(s4);
	
	return result;
}

char *substring(const char *begin, const char *end)
{
	// End must be after begin.
	if ( end <= begin ) {
		return NULL;
	}
	
	size_t stringLength = (end-begin);
	
	char *sub = malloc(sizeof(char) * (stringLength + 1) );
	
	char *beginning = (char *) begin;
	
	memcpy(sub, beginning, sizeof(char) * stringLength);
	
	sub[stringLength] = '\0';
	
	return sub;
}


bool splitStringAtString(const char *fullString, const char *delimiter, char **firstString, char **secondString)
{
	// Must have valid fullString and delimiter.
	if ( fullString == NULL || delimiter == NULL ) {
		return false;
	}
	
	char *occurance = strstr(fullString, delimiter);
	
	if ( occurance == NULL ) {
		return false;
	}
	
	*firstString = substring(fullString, occurance);
	
	*secondString = stringDuplicate(occurance + strlen(delimiter));
	
	return true;
}
