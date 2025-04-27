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

    /* call getaddrinfo() to obtain a list of addresses that we can try to bind*/
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;
    /* wildcard ip und service ist numerisch*/

    if((addrError = getaddrinfo(NULL, PORT_NUM, &hints, &result)) != 0)
        printf("getaddrinfo: %s\n", gai_strerror(addrError));
    
    /* walk through returned list until we find an adress structure that can be used to 
        successfully create and bind a socket */
    optval = 1;
    for(rp = result; rp != NULL; rp = rp->ai_next){
        lfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if( lfd == -1)
            continue;       /* on error try next adress*/

        if(setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
            errExit("setsockopt");
        
        if(bind(lfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;

        /*bind failed close socket and try next address*/
        close(lfd) ;

    }
    if(rp == NULL)
        fatal("COuld not bind socket to any address\n");

    if(listen(lfd, BACKLOG) == -1)
        errExit("listem");
    
    freeaddrinfo(result);

    for(;;){
        printf("Server wartet auf eingehende Verbindungen...\n");
        addrlen = sizeof(struct sockaddr_storage);
        cfd = accept(lfd, (struct sockaddr* ) &claddr, &addrlen);
        if(cfd == -1){
            errMsg("accept");
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