#include "mlist.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int comp(void *p, void *q)
{
    char *p1 = (char *)p;
    char *p2 = (char *)q;

    return strcmp(p1, p2);
}

int main()
{
    LIST *L1 = ListCreate();
    LIST *L2 = ListCreate();

    ListAdd(L1, "The ");

    ListAdd(L1, "jump ");
    ListInsert(L1, "fox ");
    ListAppend(L1, "over ");
    ListPrepend(L1, "Look ");

    ListAppend(L2, "dog");

    ListConcat(L1, L2);

    printf("%s\n", ListFirst(L1));
    assert(strcmp(ListLast(L1), "dog") == 0);
    assert(strcmp(ListFirst(L1), "Look ") == 0);
    assert(strcmp(ListNext(L1), "The ") == 0);
    assert(strcmp(ListPrev(L1), "Look ") == 0);
    assert(strcmp(ListCurr(L1), "Look ") == 0);

    assert(strcmp(ListRemove(L1), "Look ") == 0);
    assert(strcmp(ListCurr(L1), "The ") == 0);

    assert(strcmp(ListTrim(L1), "dog") == 0);
    assert(strcmp(ListLast(L1), "over ") == 0);

    assert(strcmp(ListFirst(L1), "The ") == 0);
    assert(strcmp(ListSearch(L1, comp, "fox "), "fox ") == 0);

    return 0;
}
