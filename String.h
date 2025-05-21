#ifndef __STRING_H
#define __STRING_H

#include "DataBase.h"

int _SET(DATABASE *const database, const char *const key, const char *const value); // 建立新String
char *_GET(const DATABASE *const database, const char *const key); // 依照給定的key取得value
size_t _SRANGE(const DATABASE *const database, long start, long end); // 印出所有String

#endif