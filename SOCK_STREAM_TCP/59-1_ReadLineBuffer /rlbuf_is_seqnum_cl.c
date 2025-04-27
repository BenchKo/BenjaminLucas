#include <netdb.h>
#include "is_seqnum.h"


int main(int argc, char* argv[]){

    char *reqLenStr;                /* requsted length of sequence*/
    char seqNumStr[INT_LEN];        /* start of granted sequence*/
    int cfd;
    ssize_t numRead;
    struct addrinfo hints;
    struct addrinfo *rp, *result;
    struct ReadLineBuffer* rlbuf;
    char extBuffer[MAX_EXT_BUF];
    if(argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s server-host [sequence-len]\n", argv[0]);
    
    /* call to getaddrinfo() to obtain a list of addresses that we can try to connect to*/

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_addr = NULL;
    hints.ai_canonname = NULL;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;
    hints.ai_next = NULL;


    if(getaddrinfo(argv[1], PORT_NUM, &hints, &result) != 0)
        errExit("getaddrinfo");
   
        
    /* Walk through returned list until we find an adress structure that can be used 
    to successfully connect a socket  */
    for(rp = result; rp != NULL; rp = rp->ai_next){
        cfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(cfd == -1)
            continue;
        
        if(connect(cfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break; /* success*/
        /* if no break connection failed for this socket*/
        close(cfd);
    }


    if(rp == NULL)
        fatal("Could not connect socket to ANY address!\n");
    
    
        /* send requested sequence length with terminating newline */
    reqLenStr = (argc > 2) ? argv[2] : "1";

    if(write(cfd, reqLenStr, strlen(reqLenStr)) != strlen(reqLenStr))
        fatal("partial/failed write (newline)");
    
    
    if(write(cfd, "\n", 1 ) != 1)
        fatal("Partial/failes write (newline)");
    
    
        /* read and display sequence number returned bei Server*/
    readLineBufferInit(cfd, rlbuf);
    
    numRead = readLineBuffer(rlbuf, extBuffer, MAX_EXT_BUF);
    printf("numread == %d\n", numRead);
    
    if(numRead == 0)
        fatal("unexpected error");
    
    else if(numRead == -1)
        errExit("readline");
    
    
    printf("sequence number: %s\n", extBuffer);
    exit(EXIT_SUCCESS);


}   