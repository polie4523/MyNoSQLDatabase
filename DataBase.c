#include <stdlib.h>
#include <string.h>
#include "DataBase.h"

DATA *NewData(const char *const key) {
    DATA *newdatanode = (DATA *) malloc(sizeof(DATA));
    newdatanode->key = (char *) malloc((strlen(key)+1)*sizeof(char));
    memcpy(newdatanode->key, key, strlen(key)+1);
    newdatanode->value = NULL;
    newdatanode->next = NULL;
    return newdatanode;
}

DATA *SearchKey(const DATABASE *const database, const char *const key, DATA_TYPE datatype) {
    DATA *tmp = database->datalist_head[datatype];
    while (tmp) {
        if (strcmp(tmp->key, key)==0) return tmp;
        tmp = tmp->next;
    }
    return NULL;
}