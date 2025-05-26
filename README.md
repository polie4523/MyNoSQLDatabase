# MyNoSQLDatabase

MyNoSQLDatabase 是一個簡單的 NoSQL 資料庫專案，為2023年中大資料結構課程專案作業，旨在學習基本資料結構與實作非關聯式資料儲存技術。使用C語言仿照Redis的指令格式與部分的儲存資料類型實現了自己的簡易NoSQL資料庫。經Valgrind測試無發生記憶體洩漏。

**儲存資料類型**
1. String 儲存簡單字串健值對，使用Linked list實作
2. List 儲存一序列字串健值對，使用doubly linked list實作
3. Sorted Set 儲存有序的字串健值對，依照分數排序，使用skip list實作
4. Hash 儲存鍵值對的集合，使用hash table實作

## API說明
所有api都宣告在"MyNoSQL.h"，使用只需包含該檔案，api指令格式仿照Redis，範例請看"test_MyNoSQL.c"。以下是各資料型態的API說明：

```c
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

/* Sorted Set資料型態API */
int ZADD(DATABASE *const database, const char *const key, const double score, const char *const name); // 新增sorted set的元素
size_t ZCARD(const DATABASE *const database, const char *const key); // 回傳全部sorted set的元素數量
size_t ZCOUNT(const DATABASE *const database, const char *const key, const double min, const double max); // 回傳給定分數範圍的sorted set的元素數量
size_t ZRANGE(const DATABASE *const database, const char *const key, long start, long stop); // 印出給定索引範圍的sorted set的元素
size_t ZRANGEBYSCORE(const DATABASE *const database, const char *const key, const double min, const double max); // 印出給定分數範圍的sorted set的元素
size_t ZRANK(const DATABASE *const database, const char *const key, const char *const name); // 回傳給定名字的sorted set的元素索引
int ZREM(const DATABASE *const database, const char *const key, const char *const name); // 刪除給定名字的sorted set的元素
int ZREMRANGEBYSCORE(const DATABASE *const database, const char *const key, const double min, const double max); // 刪除給定分數範圍的sorted set的元素

/* Hash資料型態API */
void HSET(DATABASE *const database, const char *const key, const char *const field, const char *const value); // 新增hash的元素
char *HGET(const DATABASE *const database, const char *const key, const char *const field); // 依照給定的key取得value
int HDEL(DATABASE *const database, const char *const key, const char *const field); // 刪除hash的元素
```

## 安裝與使用
測試環境:
- 作業系統:Ubuntu 24.04 LTS
- cmake版本:3.28.3
- gcc版本:13.3.0
- valgrind版本:3.22.0

```bash
git clone https://github.com/yourusername/MyNoSQLDatabase.git
cd MyNoSQLDatabase
mkdir build && cd build
cmake ../
make
./test_MyNoSQL
valgrind --tool=memcheck --leak-check=full --track-origins=yes -s ./test_MyNoSQL # valgrind記憶體洩漏測試
```