#include "MyNoSQL.h"
#include "List.h"
#include <string.h>
#include <stdio.h>


DATA *NewList(DATABASE *const database, const char *const key)
{
    DATA *newnode = NewData(key);
    LIST *newlist = (LIST *) malloc(sizeof(LIST));
    if ((!newlist) || (!newnode)) exit(1);
    newlist->Len = 0;
    newlist->MostLeft = NULL;
    newlist->MostRight = NULL;
    newnode->value = (void *) newlist;
    if (database->length[TYPE_LIST]==0)
    {
        database->datalist_head[TYPE_LIST] = newnode;
        database->datalist_tail[TYPE_LIST] = newnode;
        database->length[TYPE_LIST] = 1;
    }
    else
    {
        newnode->next = database->datalist_head[TYPE_LIST];
        database->datalist_head[TYPE_LIST] = newnode;
        (database->length[TYPE_LIST])++;
    }
    return newnode;
}

void LPUSH(DATABASE *const database, const char *key, const char *value)
{
    BiNODE *newnode = (BiNODE *) malloc(sizeof(BiNODE));
    newnode->Value = (char *) malloc((strlen(value)+1)*sizeof(char));
    if ((!newnode->Value) || (!newnode)) exit(1);
    memcpy(newnode->Value, value, (strlen(value)+1));

    DATA *tmp = SearchKey(database, key, TYPE_LIST);
    if (!tmp) tmp = NewList(database, key);
    LIST *tmp_list = (LIST *) tmp->value;
    if (tmp_list->MostLeft==NULL)
    {
        newnode->left = NULL;
        newnode->right = NULL;
        tmp_list->MostLeft = newnode;
        tmp_list->MostRight = newnode;
        tmp_list->Len = 1;
    }
    else
    {
        newnode->left = NULL;
        newnode->right = tmp_list->MostLeft;
        tmp_list->MostLeft->left = newnode;
        tmp_list->MostLeft = tmp_list->MostLeft->left;
        tmp_list->Len++;
    }
}

void RPUSH(DATABASE *const database, const char *key, const char *value)
{
    BiNODE *newnode = (BiNODE *) malloc(sizeof(BiNODE));
    newnode->Value = (char *) malloc((strlen(value)+1)*sizeof(char));
    if ((!newnode->Value) || (!newnode)) exit(1);
    memcpy(newnode->Value, value, (strlen(value)+1));
    
    DATA *tmp = SearchKey(database, key, TYPE_LIST);
    if (!tmp) tmp = NewList(database, key);
    LIST *tmp_list = (LIST *) tmp->value;
    if (tmp_list->MostRight==NULL)
    {
        newnode->right = NULL;
        newnode->left = NULL;
        tmp_list->MostRight = newnode;
        tmp_list->MostLeft = newnode;
        tmp_list->Len = 1;
    }
    else
    {
        newnode->right = NULL;
        newnode->left = tmp_list->MostRight;
        tmp_list->MostRight->right = newnode;
        tmp_list->MostRight = tmp_list->MostRight->right;
        tmp_list->Len++;
    }
}

void LPOP(const DATABASE *const database, const char *const key)
{
    DATA *tmp = SearchKey(database, key, TYPE_LIST);
    if (!tmp) printf("No list is called by %s\n", key);
    else
    {
        LIST *list = (LIST *) tmp->value;
        if (list->MostLeft==NULL) printf("*List is empty!*");
        else
        {
            BiNODE *tmp = list->MostLeft;
            list->MostLeft = list->MostLeft->right;
            list->MostLeft->left = NULL;
            puts(tmp->Value);
            free(tmp->Value);
            free(tmp);
            list->Len--;
        }        
    }
}

void RPOP(const DATABASE *const database, const char *const key)
{
    DATA *tmp = SearchKey(database, key, TYPE_LIST);
    if (!tmp) printf("No list is called by %s\n", key);
    else
    {
        LIST *list = (LIST *) tmp->value;
        if (list->MostRight==NULL) printf("*List is empty!*");
        else
        {
            BiNODE *tmp = list->MostRight;
            list->MostRight = list->MostRight->left;
            list->MostRight->right = NULL;
            puts(tmp->Value);
            free(tmp->Value);
            free(tmp);
            list->Len--;
        }
    }
}

size_t LLen(const DATABASE *const database, const char *const key)
{
    DATA *tmp = SearchKey(database, key, TYPE_LIST);
    if (!tmp) return 0;
    LIST *list = (LIST *) tmp->value;
    return list->Len;
}

// int DeleteList(DATABASE *const database, const char *const key)
// {
//     LIST_NODE *current=listlist->Head, *previous=NULL;
//     while (current!=NULL)
//     {
//         if (strcmp(current->List->Key, key)==0) break;
//         previous = current;
//         current = current->next;
//     }
//     if (current==NULL) return 1;
//     if (current==listlist->Head) listlist->Head = listlist->Head->next;
//     else previous->next = current->next;
//     FreeList(current->List);
//     free(current);
//     listlist->Len--;
//     return 0;
// }

size_t LRANGE(const DATABASE *const database, const char *key, long start, long end)
{
    /*left(start)------->right(end)*/
    DATA *tmp = SearchKey(database, key, TYPE_LIST);
    if (!tmp) return 0;
    LIST *list = (LIST *) tmp->value;
    if (start < 0) start = list->Len+start;
    if (start < 0) start = 0;
    if (end < 0) end = list->Len+end;
    if ((list->Len<=start) || (end<start)) {printf("Empty array\n");return 0;}
    if (end >= list->Len) end = list->Len-1;

    BiNODE *start_ptr=list->MostLeft, *end_ptr=list->MostLeft;
    for (size_t i = 0; i < start; i++) start_ptr = start_ptr->right;
    for (size_t i = 0; i < end; i++) end_ptr = end_ptr->right;

    printf("List : %s\n", tmp->key);
    printf("Index\tValue\n");
    printf("---------------------------\n");
    printf("(Left)\n");
    size_t num = start;
    while (start_ptr!=end_ptr->right)
    {
        printf("%ld\t%s\n", num, start_ptr->Value);
        start_ptr = start_ptr->right;
        num++;
    }
    printf("(Right)\n");
    printf("---------------------------\n");
    return num;
}



void FreeList(DATA *const data)
{
    LIST *list = (LIST *) data->value;
    BiNODE *curr = list->MostLeft, *prev=NULL;
    while (curr)
    {
        prev = curr;
        curr = curr->right;
        free(prev->Value);
        free(prev);
    }
    free(list);
}