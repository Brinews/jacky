#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dnsdb.h"

#define BUFSIZE 1024

void read_dns_file(list **head,const char *filename)
{
	char buf[BUFSIZE];

	FILE* fp = fopen(filename, "r");
	if (NULL == fp)
	{
		perror("open file failed.");
	}

	while (fgets(buf, BUFSIZE, fp) != NULL)
	{
		buf[strlen(buf)-1] = '\0';
		node *n = (node *) malloc(sizeof(node));

		/* some urgue code */
		char *sp = strtok(buf, ",");
		if (sp != NULL)
			strcpy(n->hostname, sp);

		sp = strtok(NULL, ",");
		if (NULL != sp)
			sscanf(sp, "%d", &n->no);

		sp = strtok(NULL, ",");
		if (NULL != sp)
			strcpy(n->in, sp);

		sp = strtok(NULL, ",");
		if (NULL != sp)
			strcpy(n->type, sp);

		sp = strtok(NULL, ",");
		if (NULL != sp)
			strcpy(n->ip, sp);

		printf("%s,%d,%s,%s,%s\n", 
				n->hostname, 
				n->no,
				n->in,
				n->type,
				n->ip);


		if (*head == NULL)
		{
			*head = (list *) malloc(sizeof(list));
			(*head)->next = NULL;
			(*head)->addr = n;
		}
		else
		{
			list *p = (list *) malloc(sizeof(list));
			p->addr = n;
			p->next = *head;

			*head = p;
		}
	}

	fclose(fp);
}

int add_dns(list **head, node *n)
{
	if (*head == NULL)
	{
		*head = (list *) malloc(sizeof(list));

		(*head)->next = NULL;
		(*head)->addr = n;
	}
	else
	{
		list *tail = *head;

		while (tail->next != NULL)
		{
			tail = tail->next;
		}

		list *toadd = (list *) malloc(sizeof(list));
		toadd->addr = n;
		toadd->next = NULL;

		tail->next = toadd;
	}

	return 1;
}

node* find_dns(list *head, char *hostname)
{
	list *p = head;

	while (p != NULL)
	{
		if (strcmp(p->addr->hostname, hostname) == 0)
		{
			return p->addr;
		}

		p = p->next;
	}

	return NULL;
}

void destroy_list(list *head)
{
	list *p;

	while (head != NULL)
	{
		p = head;
		head = head->next;

		free(p->addr);
		free(p);
	}
}

void print_list(list *head)
{
	while (head != NULL)
	{
		node *n = head->addr;

		printf("%s,%d,%s,%s,%s\n", 
				n->hostname, 
				n->no,
				n->in,
				n->type,
				n->ip);

		head = head->next;
	}
}

void write_out_dns(list *head, const char *filename)
{
	FILE *fp = fopen(filename, "w");
	if (NULL == fp)
		perror("open file to write failed.");

	while (head != NULL)
	{
		node *n = head->addr;

		fprintf(fp, "%s,%d,%s,%s,%s\n", 
				n->hostname, 
				n->no,
				n->in,
				n->type,
				n->ip);

		head = head->next;
	}

	fclose(fp);
}
