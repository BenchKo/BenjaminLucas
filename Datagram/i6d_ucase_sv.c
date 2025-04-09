#include "i6d_ucase.h"
#include "error_functions.h"

int main() {

    struct sockaddr_in6 svaddr;
    struct sockaddr_in6 claddr;

    int sfd;
    int j;
    socklen_t len;
    ssize_t numBytes;
    char buf[BUFSIZE];
    char claddrStr[INET6_ADDRSTRLEN];

    sfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if(sfd == -1)
        errExit("socket");

    /* Adress STruktur initislisieren*/

    memset(&svaddr, 0, sizeof(struct sockaddr_in6));
    svaddr.sin6_family = AF_INET6;
    svaddr.sin6_addr = in6addr_any;                 /* Wildcard adress*/
    svaddr.sin6_port = htons(PORT_NUM);

    if(bind(sfd, (struct sockaddr*) &svaddr, sizeof(struct sockaddr_in6)) == -1)
        errExit("bind");

    /* Receive messages, converts to uppercase, and return to client*/

    while(1){

        len = sizeof(struct sockaddr_in6);

        numBytes = recvfrom(sfd, buf, BUFSIZE, 0, (struct sockaddr*) &claddr, &len);

        if(numBytes == -1)
            errExit("recvfrom");

        if(inet_ntop(AF_INET6, &claddr.sin6_addr, claddrStr, INET6_ADDRSTRLEN) == NULL)
            printf("Couldnt convert client address to string!\n");
        else
            printf("Server received %ld bytes from (%s, %u)\n", (long) numBytes, claddrStr, ntohs(claddr.sin6_port));
       
        for(j = 0; j < numBytes; j++)
            buf[j] = toupper((unsigned char) buf[j]);

        if(sendto(sfd, buf, numBytes, 0, (struct sockaddr*) &claddr, len) == -1)
            fatal("sendto");
        

    }

}