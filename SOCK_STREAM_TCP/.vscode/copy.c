void displayUserContent(struct Request* req, struct NetworkContext* netCtx, int pin){
    char displayUserBuf[MAXDISPLAYBUFFER];
    char tempFmtBuf[64];

    snprintf(displayUserBuf,strlen("\n\nstart\n") + 1 ,"\n\nstart\n"); 
    
    for(int i = 0; i < TABSIZE; i++){
        
    if(hashtab[i] == NULL || hashtab[i]->key == NULL || req->key == NULL || pin == -1){
        //indexe
        snprintf(tempFmtBuf, sizeof(int) +1, "%2i", i);
        strncat(displayUserBuf, tempFmtBuf, strlen(tempFmtBuf) + 1);

        for(int j = 0; j < 32; j++){
            strncat(displayUserBuf, "-", strlen("-"));
        }
        //Newline
        strncat(displayUserBuf,"\n", strlen("\n"));

    }else{

        snprintf(tempFmtBuf, strlen(hashtab[i]->key) + strlen(hashtab[i]->value) + 2,"%2i %s - %-10s - %-20s", i,
          hashtab[i]->key, hashtab[i]->value);
        strncat(displayUserBuf, tempFmtBuf, strlen(tempFmtBuf) + 1);

        if(hashtab[i]->next != NULL){
            snprintf(tempFmtBuf, strlen(hashtab[i]->next->value) + 1, "%s\n", hashtab[i]->next->value);
            strncat(displayUserBuf, tempFmtBuf, strlen(tempFmtBuf) + 1 );
            
        }else{
            strncat(displayUserBuf, "\n", strlen("\n") + 1);
        }
    }
    }
    strncat(displayUserBuf, "end\n\n", strlen("end\n\n"));
    write(netCtx->cfd, displayUserBuf, sizeof(displayUserBuf));
}

