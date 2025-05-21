#include <string.h>
#include <stdio.h>
#include "MyNoSQL.h"
#include "String.h"

int SET(DATABASE *const database, const char *const key, const char *const value)
{
    DATA *newnode = SearchKey(database, key, TYPE_STRING);
    if (!newnode) newnode = NewData(key);
    char *str = (char *) malloc((strlen(value)+1)*sizeof(char));
    if (!str) exit(1);
    memcpy(str, value, strlen(value)+1);
    newnode->value = (void *) str;

    if (database->length[TYPE_STRING]==0)
    {
        database->datalist_head[TYPE_STRING] = newnode;
        database->datalist_tail[TYPE_STRING] = newnode;
        database->length[TYPE_STRING] = 1;
    }
    else
    {
        newnode->next = database->datalist_head[TYPE_STRING];
        database->datalist_head[TYPE_STRING] = newnode;
        (database->length[TYPE_STRING])++;
    }
    return 0;
}

char *GET(const DATABASE *const database, const char *const key)
{
    DATA *tmp = SearchKey(database, key, TYPE_STRING);
    if (!tmp) return NULL;
    char *str = (char *) tmp->value;
    printf("%s\n", str);
    return str;
}

// int DeleteString(const DATABASE *const database, const char *key)
// {
//     S_DATA_NODE *current=list->Head, *previous=NULL;
//     while (current!=NULL)
//     {
//         if (strcmp(current->Key, key)==0) break;
//         previous = current;
//         current = current->next;
//     }
//     if (current==NULL) return 1;
//     puts(current->Value);
//     previous->next = current->next;
//     free(current->Key);
//     free(current->Value);
//     free(current);
//     list->Len--;
//     return 0;
// }

size_t SRANGE(const DATABASE *const database, long start, long end)
{
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
    while (num<=end)
    {
        char *str = (char *) start_ptr->value;
        printf("%ld\t%s\t%s\n", num, start_ptr->key, str);
        start_ptr = start_ptr->next;
        num++;
    }
    printf("---------------------------\n");
    return num;
}