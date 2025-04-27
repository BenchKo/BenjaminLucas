#include "hash.h"
#include "hash_functions.h"
#include <netinet/tcp.h>    /* TCP_NODELAY Konstante*/

struct nlist* hashtab[TABSIZE];

int main(int argc, char* argv[]) {


    //netzwerk Structs
    struct NetworkContext netCtx;
    struct Request req;
    struct Response res;

    int reqChoice;
    int cond;
    int pinState;
    /* Verhindert dass der Server durch ein Signal des clients geschlossen wird.*/
    if(signal(SIGPIPE, SIG_IGN) == SIG_ERR){
        errExit("signal");
    }

    netCtx.lfd = inetListen(PORT_NUM, 0, &netCtx.addrlen);
    if(netCtx.lfd == -1){
        perror("inetListen");
        return -1;
    }

    // initialisierung der Response struct
    memset(&res, 0 , sizeof(struct Response));
    strncpy(res.message, "Eingabe nicht bekannt!", 23);

    // init Request struct
    memset(&req, 0, sizeof(struct Request));

    printf("Server wartet auf eingehende Verbindungen...\n");
    
    for(;;){

        netCtx.cfd = accept(netCtx.lfd, (struct sockaddr*) &netCtx.claddr, &netCtx.addrlen);
        if(netCtx.cfd == -1){
            perror("accept");
            continue;
        }

        if((inetAddressStr((struct sockaddr*) &netCtx.claddr, netCtx.addrlen, netCtx.clAddrStr, MAXCLADDRSTR)) == NULL)
            perror("inetAddrStr");

        
        printf("Connection from %s\n", netCtx.clAddrStr);

        //Lesen aus connect Socket mit
        if (read (netCtx.cfd, req.requestType, sizeof(req.requestType)) <= 0){      //(1) read   
            perror("read");
            return -1;
        }
        //Pin wird in request type gespeichert!!! 
        printf("received: %s",req.requestType);
        printf("received key:%s val:%s\n", req.key, req.value);
        // transform string in integar
        reqChoice = atoi(req.requestType);

        switch(reqChoice){

            case 1:
                // EMpfang der key und value
                if(read(netCtx.cfd, &req, sizeof(struct Request)) == -1){
                    perror("read");
                    exit(EXIT_FAILURE);
                }
                if(addDataWithPin(&req, &res, &netCtx) == -1){
                    perror("addDataWithPin");
                    break;
                }

                DisplayTable(&netCtx);

                break;
            
            case 3:
                // Check ob Pin korrekt ist und teile es dem clienten Synchron mit.
                if(server_security(&netCtx, &res, &req) == -1){
                    printf("Pin zu oft Falsch eingegeben!\n");
                    break;
                }
        
                //delete hash entry. 
                if((res.status = delete_entry(req.key)) != 0){
                    perror("delete_entry");
                    return -1;
                }
                
                printf("Entry successfully deleted!\n");
                DisplayTable(&netCtx);

                break; 
            
            case 5:
                if(server_security(&netCtx, &res, &req) == -1){
                    printf("Pin zu oft Falsch eingegeben!\n");
                    break;
                }
                // Empfange neuen Value...
                read(netCtx.cfd, req.value, sizeof(req.value));

                if(modify_value(req.key, req.value) == NULL){
                    perror("modify_value");
                    res.status = -1;
                    return -1;
                }
                DisplayTable(&netCtx);
                break;

            case 7:
                
                // aktuell breaken wir loop nur bei server security -1
                while(1){

                    // read key für server_security und displayUserContent
                    read(netCtx.cfd, req.key, sizeof(req.key));
                    pinState = 1;
                    if((pinState = server_security(&netCtx, &res, &req)) == -1){
                        printf("\n\n\nPin zu oft Falsch eingegeben!\n");
                        break;
                    }
                    if(pinState == 0){
                        break;
                    }
                }
                if(pinState != 0){
                    break;
                }
                // show user content from table if
                displayUserContent(&req, &netCtx ,pinState); // write syscall Package 
                
            default:
                write(netCtx.cfd, res.message, 23);
                break;

        }   
        res.seqNum++;
        //Übermittel Statusbericht 
        write(netCtx.cfd, &res, sizeof(struct Response));
        printf("\n\n\nseqnum-> '%d'\n", res.seqNum); 



   }
}
