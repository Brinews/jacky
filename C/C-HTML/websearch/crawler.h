#ifndef CRAWLER_H
#define CRAWLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ADDR_LENGTH 1000 // for crawler

typedef struct listNode{
  char addr[MAX_ADDR_LENGTH];

  struct listNode *link;
} listNode;//crawler struct

/* returns 1 if the list starting at pNode contains the address addr,
   and returns 0 otherwise
*/
int contains(const struct listNode *pNode, const char *addr);
/* inserts the address addr as a new listNode at the end of
    the list
*/
void insertBack(struct listNode *pNode, const char *addr);
/* prints the addresses from pNode to the end of the list,
   one on each line
*/
void printAddresses(const struct listNode *pNode);
/* frees the memory associated with this node and all subsequent nodes
 */
void destroyList(struct listNode *pNode);
/* srcAddr should be a web address (e.g., http://www.yahoo.com).
 * link should point to an array of maxLinkLength characters.
 * getLink returns 1 if a link was found and 0 otherwise.
 * If a link was found, "link" will be filled in with the web address.
*/
int getLink(const char* srcAddr, char* link, const int maxLinkLength);

#endif
