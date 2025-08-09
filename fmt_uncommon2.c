#include <stdio.h>
#include <stdlib.h>


int main(){
    int A = 5;
    int B = 7;
    int count_one, count_two;

    // example of a %n format string
    printf("The number of bytes written up to this point X%n s being stored in count_one and the"
            "number of bytes up to here X%n is being stored in count_two.\n", &count_one, &count_two);

    printf("count_one:%d\n",count_one);
    printf("count_two:%d\n",count_two);
    
    // what if we get rid of the last argument after the coma?
    //stack example                                              this one
    printf("A is %d and is at %08x. B is %p \n",A, (unsigned int) &A);

    exit(0);
}
