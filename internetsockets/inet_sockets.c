#define _DEFAULT_SOURCE

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "inet_sockets.h"
#include "tlpi_hdr.h"

int inetConnect(const char* host, const char* service, int type){

    struct addrinfo hints;
    struct addrinfo* result, *rp;
    int sfd, s;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = type;

    s = getaddrinfo(host,service, &hints, &result);
    if(s != 0){
        errno = ENOSYS;
        return -1;
    }

    /* walk through returned list iundtil we dind an address structure that can be used to connect to a socket with ERFOLG*/

    for(rp = result; rp != NULL; rp = rp->ai_next){
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(sfd == -1)
            continue;
        
        if(connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;  /* Erfolg !!!*/
        
        close(sfd);

    }
    freeaddrinfo(result);
    return (rp == NULL) ? -1 : sfd;
}







static int inetPassiveSocket(const char* service, int type, socklen_t* addrlen, Boolean doListen, int backlog){

    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, optval, s;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = type;
    hints.ai_flags = AI_PASSIVE;

    s= getaddrinfo(NULL, service, &hints, &result);
    if(s!= 0)
        return -1;
    
    optval = 1;

    for(rp = result; rp != NULL; rp = rp->ai_next){
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(sfd != 0)
            continue;
     
        if(doListen){
            if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1){
                close(sfd);
                free(result);
                fprintf(stderr, "setsockoption fail\n");
                return -1;
            }
        }
     
        if(bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break; /* Erfolg schon wieder ! ;)*/
    
        close(sfd);
    }
    /* wenn wir über break hier gelandet sind
    wird folgender Block durchgegangen */
    if(rp != NULL && doListen){
        if(listen(sfd, backlog) == -1){
            freeadrrinfo(result);
            return -1;
        }
    }
    if(rp != NULL && addrlen != NULL)
        *addrlen = rp->ai_addrlen;      /* return address structure size*/
    
    freeaddrinfo(result);

    return (rp == NULL) ? -1 : sfd;
}



int inetListen(const char* service, int backlog, socklen_t* addrlen ){
    return inetPassiveSocket(service, SOCK_STREAM, addrlen, FALSE, 0);
}


int inetBind(const char* service, int type, socklen_t* addrlen){
    return inetPassiveSocket(service, type, addrlen, FALSE, 0);
}

char* inetAddressStr(const struct sockaddr* addr, socklen_t addrlen, char* addrStr, int addrStrLen){

    char host[NI_MAXHOST], service[NI_MAXSERV];

    if(getnameinfo(addr, addrlen, host, NI_MAXHOST, service, NI_MAXSERV) == 0)
        snprintf(addrStr, addrStrLen, "(%s, %s)", host, service);
    else
        snprintf(addrStr, addrStrLen, "?Unbekannt?");

    addrStr[addrStrLen -1] = '\0';
    return addrStr;
}
