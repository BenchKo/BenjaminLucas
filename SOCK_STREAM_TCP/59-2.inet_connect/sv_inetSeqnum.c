#define _DEFAULT_SOURCE

#include <netdb.h>
#include "is_seqnum.h"

#define BACKLOG 50

int main(int argc, char* argv[]){

    uint32_t seqNum;
    char reqLenStr[INT_LEN];        /* länge des angeforderten Strings*/
    char seqNumStr[INT_LEN];        /*  start of granted sequence*/
    struct sockaddr_storage claddr;
    int lfd;
    int cfd;
    int optval;
    int addrError;
    int reqLen;
    socklen_t addrlen;
    struct addrinfo hints;
    struct addrinfo* rp;
    struct addrinfo* result;
#define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)
    char addrStr[ADDRSTRLEN];
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    if(argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [init-seq-num]\n", argv[0]);
    
    seqNum = (argc > 1) ? atoi(argv[1]) : 0;
    
    if(signal(SIGPIPE, SIG_IGN) == SIG_ERR)
        errExit("signal");

   /* hier die inet funktion einfügen inetListen */
    lfd = inetListen(PORT_NUM, BACKLOG, &addrlen);
    if(lfd == -1){
        perror("inetListen");
        exit(EXIT_FAILURE);
    }

    printf("Server wartet auf eingehende Verbindungen...\n");
    for(;;){
        cfd = accept(lfd, (struct sockaddr* ) &claddr, &addrlen);
        if(cfd == -1){
            continue;
        }
        if(getnameinfo( (struct sockaddr*) &claddr, addrlen, host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
            snprintf(addrStr, ADDRSTRLEN, "%s, %s", host, service);
        else
            snprintf(addrStr, ADDRSTRLEN, "?UNKNOWN?");
        printf("Connection from %s\n", addrStr);

        /* Read Client reqest, send reqnum back*/
        if(readLine(cfd, reqLenStr, INT_LEN) <= 0){
            close(cfd);
            continue;      /* failed read; skip request*/
        }
        reqLen = atoi(reqLenStr);
        if(reqLen <= 0){            /* watch for misbehaving clients*/
            close(cfd);
            continue;
        }

        snprintf(seqNumStr, INT_LEN, "%d", seqNum);
        if(write(cfd, &seqNumStr, strlen(seqNumStr)) != strlen(seqNumStr))
            fprintf(stderr, "Error on Write");
        
        seqNum += reqLen;
        if(close(cfd) == -1)
            errMsg("close");
    } 

}