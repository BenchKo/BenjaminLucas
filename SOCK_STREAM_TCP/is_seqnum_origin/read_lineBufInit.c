#include <unistd.h>
#include <errno.h>
#include "read_line.h"
#include "error_functions.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Aufgabe: 59-1 
Entwickle Zwei Funktionen, eine die alles Vorbereitet mit Hilfe einer VerwaltungsdatenStruktur welche folgendes macht:
Funktion 1:
- Puffer initialisieren. 
- Größe des Puffers festlegen // int BufSize
- Pointer auf nächstes noch ungelesenes Zeichen bereitstellen // void* / char* nextchar 
- Kopie des fd bereitstellen 
Funktion 2: 
- Liest einen Satz aus Buffer und füllt diesen mit neuen Daten
- 
*/

void readLineBufInit(int fd, struct book_keeping** rlbuf){

    *rlbuf = malloc(sizeof(struct book_keeping));
    if(*rlbuf == NULL){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    printf("rlbuf address: %p\n", (void*)*rlbuf);
    (*rlbuf)->bufsize = 64;
    (*rlbuf)->buffer = (char*) malloc((*rlbuf)->bufsize + 1);
    if((*rlbuf)->buffer == NULL){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    (*rlbuf)->fdcopy = fd;
    (*rlbuf)->nextChar = (*rlbuf)->buffer;    /* start*/
    (*rlbuf)->buffer[0] = '\0';
    printf("Debug: rlbuf initialized successfully. Address: %p\n", (void*)*rlbuf);
    
}

int readLineBuffer(int fd ,struct book_keeping* rlbuf){
    int bytesRead = 0;

    if(rlbuf->buffer == NULL || rlbuf == NULL){
        fprintf(stderr, "buffer oder gesamte Struktur ist NULL");
        return -1;
    } 
    printf("Debug: Vor read().\n");
    fflush(stdout);
    bytesRead = read(fd, rlbuf->buffer, rlbuf->bufsize - 1);
    printf("Debug: Nach read(), bytesRead = %d.\n", bytesRead);
    fflush(stdout);
    if(bytesRead == -1){
        perror("Error during read!\n");
        return -1;
    }
    if(bytesRead == 0){
        printf("Detected EOF\n");
        return 0;
    }

    rlbuf->buffer[bytesRead] = '\0';
    rlbuf->nextChar = rlbuf->buffer;
    return bytesRead;

}
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