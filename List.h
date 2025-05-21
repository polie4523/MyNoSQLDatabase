#ifndef __LIST_H
#define __LIST_H

typedef struct data_node DATA;
typedef struct data_base DATABASE;

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

DATA *NewList(DATABASE *const database, const char *const key); // 建立新list
void LPUSH(DATABASE *const database, const char *const key, const char *); // 在給定key的list最左邊push新資料
void RPUSH(DATABASE *const database, const char *const key, const char *); // 在給定key的list最右邊push新資料
void LPOP(const DATABASE *const database, const char *const key); // 在給定key的list最左邊pop資料
void RPOP(const DATABASE *const database, const char *const key); // 在給定key的list最右邊pop資料
size_t LLen(const DATABASE *const database, const char *const key); // 回傳給定key的list長度
// int DeleteList(DATABASE *const database, const char *const key); // 刪除給定key的list
size_t LRANGE(const DATABASE *const database, const char*, long, long); // 印出給定key的list的指定索引範圍內容
void FreeList(DATA *const data); // 釋放list的所有內容

#endif