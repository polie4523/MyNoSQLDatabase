#ifndef __HEADER2_H
#define __HEADER2_H

#include "DataBase.h"

#define MAX_LEVEL 3

typedef struct SkipListNode {
    char *name;
    double score;
    struct SkipListNode *forward[MAX_LEVEL+1];
} Sorted_set_element;

typedef struct SkipList {
    Sorted_set_element *head;
    size_t len, level;
} Sorted_set;


// Sorted_set_element *SearchElement(const Sorted_set *const set, const double score);
void FreeSortedSet(DATA *const data);

int _ZADD(DATABASE *const database, const char *const key, const double score, const char *const name); // 新增sorted set的元素
size_t _ZCARD(const DATABASE *const database, const char *const key); // 回傳全部sorted set的元素數量
size_t _ZCOUNT(const DATABASE *const database, const char *const key, const double min, const double max); // 回傳給定分數範圍的sorted set的元素數量
size_t _ZRANGE(const DATABASE *const database, const char *const key, long start, long stop); // 印出給定索引範圍的sorted set的元素
size_t _ZRANGEBYSCORE(const DATABASE *const database, const char *const key, const double min, const double max); // 印出給定分數範圍的sorted set的元素
size_t _ZRANK(const DATABASE *const database, const char *const key, const char *const name); // 回傳給定名字的sorted set的元素索引
int _ZREM(const DATABASE *const database, const char *const key, const char *const name); // 刪除給定名字的sorted set的元素
int _ZREMRANGEBYSCORE(const DATABASE *const database, const char *const key, const double min, const double max); // 刪除給定分數範圍的sorted set的元素


#endif