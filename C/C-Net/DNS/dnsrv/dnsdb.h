#ifndef _DNSDB_H
#define _DBSDB_H

typedef struct addr_node
{
	char hostname[100];
	int no;
	char in[3];
	char type[6];
	char ip[16];
} node;

typedef struct addr_list
{
	struct addr_node *addr;
	struct addr_list *next;
}list;

void read_dns_file(list **head,const char *filename);
int add_dns(list **head, node *n);
node* find_dns(list *head, char *hostname);
void print_list(list *head);
void destroy_list(list *head);
void write_out_dns(list *head,const char *filename);

#endif
