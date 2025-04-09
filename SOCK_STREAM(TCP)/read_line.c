#include <unistd.h>
#include <errno.h>
#include "read_line.h"


/* read from fd with system call read and return the number Bytes read*/

ssize_t readLine(int fd, void* buffer, size_t n) {
    
    ssize_t numRead;
    size_t totRead;
    char* buf;
    char ch;

    if(n <= 0 || buffer == NULL){
        errno = EINVAL;
        return -1;
    }
    buf = buffer;                       /*No pointer arithmetic on "void*" */

    totRead = 0;

    while(1){
        numRead = read(fd, &ch, 1);

        if(numRead == -1){
            if(errno == EINTR)          /* Interrupted --< restart read()*/
                continue;
           else
                return -1;
        
        }else if(numRead == 0){       /* EOF*/
            if(totRead == 0)            /* No Bytes read*/
                return 0;
            else                        /* some bytes read add \0 */
                break;

        }else{                          /* numRead must be 1 to get here*/
        if(totRead < (n - 1)){       /*  Discard > (n - 1) bytes*/
                totRead++;
                *buf++ = ch;
            }
        
        if(ch == '\n')
            break;
        }
    }
    *buf = '\0';
    return totRead;    
            
}