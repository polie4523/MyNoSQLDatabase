#ifndef DATABASE_H
#define DATABASE_H

#include <stddef.h>

typedef enum {
    TYPE_STRING = 0,
    TYPE_LIST,
    TYPE_SORTEDSET,
    TYPE_HASH,
    NUMOFTYPE
} DATA_TYPE;

typedef struct data_node {
    char *key;
    void *value;
    struct data_node *next;
} DATA;

typedef struct data_base {
    DATA *datalist_head[NUMOFTYPE];
    DATA *datalist_tail[NUMOFTYPE];
    size_t length[NUMOFTYPE];
} DATABASE;

DATA *NewData(const char *const key); // 產生新資料節點
DATA *SearchKey(const DATABASE *const database, const char *const key, DATA_TYPE datatype); // 搜尋資料節點

#endif // DATABASE_H