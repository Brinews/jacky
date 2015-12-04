/*
 * Date 	: 2015-10-26
 * StuNumber: 301213518
 * StuName  : xiaonan xu
 *
 * @basic DHT simulation
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct id_node_t*  id_node;
typedef struct key_node_t*  key_node;

struct key_node_t {
	int keys;					//hashed keys
	key_node next;
};

struct id_node_t {
	int ids;					//hashed node ids

	key_node key_list;			//keys on node

	id_node first_sibling;		//first next node
	id_node second_sibling;		//second next node

	id_node prev_sibling;		//previous node
};

int HASHSPACE;

int bdht_hash(int val, int space);					//hash function
int bdht_query(id_node head, int keyid, int nodeid);//get query path length

id_node bdht_build();							//DHT init
void	bdht_destroy(id_node list);				//DHT destroy
void	bdht_destroy_key(key_node list);

id_node bdht_add_ids(id_node head, int ids_no);	//add node into DHT
void bdht_add_keys(id_node head, int keys_no);	//add keys to node

void bdht_add_keys(id_node head, int keys_no)
{
	int hash_key = bdht_hash(keys_no, HASHSPACE);
	key_node knode;

	//empty node list
	if (head->first_sibling == head) return;

	// init key node
	knode = (key_node) malloc(sizeof(key_node_t));
	knode->keys = keys_no;

	id_node ptr = head;

	// locate ids node
	while (head->prev_sibling != ptr) {
		if (hash_key > ptr->ids) ptr = ptr->first_sibling;
		else break;
	}

	// add key node
	if (hash_key > ptr->ids) {
		// key will on the first node
		ptr = head->first_sibling;
	}

	knode->next = ptr->key_list;
	ptr->key_list = knode;
}

id_node bdht_add_ids(id_node head, int ids_no)
{
	id_node idn = NULL;

	idn = (id_node) malloc(sizeof(id_node_t));
	idn->key_list = NULL;
	idn->ids = ids_no;

	// add to list
	// first locate the prev node
	id_node ptr = head;
	while (head->prev_sibling != ptr) {
		if (ids_no > ptr->ids) ptr = ptr->first_sibling;
		else break;
	}

	// next add to list
	if (ptr == head && ptr->first_sibling == ptr) {
		// when there is only one node
		idn->first_sibling = ptr;
		idn->second_sibling = idn;
		idn->prev_sibling = ptr;

		ptr->first_sibling = idn;
		ptr->second_sibling = ptr;
		ptr->prev_sibling = idn;
	} else {

		// add before
		if (idn->ids < ptr->ids) ptr = ptr->prev_sibling;

		// add after
		idn->first_sibling = ptr->first_sibling;
		idn->second_sibling = ptr->second_sibling;
		idn->prev_sibling = ptr;

		ptr->first_sibling->prev_sibling = idn;
		ptr->prev_sibling->second_sibling = idn;

		ptr->second_sibling = ptr->first_sibling;
		ptr->first_sibling = idn;
	}

	return head;
}

id_node bdht_build()
{
	int node_num, key_num;			// hash space, count of node, key
	int node_id, key_id;			// node id, key id
	int i;

	id_node head = NULL; 

	head = 	(id_node) malloc(sizeof(id_node_t));	

	// init dummy node
	head->ids = -1;
	head->key_list = NULL;
	head->first_sibling = head->second_sibling = head;
	head->prev_sibling = head;

	scanf("%d", &HASHSPACE);
	scanf("%d", &node_num);
	scanf("%d", &key_num);

	// add node ids
	for (i = 0; i < node_num - 1; i++) {
		scanf("%d,", &node_id);

		node_id = bdht_hash(node_id, HASHSPACE);

		bdht_add_ids(head, node_id);
	}

	scanf("%d", &node_id);
	bdht_add_ids(head, bdht_hash(node_id, HASHSPACE));

	// add keys
	for (i = 0; i < key_num - 1; i++) {
		scanf("%d,", &key_id);
		bdht_add_keys(head, key_id);
	}

	scanf("%d", &key_id);
	bdht_add_keys(head, key_id);

	return head;
}

int bdht_hash(int val, int space)
{
	return val % space;
}

int bdht_find_key(key_node klist, int key)
{
	key_node ptr = klist;

	while (ptr != NULL) {
		if (ptr->keys == key) return 1;

		ptr = ptr->next;
	}

	return 0;
}

int bdht_query(id_node head, int qkey, int qid)
{
	// empty list
	if (head->first_sibling == head) return -1;

	// locate key
	id_node ptr = head->first_sibling;
	while (ptr != head) {
		if (ptr->ids == qid) break;

		ptr = ptr->first_sibling;
	}

	int path = 0;

	if (bdht_find_key(ptr->key_list, qkey)) return path;

	id_node prev = ptr->prev_sibling;

	while (ptr != prev) {

		if (path != 0 && ptr == prev->first_sibling) break;

		path++;
		if (ptr->first_sibling->ids == bdht_hash(qkey, HASHSPACE)
			|| ptr->second_sibling->ids == bdht_hash(qkey, HASHSPACE)
			|| bdht_find_key(ptr->first_sibling->key_list, qkey)
				|| bdht_find_key(ptr->second_sibling->key_list, qkey)) 
			break;

		ptr = ptr->second_sibling;
	}

	return path;
}

void bdht_destroy_key(key_node list)
{
	while (list != NULL) {
		key_node temp = list->next;
		free(list);
		list = temp;
	}
}

void bdht_destroy(id_node head)
{
	id_node ptr = head->first_sibling;

	while (ptr != head) {
		id_node temp = ptr->first_sibling;

		bdht_destroy_key(ptr->key_list);
		free(ptr);

		ptr = temp;
	}

	bdht_destroy_key(ptr->key_list);
	free(ptr);
}

int main(int argc, char **argv)
{
	int query_id, query_key;	// key and query node id
	char buf[1024];

	printf("Input File Name:");
	scanf("%s", buf);

	freopen(buf, "r", stdin);

	char out[1024] = {'\0'};
	int i;
	for (i = 0; buf[i] != '.'; i++) {
		out[i] = buf[i];
	}

	out[i] = '.';
	out[i+1] = 'o';
	out[i+2] = 'u';
	out[i+3] = 't';
	out[i+4] = '\0';


	freopen(out, "w", stdout);

	id_node dht_list;			// DHT table list

	dht_list = bdht_build();

	while (scanf("%d,%d", &query_key, &query_id) > 0) {
		if (query_id < 0 || query_key < 0) break;

		printf("%d\n", bdht_query(dht_list, query_key, query_id));
	} 
	bdht_destroy(dht_list);

	return 0;
}
