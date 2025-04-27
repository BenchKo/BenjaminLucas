#include "clientHash.h"
#include "hash.h"
#include "utils.h"
#include <string.h>     // strcspn





int main(int argc, char* argv[]){
    //Network structs 
    struct ClientNetworkContext clNetCtx;
    struct Request req;
    struct Response res;
    
    int request;
    char takeTableBuf[MAXDISPLAYBUFFER];
    if(argc < 2){
        usageErr("%s: mindestens HostName bzw. Adresse!\n", argv[0]);
        return -1;
    }
    
    clNetCtx.socketFD = inetConnect(argv[1], PORT_NUM, SOCK_STREAM);
    if(clNetCtx.socketFD == -1){
        perror("inetConnect777");
        return -1;
    }
    //initisalisieren der addrinfo struktur 
    memset(&clNetCtx.svaddr, 0, sizeof(struct addrinfo));
    clNetCtx.svaddr.ai_family = AF_INET;
    clNetCtx.svaddr.ai_socktype = SOCK_STREAM;

    //initialisiern der Response Struktur
    memset(&res, 0, sizeof(struct Response));

    //initialisieren der Request Struktur
    memset(&req, 0, sizeof(struct Request));

    //translate host name eg localhost to binary IP and with ntop to doddet 127.0.0.0.1 
    if (getaddrinfo(argv[1], NULL, &clNetCtx.svaddr, &clNetCtx.result) != 0){
        perror("getaddrinfo444");
        return -1;
    }

    // convert IP Adress to redable string; save in .ipStr. Er soll kein NULL wiedergeben
    if(inet_ntop(AF_INET, &(((struct sockaddr_in* )clNetCtx.result->ai_addr)->sin_addr), clNetCtx.ipStr, INET6_ADDRSTRLEN) == NULL){
        perror("inet_ntop");
        return -1;
    }
    freeaddrinfo(clNetCtx.result);

    //user interface und aufnahme des requestTypes: 1 = ADD, 2 = Delete etcc.
    displayOpts(argv[1], clNetCtx.ipStr);
    // get requestType from user
    fgets(req.requestType, sizeof(struct Request), stdin);
    // Entferne Newline char 
    req.requestType[2] = '\0';

    write(clNetCtx.socketFD, req.requestType, sizeof(req.requestType)); // (1) write

    request = atoi(req.requestType);
    switch(request){

        case 1:     
            printf("Add a keyword:\n>");
            fgets(req.key, sizeof(req.key), stdin);
            cutNewline(req.key);

            printf("Add the value for that key now:\n>");
            fgets(req.value, sizeof(req.value), stdin);
            cutNewline(req.value);

            if(write(clNetCtx.socketFD, &req, sizeof(struct Request)) <= 0){    // (2) write
                perror("write");
                exit(EXIT_FAILURE);
            }
            //Pin message
            read(clNetCtx.socketFD, res.resPin, sizeof(res.resPin));
            
            printf("You have logged in '%s' as a key for '%s'\n", req.key, req.value);
            printf("%s", res.resPin);
            break;
        
        case 3:
            printf("Type the KEY for your Matrix to be deleted:\n>");
            fgets(req.key, sizeof(req.key), stdin);
            cutNewline(req.key);

            
            if(pin_checking(&req, &clNetCtx, &res) != 0){
                printf("Authentizifikation fehlgeschlagen\n");
                return -1;
            }
            
            sleep(1);
            printf("Key wurde erfolgreich gelöscht!\n");
            break;
        
        
        case 5:
            
            printf("Lets modify ""villian laugh""\n");
            printf("Type the key of the value you want to change:\n>");
            fgets(req.key, sizeof(req.key), stdin);
            cutNewline(req.key);

            if(pin_checking(&req, &clNetCtx, &res) != 0){
                printf("Authentizifikation fehlgeschlagen\n");
                return -1;
            }
            sleep(1);
            printf("To value..\n>");
            // Bei weiteren SItuationen wie diesen den COde abschnitt auslagern für die Lesbarkeit
            for(;;){
                fgets(req.value, sizeof(req.value), stdin);
                cutNewline(req.value);
                if(strlen(req.value) > 1){
                    break;
                }
            }

            write(clNetCtx.socketFD, req.value, strlen(req.value) + 1);

            printf("Your new value after %s is '%s'\n", req.key, req.value);
            break;

        case 7:
            

            printf("Type in the Key you want to display\n>");
            fgets(req.key, sizeof(req.key), stdin);
            cutNewline(req.key);

            // send req.key an Sv
            write(clNetCtx.socketFD, req.key, strlen(req.key) + 1);

            if(pin_checking(&req, &clNetCtx, &res) != 0){
                printf("Authentizifikation fehlgeschlagen\n");
                return -1;
            }
            printf("Pin Korrekt!\n");
            sleep(1);
            
            // Read Table interface 
            read(clNetCtx.socketFD, takeTableBuf ,sizeof(takeTableBuf));

            printf("%s\n", takeTableBuf);

            break;

        default:
            printf("Wrong input\n");
            break;
    
    }

    //Übergabe passiert hier im write() sys call
    if(write(clNetCtx.socketFD, &req, sizeof(struct Request)) <= 0) {        
        perror("write");
        return -1;
    }

    if (read(clNetCtx.socketFD, &res, sizeof(struct Response)) == -1){
        perror("read");
        return -1;
    }

    // Auswertung des Responses vom Server: seqNum, status, message, pin...
    printf("seqnum-> %d\n", res.seqNum);

    close(clNetCtx.socketFD);
    printf("\nDisconnected.\n");



}

