#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "MyNoSQL.h"

void test_database_create_free() {
    DATABASE *db = NewDatabase();
    assert(db != NULL);
    FreeDatabase(db);
}

void test_string_api() {
    DATABASE *db = NewDatabase();
    assert(SET(db, "key1", "value1") == 0);
    assert(strcmp(GET(db, "key1"), "value1") == 0);
    assert(SET(db, "key2", "value2") == 0);
    assert(strcmp(GET(db, "key2"), "value2") == 0);
    assert(SRANGE(db, 0, -1) >= 2);
    assert(DEL(db, "key1", TYPE_STRING) == 0);
    assert(GET(db, "key1") == NULL);
    FreeDatabase(db);
}

void test_list_api() {
    DATABASE *db = NewDatabase();
    LPUSH(db, "list1", "a");
    RPUSH(db, "list1", "b");
    RPUSH(db, "list1", "c");
    assert(LLen(db, "list1") == 3);
    LRANGE(db, "list1", 0, -1);
    LPOP(db, "list1");
    assert(LLen(db, "list1") == 2);
    RPOP(db, "list1");
    assert(LLen(db, "list1") == 1);
    FreeDatabase(db);
}

void test_sorted_set_api() {
    DATABASE *db = NewDatabase();
    assert(ZADD(db, "zset1", 1.0, "one") == 0);
    assert(ZADD(db, "zset1", 2.0, "two") == 0);
    assert(ZADD(db, "zset1", 3.0, "three") == 0);
    assert(ZCARD(db, "zset1") == 3);
    assert(ZCOUNT(db, "zset1", 1.0, 2.0) == 2);
    ZRANGE(db, "zset1", 0, -1);
    ZRANGEBYSCORE(db, "zset1", 1.0, 3.0);
    assert(ZRANK(db, "zset1", "one") == 0);
    assert(ZREM(db, "zset1", "one") == 0);
    assert(ZCARD(db, "zset1") == 2);
    assert(ZREMRANGEBYSCORE(db, "zset1", 2.0, 2.0) == 0);
    ZRANGE(db, "zset1", 0, -1);
    FreeDatabase(db);
}

void test_hash_api() {
    DATABASE *db = NewDatabase();
    HSET(db, "hash1", "field1", "val1");
    HSET(db, "hash1", "field2", "val2");
    assert(strcmp(HGET(db, "hash1", "field1"), "val1") == 0);
    assert(strcmp(HGET(db, "hash1", "field2"), "val2") == 0);
    assert(HDEL(db, "hash1", "field1") == 0);
    assert(HGET(db, "hash1", "field1") == NULL);
    FreeDatabase(db);
}

int main() {
    printf("Testing database create/free...\n");
    test_database_create_free();
    printf("Testing string API...\n");
    test_string_api();
    printf("Testing list API...\n");
    test_list_api();
    printf("Testing sorted set API...\n");
    test_sorted_set_api();
    printf("Testing hash API...\n");
    test_hash_api();
    printf("All tests passed!\n");
    return 0;
}