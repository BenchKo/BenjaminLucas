#ifndef READ_LINE_H
#define READ_LINE_H
#include <string.h> /* memset*/
#include <unistd.h>
#include <stdbool.h>
#define MAX_BUF 8
#define MAX_EXT_BUF 64
/* read_line_buf.h

    Header file for read_line_buf.c (implementation of readLineBuf()).
*/

struct ReadLineBuffer{

    char buffer[MAX_BUF];   /* current buffer from file*/
    int nextChar;           /* index of next unread character in buf */
    int fdcopy;             /* file descriptor from which to read */
    ssize_t len;            /* Number of characters in buffer*/
};



void readLineBufferInit(int fd, struct ReadLineBuffer*);

ssize_t readLineBuffer(struct ReadLineBuffer*, char* buffer, size_t n);

#endif