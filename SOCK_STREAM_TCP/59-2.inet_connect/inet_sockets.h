#pragma once

#include <sys/socket.h>
#include <netdb.h>
#include "tlpi_hdr.h"
int inetConnect(const char* host, const char* service, int type);

int inetListen(const char* service, int backlog, socklen_t* addrlen);

int inetBind(const char* service, int type, socklen_t* addrlen);

char* inetAddressStr(const struct sockaddr* addr, socklen_t addrlen, char* addrStr, int addrStrlen);
static int inetPassiveSocket(const char* service, int type, socklen_t* addrlen, Boolean doListen, int backlog);

#define IS_ADDR_STR_LEN 4096

/* empfohlene Länge für String Puffer
    dessen Puffer der caller an inetAddressStr() geben soll.
    Muss größer sein als (NI_MAXHOST + NI_MAX_SERV + 4 )*/
