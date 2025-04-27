#include <netdb.h>
#include "is_seqnum.h"


int main(int argc, char* argv[]){

    char *reqLenStr;                /* requsted length of sequence*/
    char seqNumStr[INT_LEN];        /* start of granted sequence*/
    int cfd;
    ssize_t numRead;
    struct addrinfo hints;
    struct addrinfo *rp, *result;


    if(argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s server-host [sequence-len]\n", argv[0]);
    
    cfd = inetConnect(argv[1], PORT_NUM, SOCK_STREAM);
    if(cfd == -1){
        perror("inetConnect");
        return -1;
    }

    /* send requested sequence length with terminating newline */

    reqLenStr = (argc > 2) ? argv[2] : "1";

    if(write(cfd, reqLenStr, strlen(reqLenStr)) != strlen(reqLenStr))
        fatal("partial/failed write (newline)");
    if(write(cfd, "\n", 1 ) != 1)
        fatal("Partial/failes write (newline)");

    /* read and display sequence number returned bei Server*/
    numRead = readLine(cfd, seqNumStr, INT_LEN);

    if(numRead == 0)
        fatal("unexpected error");
    else if(numRead == -1)
        errExit("readline");
    
    printf("sequence number: %s\n", seqNumStr);
    exit(EXIT_SUCCESS);


}   