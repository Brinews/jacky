#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <signal.h>
#include <assert.h>
#include <errno.h>
#include "bsp.h"

/* Paralleler Server                            */
/* Beschränkung auf maximal MAXCONN Connections */

extern int errno;

volatile int numconnections = 0;


/* Signalhandler, für SIGCHLD */
void handler(int sig) {
   if(sig == SIGCHLD) {
        numconnections --;
        fprintf(stderr, "Sig: num=%d\n", numconnections);
        wait(NULL);
    } 
    return;
}

int main(void)
{
    char line[100],
	*presult;

    socklen_t fromlen; 
    int i,
    	opt,
    	s,
    	ns,
        result,
    	parent;

    struct sockaddr_in sockin, peerin;
    struct hostent* peernameptr;
    struct hostent unknown = {"unknown", NULL, AF_INET, 4, NULL};
    FILE *sockfd;
    struct sigaction act, oldact;

    /* neuer Signalhandler */
    act.sa_handler=handler;

    /* keine anderen Signale sperren */
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;

    /* Signalhandler installieren                   */
    if(sigaction(SIGCHLD, &act, &oldact) < 0) {
            perror("installing signalhandler");
            return EXIT_FAILURE;
    }

    /* Streamsocket holen  */
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("server: socket");
        return EXIT_FAILURE;
    }

    /* Adresse für Server erzeugen */
    sockin.sin_family = AF_INET;
    sockin.sin_port = htons(PORT);
    sockin.sin_addr.s_addr = htonl(INADDR_ANY);

    /* Option REUSEADDR setzen */
    opt = 1;
    result=setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    assert(result >=0);

    /* Adresse binden */
    if (bind(s, (struct sockaddr *) &sockin, sizeof(sockin)) < 0) {
        perror("server: bind");
        return EXIT_FAILURE;
    }

    /* Listen */
    if (listen(s, 5) < 0) {
        perror("server: listen");
        return EXIT_FAILURE;
    }

    /* Serverschleife */
    while(1) {
        /* Accept connections  */
        fromlen = sizeof(peerin);

        if ((ns = accept(s, (struct sockaddr *) &peerin, &fromlen)) < 0) {
            if(errno == EINTR) continue;
            perror("server: accept");
    	    exit(EXIT_FAILURE);
        }
        
        // Socket für gepufferte E/A öffnen
        sockfd = fdopen(ns, "r+");
        assert(sockfd != NULL);
        
        peernameptr = gethostbyaddr(&peerin.sin_addr, 4, AF_INET);
        // Peer im DNS unbekannt?
        if(peernameptr == NULL) {
            peernameptr = &unknown;
        }
        
        numconnections ++;
        fprintf(stderr, "Serv: num=%d\n",numconnections);

        if(numconnections > MAXCONN) {
            numconnections --;

	    	result=fprintf(sockfd, "Hallo %s IP %s Port %d\n",
                            peernameptr->h_name,
                            inet_ntoa(peerin.sin_addr),
                            ntohs(peerin.sin_port)
                          );
	    	assert(result >=0);

	    	result=fputs("Zu viele Clients, schließe!\n",sockfd);
	    	assert(result >=0);
            fclose(sockfd);	/* fclose, damit Puffer geleert wird! */
            continue;

        }

        /* paralleler Server */
        parent = fork();

        /* Kindprozess bedient Anforderung */
        if(!parent) {
            result=fprintf(stderr, "Connected to %s ip %s port %d\n",
                            peernameptr->h_name,
                            inet_ntoa(peerin.sin_addr), 
                            ntohs(peerin.sin_port)
                            );

            result=fprintf(sockfd, "Hallo  %s IP %s Port %d\n",
                            peernameptr->h_name,
                            inet_ntoa(peerin.sin_addr),
                            ntohs(peerin.sin_port)
                            );
            assert(result >=0);

            result=fputs("gib Zeilen ein, Ende ist ENDE\n",sockfd);
            assert(result >=0);

            while((presult=fgets(line, sizeof(line), sockfd))!=NULL) {
                for(i=0; i<strlen(line)-1; i++) {
                    line[i]=toupper(line[i]);
                }
                result=fputs(line, sockfd);
                assert(result >=0);
            }

            result=close(ns);
            assert(result >=0);
            exit(EXIT_SUCCESS);
        }
   }

   return EXIT_SUCCESS;
}
