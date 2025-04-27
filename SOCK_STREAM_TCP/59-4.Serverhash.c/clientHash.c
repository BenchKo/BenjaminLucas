#include "clientHash.h"
#include "hash.h"
/*************************************************************************************** 
 *    Client functions. maybe better to modulize them into different header. just maybe*/

 void displayOpts(char* argv, const char* ipStr){

    printf("Connection to %s established!\n", argv);
    printf("Connected to: %s\n", ipStr);
    printf("\n\n))))) HASHTABLE (((((\n");
    printf("\n(1)add\n(3)delete\n(5)modify\n(7)Show my content\n");
    printf("Enter request Type:\n");
}


int pin_checking(struct Request* req, struct ClientNetworkContext* clNetCtx, struct Response* res){
    char pinStateStr[2]; // Empfängt und speichert Speichert Status von Server ob Pin korrekt war
    
    
    printf("Pin eingeben\n"); 
    for(int i = 0; i < PIN_TRY; i++){
        printf(">");
        
        fgets(req->pin, sizeof(req->pin), stdin);
        //Skip newline char lines
        if(strlen(req->pin) < 2){
            --i;
            continue;
        }

        cutNewline(req->pin);

        // Schicke Pin an Server
        if(write(clNetCtx->socketFD, req->pin, sizeof(req->pin)) == -1){
            perror("write: pin_checking");
        }

        // Nimm response entgegen: Ob Pin Korrekt war
        if(read(clNetCtx->socketFD, pinStateStr, sizeof(pinStateStr)) == -1){
            perror("read: pin_chcking");
        }

        // Schlank aber nicht unbd. best practise atoi in der if
        if(atoi(pinStateStr) == 1){
            printf("Pin Korrekt!\n");
            return 0;
        
        // Wenn Pin nach 3 Versuchen falsch war: Exit   
        }else if(atoi(pinStateStr) == 0 && i > PIN_TRY){

            printf("\nKeine Versuche mehr. Programm wird beendet!\n");
            exit(EXIT_FAILURE);
        }
    }
}