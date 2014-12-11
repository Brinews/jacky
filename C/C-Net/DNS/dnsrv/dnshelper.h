#include "ndns.h"

struct gethost_reply {
  int type;             /* -1: Not found, 0: Found IP, 1: Found nameserver */
  char details[100];
};

typedef struct peer_request
{
	char type[2];
	char hostname[100];
	char ip[16];
} peer_rq;

typedef struct gethost_reply ghreply;
ghreply ngethostbyname(char  *, char  *, int, int);
void ChangetoDnsNameFormat (char *,char *);
char * ReadName (char *,char *,int*);

peer_rq* peerRequest(char *buf, int size, char *ip);
