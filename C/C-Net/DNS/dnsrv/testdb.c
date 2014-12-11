#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dnsdb.h"

int main()
{
	list *head = NULL;
	read_dns_file(&head, "cache.conf");

	node *n = (node *) malloc(sizeof(node));
	strcpy(n->hostname, "北京的主页");
	n->no = 1999;
	strcpy(n->in, "KK");
	strcpy(n->type, "cNAME");
	strcpy(n->ip, "1.2.1.1");

	add_dns(&head, n);

	node *t = find_dns(head, "北京的主页");

	if (NULL != t)
	{
		printf("found:%s\n", t->ip);
	}

	print_list(head);

	write_out_dns(head, "cache.conf");

	destroy_list(head);

	return 0;
}
