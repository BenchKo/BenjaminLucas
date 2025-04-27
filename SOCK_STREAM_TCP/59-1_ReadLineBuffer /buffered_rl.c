#include <unistd.h>
#include <errno.h>
#include "buffered_rl.h"

/* Initialisiere eine ReadLineBuffer Struktur 
Verwaltungsstruktur für den ReadLineBuffer*/
void readLineBufferInit(int fd, struct ReadLineBuffer* rlbuf){

    memset(rlbuf->buffer, 0, MAX_BUF); 
    rlbuf->fdcopy = fd;
    rlbuf->nextChar = 0;
    rlbuf->len = 0;
}


/* Return a line of input from the intern buffer 'rlbuf->buffer', placing the characters in
   the extern buffer 'buffer'. The 'n' argument specifies the size of 'buffer'. If the line of
   input is larger than this, then the excess characters are discarded. */
ssize_t readLineBuffer(struct ReadLineBuffer* rlbuf, char* buffer, size_t n){

    size_t cnt;
    char c;

    if(n <= 0 || buffer == NULL){
        errno = EINVAL;             /* ungültiges Argument */
        return -1;
    }
    cnt = 0;

    /*Holt Zeichen aus rlbuf->buf, bis zur nächsten neuen Zeile.*/

    for(;;){

        /* Wenn nicht genügend Zeichen in „rlbuf“ vorhanden sind, nimm
        weiteren Input aus dem zugehörigen Dateideskriptor. */
        
        if(rlbuf->nextChar >= rlbuf->len){
            rlbuf->len = read(rlbuf->fdcopy, rlbuf->buffer, MAX_BUF);
            if(rlbuf->len == -1)
                return -1;
            
            if(rlbuf->len == 0)
                break;  /* end of file*/
            
            rlbuf->nextChar = 0;
        }

        c = rlbuf->buffer[rlbuf->nextChar];
        rlbuf->nextChar++;
        
        if(cnt < n )
            buffer[cnt++] = c;

        if(c == '\n')
            break;
    
    }
    buffer[cnt] = '\0';
    return cnt;
}
