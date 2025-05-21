#ifndef __MyNoSQL_H
#define __MyNoSQL_H

#include <stdlib.h>
#include "String.h"
#include "List.h"
#include "Sorted_set.h"
#include "Hash.h"

#define NUMOFTYPE 4
#define TYPE_STRING 0
#define TYPE_LIST 1
#define TYPE_SORTEDSET 2
#define TYPE_HASH 3

typedef struct data_node
{
    char *key;
    void *value;
    struct data_node *next;
} DATA;

typedef struct data_base
{
    DATA *datalist_head[NUMOFTYPE];
    DATA *datalist_tail[NUMOFTYPE];
    size_t length[NUMOFTYPE];
} DATABASE;

DATABASE *NewDatabase(void); //建立資料庫物件
DATA *NewData(const char *const key);
DATA *SearchKey(const DATABASE *const database, const char *const key, const int datatype);
int DEL(DATABASE *const database, const char *const key, const int datatype);
void FreeData(DATA *const data, const int datatype);
void FreeDatabase(DATABASE *const database); //釋放資料庫物件

#endif