#ifndef __STRING_H
#define __STRING_H

typedef struct data_node DATA;
typedef struct data_base DATABASE;

int SET(DATABASE *const database, const char *const key, const char *const value); // 建立新String
char *GET(const DATABASE *const database, const char *); // 依照給定的key取得value
// int DeleteString(S_DATA_LIST *, const char *); // 刪除給定的key的String
size_t SRANGE(const DATABASE *const database, long start, long end); // 印出所有String

#endif