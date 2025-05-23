#include <stdlib.h>
#include <string.h>
#include "MyNoSQL.h"
#include "String.h"
#include "List.h"
#include "Sorted_set.h"
#include "Hash.h"

DATABASE *NewDatabase(void) {
    DATABASE *new = (DATABASE *) malloc(sizeof(DATABASE));
    if (!new) exit(1);
    for (int i = 0; i < NUMOFTYPE; i++) {
        new->datalist_head[i] = NULL;
        new->datalist_tail[i] = NULL;
        new->length[i] = 0;
    }
    return new;
}

static void FreeData(DATA *const data, DATA_TYPE datatype) {
    switch (datatype)
    {
    case TYPE_STRING:
        free(data->value);
        data->value = NULL;
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
    data->key = NULL;
}

int DEL(DATABASE *const database, const char *const key, DATA_TYPE datatype) {
    size_t key_len = strlen(key)+1;
    DATA *curr=database->datalist_head[datatype], *prev=NULL;
    while (curr && strncmp(curr->key, key, key_len)) {
        prev = curr;
        curr = curr->next;
    }
    if (!curr) return 1;
    if (curr == database->datalist_head[datatype]) {
        database->datalist_head[datatype] = curr->next;
    } else {
        prev->next = curr->next;
    }
    FreeData(curr, datatype);
    free(curr);
    database->length[datatype]--;
    return 0;
}

void FreeDatabase(DATABASE *database) {
    for (int i = 0; i < NUMOFTYPE; i++) {
        DATA *curr = database->datalist_head[i];
        DATA *prev = NULL;
        while (curr) {
            FreeData(curr, i);
            prev = curr;
            curr = curr->next;
            free(prev);
            prev = NULL;
        }
    }
    free(database);
    database = NULL;
}

/* String資料型態API */
int SET(DATABASE *const database, const char *const key, const char *const value){
    return _SET(database, key, value);
}
char *GET(const DATABASE *const database, const char *const key){
    return _GET(database, key);
}
size_t SRANGE(const DATABASE *const database, long start, long end){
    return _SRANGE(database, start, end);
}

/* List資料型態API */
void LPUSH(DATABASE *const database, const char *const key, const char *const value) {
    _LPUSH(database, key, value);
}
void RPUSH(DATABASE *const database, const char *const key, const char *const value) {
    _RPUSH(database, key, value);
}
void LPOP(const DATABASE *const database, const char *const key) {
    _LPOP(database, key);
}
void RPOP(const DATABASE *const database, const char *const key) {
    _RPOP(database, key);
}
size_t LLen(const DATABASE *const database, const char *const key) {
    return _LLen(database, key);
}
size_t LRANGE(const DATABASE *const database, const char *const key, long start, long end) {
    return _LRANGE(database, key, start, end);
}

/* Sorted_set資料型態API */
int ZADD(DATABASE *const database, const char *const key, const double score, const char *const name) {
    return _ZADD(database, key, score, name);
}
size_t ZCARD(const DATABASE *const database, const char *const key) {
    return _ZCARD(database, key);
}
size_t ZCOUNT(const DATABASE *const database, const char *const key, const double min, const double max) {
    return _ZCOUNT(database, key, min, max);
}
size_t ZRANGE(const DATABASE *const database, const char *const key, long start, long stop) {
    return _ZRANGE(database, key, start, stop);
}
size_t ZRANGEBYSCORE(const DATABASE *const database, const char *const key, const double min, const double max) {
    return _ZRANGEBYSCORE(database, key, min, max);
}
size_t ZRANK(const DATABASE *const database, const char *const key, const char *const name) {
    return _ZRANK(database, key, name);
}
int ZREM(const DATABASE *const database, const char *const key, const char *const name) {
    return _ZREM(database, key, name);
}
int ZREMRANGEBYSCORE(const DATABASE *const database, const char *const key, const double min, const double max) {
    return _ZREMRANGEBYSCORE(database, key, min, max);
}

/* Hash資料型態API */
void HSET(DATABASE *const database, const char *const key, const char *const field, const char *const value) {
    _HSET(database, key, field, value);
}
char *HGET(const DATABASE *const database, const char *const key, const char *const field) {
    return _HGET(database, key, field);
}
int HDEL(DATABASE *const database, const char *const key, const char *const field) {
    return _HDEL(database, key, field);
}