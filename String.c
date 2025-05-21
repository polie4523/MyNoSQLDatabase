#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "String.h"

int _SET(DATABASE *const database, const char *const key, const char *const value) {
    if (!key || !value) return 1;
    if (strlen(key)==0 || strlen(value)==0) return 1;
    char *str = (char *) malloc((strlen(value)+1)*sizeof(char));
    if (!str) exit(1);
    memcpy(str, value, strlen(value)+1);
    
    DATA *newnode = SearchKey(database, key, TYPE_STRING);
    if (!newnode) {
        newnode = NewData(key);
        if (database->length[TYPE_STRING]==0) {
            database->datalist_head[TYPE_STRING] = newnode;
            database->datalist_tail[TYPE_STRING] = newnode;
            database->length[TYPE_STRING] = 1;
        } else {
            newnode->next = database->datalist_head[TYPE_STRING];
            database->datalist_head[TYPE_STRING] = newnode;
            (database->length[TYPE_STRING])++;
        }
    }
    else {
        free(newnode->value);
    }
    newnode->value = (void *) str;
    return 0;
}

char *_GET(const DATABASE *const database, const char *const key) {
    DATA *tmp = SearchKey(database, key, TYPE_STRING);
    if (!tmp) return NULL;
    char *str = (char *) tmp->value;
    printf("%s\n", str);
    return str;
}

size_t _SRANGE(const DATABASE *const database, long start, long end) {
    size_t len = database->length[TYPE_STRING];
    if (start < 0) start = len+start;
    if (start < 0) start = 0;
    if (end < 0) end = len+end;
    if ((len<=start) || (end<start)) {printf("Empty array\n");return 0;}
    if (end >= len) end = len-1;
    
    DATA *start_ptr=database->datalist_head[TYPE_STRING];
    for (size_t i = 0; i < start; i++) start_ptr = start_ptr->next;

    printf("Index\tKey\tValue\n");
    printf("---------------------------\n");
    size_t num = start;
    while (num<=end) {
        char *str = (char *) start_ptr->value;
        printf("%ld\t%s\t%s\n", num, start_ptr->key, str);
        start_ptr = start_ptr->next;
        num++;
    }
    printf("---------------------------\n");
    return num;
}