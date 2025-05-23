#ifndef __HASH_H
#define __HASH_H

#include "DataBase.h"
#define TABLE_SIZE 101
#define LOAD_THERSHOLD 0.7

typedef struct HashNode {
    char *value, *field;
    struct HashNode *next;
} HashNode_t;

typedef struct HashTable {
    size_t load;
    size_t tablesize;
    float load_factor;
    HashNode_t **head;
} HashTable_t;

void FreeHash(DATA *const data); // 釋放hash的所有內容

void _HSET(DATABASE *const database, const char *const key, const char *const field, const char *const value); // 新增hash的元素
char *_HGET(const DATABASE *const database, const char *const key, const char *const field); // 依照給定的key取得value
int _HDEL(DATABASE *const database, const char *const key, const char *const field); // 刪除hash的元素


#endif