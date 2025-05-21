#ifndef __HASH_H
#define __HASH_H

#include "DataBase.h"
#define TABLE_SIZE 101
#define LOAD_THERSHOLD 0.7


typedef struct HashNode
{
    char *value, *field;
    struct HashNode *next;
}HashNode_t;


typedef struct HashTable
{
    size_t load;
    size_t tablesize;
    float load_factor;
    HashNode_t **head;
}HashTable_t;


// static size_t HashFcn(const char *const key, const size_t tablesize); // djb2
DATA *NewHash(DATABASE *const database, const char *const key); // 建立新hash
HashTable_t *ReArrange(HashTable_t *hashtable);
void HSET(DATABASE *const database, const char *const key, const char *const field, const char *const value);
char *HGET(const DATABASE *const database, const char *const key, const char *const field);
int HDEL(DATABASE *const database, const char *const key, const char *const field);
void FreeHash(DATA *const data); // 釋放hash的所有內容

#endif