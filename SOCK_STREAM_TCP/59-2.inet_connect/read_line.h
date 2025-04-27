#include <unistd.h>
#include <stdbool.h>
struct book_keeping{
    int bufsize;
    char* buffer;
    char* nextChar;
    int fdcopy;
};
ssize_t readLine(int fd, void* buffer, size_t n);
void readLineBufInit(int fd, struct book_keeping** rlbuffer);
int readLineBuffer(int fd, struct book_keeping* rlbuf);