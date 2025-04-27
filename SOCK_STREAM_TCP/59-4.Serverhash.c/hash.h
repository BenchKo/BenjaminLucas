#pragma once

#include <signal.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "tlpi_hdr.h"
#include "error_functions.h"
#include "inet_sockets.h"
#include "utils.h"          // pinchecking braucht cutnewline
#include <arpa/inet.h>  /* inet_ntop()*/

#define PORT_NUM            "50000"
#define INT_LEN                 30  /* size of string able to hold largest Integer*/
#define TABSIZE                 16
#define MAX_VAL                128
#define MAX_KEY                 64
#define MAXCLADDRSTR            64
#define MAX_EXT_BUF             64
#define PINSIZE                  5
#define PIN_TRY                  3
#define MAXDISPLAYBUFFER      2048


struct Request{
    char requestType[4]; // Typ der Anfrage z.B. 1 = ADD, 2 = DELETE...
    char key[MAX_KEY]; 
    char value[MAX_VAL];
    char pin[PINSIZE];      //Pin Eingabe des Users
      
};
struct Response{
    int status;
    char message[128];
    int seqNum;
    char resPin[39];            // Zur rückgabe des Pins an den User
    int pin_state;
};
struct NetworkContext{
    int lfd;
    int cfd;
    int numRead;
    int seqNum;
    char seqNumStr[INT_LEN];

    socklen_t addrlen;
    char service[NI_MAXSERV];
    char host[NI_MAXHOST];
    char clAddrStr[MAXCLADDRSTR];
    struct sockaddr_storage claddr;
};


struct nlist{

    struct nlist* next;
    char* key;
    char* value;
    char pin[PINSIZE];
};

void displayOpts(char*, const char*);
// server end
int server_security(struct NetworkContext* netCtx, struct Response* res, struct Request* req);

int addDataWithPin(struct Request* req, struct Response* res, struct NetworkContext* netCtx);

void displayUserContent(struct Request* req, struct NetworkContext* netCtx, int pin);
