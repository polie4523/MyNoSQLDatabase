// valgrind --tool=memcheck --leak-check=full --track-origins=yes -s ./test
#include <stdlib.h>
#include <stdio.h>
#include "MyNoSQL.h"

int main(void)
{
    DATABASE *MyDatabase = NewDatabase();
    // HSET(MyDatabase, "test", "eric", "24");
    // HSET(MyDatabase, "test1", "amy", "22");
    // HSET(MyDatabase, "test2", "booklyn", "21");
    // HSET(MyDatabase, "test3", "canon", "14");

    // HGET(MyDatabase, "test3", "canon");
    // HGET(MyDatabase, "test", "eric");
    // HGET(MyDatabase, "test12", "asdasd");
    
    // HDEL(MyDatabase, "test", "amy");
    // HGET(MyDatabase, "test", "amy");
    
    // SET(MyDatabase, "test", "eric");
    // SET(MyDatabase, "test1", "booklyn");
    // SET(MyDatabase, "test2", "canon");
    // GET(MyDatabase, "test1");
    // SRANGE(MyDatabase, 0, -1);
    // DEL(MyDatabase, "test", TYPE_STRING);
    // SET(MyDatabase, "test", "eric1");
    // SRANGE(MyDatabase, 0, -1);

    // LPUSH(MyDatabase, "test", "eric");
    // LPUSH(MyDatabase, "test", "eric2");
    // LPUSH(MyDatabase, "test", "eric3");
    // LPUSH(MyDatabase, "test", "eric4");
    // LPUSH(MyDatabase, "test", "eric5");
    // LRANGE(MyDatabase, "test", 0, -1);
    // LPOP(MyDatabase, "test");
    // LRANGE(MyDatabase, "test", 0, -1);
    // RPOP(MyDatabase, "test");
    // LRANGE(MyDatabase, "test", 2, -1);
    
    // DEL(MyDatabase, "test", TYPE_LIST);
    // printf("After delete\n");
    // LRANGE(MyDatabase, "test", 0, -1);


    ZADD(MyDatabase, "test", 1.0, "eric");
    ZADD(MyDatabase, "test", 2.0, "booklyn");
    ZADD(MyDatabase, "test2", 5.0, "canon");
    ZADD(MyDatabase, "test2", 4.0, "eric2");
    ZADD(MyDatabase, "test2", 3.0, "eric3");
    ZRANGE(MyDatabase, "test", 0, -1);
    printf("%d\n",ZRANK(MyDatabase, "test", "eric2"));
    // ZREMRANGEBYSCORE(MyDatabase, "test", 2.0, 4.0);
    ZREM(MyDatabase, "test", "eric");
    ZRANGE(MyDatabase, "test2", 0, -1);
    printf("%d\n",ZRANK(MyDatabase, "test", "eric2"));
    FreeDatabase(MyDatabase);
    return 0;
}