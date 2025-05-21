#ifndef __LIST_H
#define __LIST_H

#include "DataBase.h"

typedef struct binode{ // 定義儲存資料的doubly linked list節點
    char *Value;
    struct binode *right;
    struct binode *left;
} BiNODE;

typedef struct list{ // 定義list的資料結構
    BiNODE *MostRight;
    BiNODE *MostLeft;
    size_t Len;
} LIST;


void _LPUSH(DATABASE *const database, const char *const key, const char *const value); // 在給定key的list最左邊push新資料
void _RPUSH(DATABASE *const database, const char *const key, const char *const value); // 在給定key的list最右邊push新資料
void _LPOP(const DATABASE *const database, const char *const key); // 在給定key的list最左邊pop資料
void _RPOP(const DATABASE *const database, const char *const key); // 在給定key的list最右邊pop資料
size_t _LLen(const DATABASE *const database, const char *const key); // 回傳給定key的list長度
size_t _LRANGE(const DATABASE *const database, const char *const key, long start, long end); // 印出給定key的list的指定索引範圍內容
void FreeList(DATA *const data); // 釋放list的所有內容

#endif