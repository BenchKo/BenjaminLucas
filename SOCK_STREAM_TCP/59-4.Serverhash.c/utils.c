#include "utils.h"
#include <string.h>


void cutNewline(char* buffer){
    
    buffer[strcspn(buffer, "\n")] = '\0';

}
