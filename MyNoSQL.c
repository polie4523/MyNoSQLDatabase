#include <string.h>
#include "MyNoSQL.h"


// extern void FreeList(DATA *const data);
// extern void FreeSortedSet(DATA *const data);

DATABASE *NewDatabase(void)
{
    DATABASE *new = (DATABASE *) malloc(sizeof(DATABASE));
    if (!new) exit(1);
    for (int i = 0; i < NUMOFTYPE; i++)
    {
        new->datalist_head[i] = NULL;
        new->datalist_tail[i] = NULL;
        new->length[i] = 0;
    }
    return new;
}

DATA *NewData(const char *const key)
{
    DATA *newdatanode = (DATA *) malloc(sizeof(DATA));
    newdatanode->key = (char *) malloc((strlen(key)+1)*sizeof(char));
    memcpy(newdatanode->key, key, strlen(key)+1);
    newdatanode->value = NULL;
    newdatanode->next = NULL;
    return newdatanode;
}

DATA *SearchKey(const DATABASE *const database, const char *const key, const int datatype)
{
    DATA *tmp = NULL;
    tmp = database->datalist_head[datatype];
    while (tmp)
    {
        if (strcmp(tmp->key, key)==0) return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

int DEL(DATABASE *const database, const char *const key, const int datatype)
{
    size_t key_len = strlen(key)+1;
    DATA *curr=NULL, *prev=NULL;
    curr = database->datalist_head[datatype];
    while (curr && strncmp(curr->key, key, key_len))
    {
        prev = curr;
        curr = curr->next;
    }
    if (!curr) return 1;
    if (curr == database->datalist_head[datatype])
    {
        database->datalist_head[datatype] = database->datalist_head[datatype]->next;
    }
    else
    {
        prev->next = curr->next;
    }
    FreeData(curr, datatype);
    free(curr);
    database->length[datatype]--;
    return 0;
}

void FreeData(DATA *const data, const int datatype)
{
    switch (datatype)
    {
    case TYPE_STRING:
        free(data->value);
        break;
    case TYPE_LIST:
        FreeList(data);
        break;
    case TYPE_SORTEDSET:
        FreeSortedSet(data);
        break;
    case TYPE_HASH:
        FreeHash(data);
        break;
    default:
        break;
    }
    free(data->key);
}

void FreeDatabase(DATABASE *const database)
{
    for (int i = 0; i < NUMOFTYPE; i++)
    {
        DATA *curr = database->datalist_head[i];
        DATA *prev = NULL;
        while (curr)
        {
            FreeData(curr, i);
            prev = curr;
            curr = curr->next;
            free(prev);
        }
    }
    free(database);
}
