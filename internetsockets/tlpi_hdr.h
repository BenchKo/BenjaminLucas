#ifndef TLPI_HDR_H
#define TLPI_HDR_H

#include <sys/types.h>      /* used for type definktions often in many programms*/
#include <stdio.h>           /* standard i/o funktciond*/
#include <stdlib.h> 
#include <unistd.h>         /* prototypes for many system calls*/
#include <errno.h>          /* Declares errno and defines error constants*/
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

typedef enum { FALSE, TRUE } Boolean;


#endif
