#include "hash.h"
#include "hash_functions.h"
#include "clientHash.h"     // Für client network struct
#include <string.h>
/*                      *Funktionen für den HashServer* 



*/
extern struct nlist* hashtab[TABSIZE];

// Verändert bei Korrekter PinEingabe res.pin_state auf 1
int server_security(struct NetworkContext* netCtx, struct Response* res, struct Request* req){
    int cond;           // condition
    res->pin_state = 0;// Init pin auf state 'falsch'
    char pin_State_String[2];

    for(int i = 1; i <= PIN_TRY; i++){
        
        
        // Lies Pineingabe von Client 
        if(read(netCtx->cfd, req->pin, sizeof(req->pin)) == -1){
            perror("read req->pin ss\n");
            return -1;
        }

        if((cond = strncmp(hashtab[djb2_hash(req->key)]->pin, req->pin, 4)) == 0){

            res->pin_state = 1; 
            printf("Eingabe Korrekt:'%s'\n", req->pin);

            snprintf(pin_State_String, sizeof(pin_State_String), "%i", res->pin_state);
            //Pin Korrekt. Client muss informiert werden!
            if(write(netCtx->cfd, pin_State_String, sizeof(pin_State_String)) == -1){
                perror("write case3");
                return -1;
            }
            //Alles Korrekt abgelaufen.
            return 0; //Korrekte Eingabe Sprung Zurück in Aufruf.
        }
        // Teile Client mit, dass der Pin Falsch war.
        snprintf(pin_State_String, sizeof(pin_State_String), "%d", res->pin_state);
        write(netCtx->cfd, pin_State_String, sizeof(pin_State_String));


        if (i < PIN_TRY && cond != 0) {
            printf("%d. Falsche Eingabe:'%s'\n", i, req->pin);

        }else{
            printf("%i. Falsche Eingabe:'%s'", i, req->pin);
            printf("-> Pin 3x Falsch.\n");
            res->pin_state = 0;
            return -1;
        }
    }
}




int addDataWithPin(struct Request* req, struct Response* res, struct NetworkContext* netCtx){
    
    install_hashval(req->key, req->value);
    printf("'%s' wurde als Key für '%s' von %s eingereicht.\n", req->key, req->value, netCtx->clAddrStr);
    
    // Generiere Zugangsschlüssel.
    if(generate_pin(hashtab[djb2_hash(req->key)]->pin) == -1){
        perror("pinBuf");
        return -1;
    }
    // Schicke Pin an Client für dessen Zugriff.
    snprintf(res->resPin, sizeof(res->resPin), "\nDein Pin für weiteren Zugriff:'%s'\n", hashtab[djb2_hash(req->key)]->pin);
    write(netCtx->cfd, res->resPin, strlen(res->resPin) + 1);
    
}

void displayUserContent(struct Request* req, struct NetworkContext* netCtx, int pin){
    char displayUserBuf[MAXDISPLAYBUFFER];
    char tempFmtBuf[64];

    snprintf(displayUserBuf,strlen("\n\nstart\n") + 1 ,"\n\nstart\n"); 
    
    for(int i = 0; i < TABSIZE; i++){
        
    if(hashtab[i] == NULL || hashtab[i]->key == NULL || req->key == NULL || pin == -1){
        //indexe
        snprintf(tempFmtBuf, sizeof(int) + 1, "%2i", i);
        strncat(displayUserBuf, tempFmtBuf, strlen(tempFmtBuf) + 1);

        for(int j = 0; j < 32; j++){
            strncat(displayUserBuf, "-", strlen("-") + 1);
        }
        //Newline
        strncat(displayUserBuf,"\n", strlen("\n") + 1);

    }else{
        int totalSize = snprintf(NULL, 0, "%2i %s - %10s", i, hashtab[i]->key, hashtab[i]->value) + 2;

        snprintf(tempFmtBuf, totalSize,"%2i %s - %10s", i,hashtab[i]->key, hashtab[i]->value);
        printf("dbug: val:'%s' totalsize = %d", hashtab[i]->value, totalSize);
        strncat(displayUserBuf, tempFmtBuf, strlen(tempFmtBuf) + 1);

        if(hashtab[i]->next != NULL){
            snprintf(tempFmtBuf, strlen(hashtab[i]->next->value) + 1, "%s\n", hashtab[i]->next->value);
            strncat(displayUserBuf, tempFmtBuf, strlen(tempFmtBuf) + 1 );
            
        }else{
            strncat(displayUserBuf, "\n", strlen("\n") + 1);
        }
    }
    }
    strncat(displayUserBuf, "end\n\n", strlen("end\n\n") +1);
    write(netCtx->cfd, displayUserBuf, sizeof(displayUserBuf));
}






