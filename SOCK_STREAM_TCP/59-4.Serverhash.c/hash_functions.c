
#include "hash.h" // Header Vater
#include "hash_functions.h"

extern struct nlist* hashtab[TABSIZE]; // Global hash table

unsigned djb2_hash(char* in_str){
    unsigned hashval;

    if(in_str == NULL || *in_str == '\0')
        return 0;

    for(hashval = 5381; *in_str; in_str++){
        hashval = ((hashval << 5) + hashval) + (unsigned int) *in_str;
    }
    return hashval % TABSIZE;
}

struct nlist* lookup(char* str){
    struct nlist* np;

    for(np = hashtab[djb2_hash(str)]; np != NULL; np = np->next){

        if(strcmp(np->key, str) == 0)
            return np;
    }
    return NULL; /* not found*/
    
}

struct nlist* install_hashval(char* name, char* value){
    struct nlist* new_entry;
    struct nlist* exist_entry;
    unsigned hashval;
    

    if((exist_entry = lookup(name)) == NULL){ /* Key noch nicht auf dem Tisch */

        if((new_entry = (struct nlist*) malloc(sizeof(*new_entry))) == NULL){
            perror("malloc");
            return NULL;
        }
        
        if(new_entry == NULL || (new_entry->key = strdup(name)) == NULL){
            perror("malloc/ strdup");
            return NULL;
        }
        hashval = djb2_hash(name);
        new_entry->next = hashtab[hashval]; /* Beim ersten Eintrag zeiigt np->next auf Null was das Ende der Liste symbolisiert*/
        hashtab[hashval] = new_entry;

    }else{ // Key bereits auf dem Tisch. Add neue defn in Liste 

        new_entry = malloc(sizeof(*new_entry));

        if((new_entry->key = strdup(name)) == NULL){
            perror("strdup failed peinlich hart!\n");
            return NULL;
        }
        // verknüpfe newschool knoten an bestehenden oldskool node
        new_entry->next = exist_entry;

        //Kopf aufsetzen
        hashtab[djb2_hash(name)] = new_entry;
        
    }

    if((new_entry->value = strdup(value)) == NULL)
        return NULL;
    return new_entry;

    
}

void DisplayTable(struct NetworkContext* netCtx){
   
    printf("\n\nstart\n"); 
    for(int i = 0; i < TABSIZE; i++){
        
        if(hashtab[i] == NULL || hashtab[i]->key == NULL){
            printf("%2i", i);

            for(int j = 0; j < 32; j++){
            printf("-");
            }
            
            printf("\n");

        }else{
            printf("%2i %s - %-10s - %-20s", i,  hashtab[i]->key, hashtab[i]->value, netCtx->clAddrStr);
            
            if(hashtab[i]->next != NULL){
                printf("&& %s\n", hashtab[i]->next->value);

            }else{
                printf("\n");
            }
        }
    }
    printf("end\n\n");
}

int delete_entry(char* key){

    struct nlist* hp;

    if((hp = lookup(key)) == NULL){
        fprintf(stderr, "key '%s' cant be found\n", key);
        return -1;
    }
    if(strcmp(hp->key, key) == 0){
        hashtab[djb2_hash(key)] = NULL;
        //Null gesamte Struktur *hp Nicht nur n menge bytes größe pointer
        memset(hp, '\0', sizeof(*hp));
    } 

    return 0;
}
struct nlist* modify_value(char* key, char* new_value){

    struct nlist* lookup_value;

    if((lookup_value = lookup(key)) == NULL){
        perror("lookup");
        return NULL;
    }
    memset(lookup_value->value, '\0', sizeof(lookup_value->value));
    snprintf(lookup_value->value, strlen(new_value)+1, new_value);

}

int generate_pin(char* pinBuf){
    int pin;
    srand(time(NULL));

    pin = rand() % PIN_MAX;
    snprintf(pinBuf, 5, "%d", pin);
    if(pin > 0 && pin < 9999){
        return 0;
    }else{
        return -1;
    }
    
}



