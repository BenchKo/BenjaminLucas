#include <signal.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "read_line.h"
#include "tlpi_hdr.h"
#include "error_functions.h"
#include "inet_sockets.h"

#define PORT_NUM                "50000"

#define INT_LEN                 30  /* size of string able to hold largest Integer*/

