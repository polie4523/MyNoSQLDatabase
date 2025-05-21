#ifndef __MyNoSQL_H
#define __MyNoSQL_H

#include "DataBase.h"

DATABASE *NewDatabase(void); //建立資料庫物件
int DEL(DATABASE *const database, const char *const key, DATA_TYPE datatype); // 刪除對應key的資料節點
void FreeDatabase(DATABASE *database); //釋放資料庫物件

/* String資料型態API */
int SET(DATABASE *const database, const char *const key, const char *const value); // 建立新String
char *GET(const DATABASE *const database, const char *const key); // 依照給定的key取得value
size_t SRANGE(const DATABASE *const database, long start, long end); // 印出所有String

/* List資料型態API */
void LPUSH(DATABASE *const database, const char *const key, const char *const value); // 在給定key的list最左邊push新資料
void RPUSH(DATABASE *const database, const char *const key, const char *const value); // 在給定key的list最右邊push新資料
void LPOP(const DATABASE *const database, const char *const key); // 在給定key的list最左邊pop資料
void RPOP(const DATABASE *const database, const char *const key); // 在給定key的list最右邊pop資料
size_t LLen(const DATABASE *const database, const char *const key); // 回傳給定key的list長度
size_t LRANGE(const DATABASE *const database, const char *const key, long start, long end); // 印出給定key的list的指定索引範圍內容

#endif