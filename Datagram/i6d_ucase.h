#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctype.h>
#include "tlpi_hdr.h"

#define BUFSIZE 10                  /* Maximum size of messages exchanged between server and client*/

#define PORT_NUM 50002              /* Server Portnummer */
