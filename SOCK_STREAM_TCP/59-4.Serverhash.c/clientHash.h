#pragma once
#include "hash.h"
//client end


struct ClientNetworkContext{
    int socketFD;
    socklen_t addrlen;
    char ipStr[INET6_ADDRSTRLEN];
    struct addrinfo svaddr;
    struct addrinfo* result;
};


int pin_checking(struct Request* req, struct ClientNetworkContext* clNetCtx, struct Response* res);