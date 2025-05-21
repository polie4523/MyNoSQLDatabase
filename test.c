// valgrind --tool=memcheck --leak-check=full --track-origins=yes -s ./test
#include <stdlib.h>
#include <stdio.h>
#include "MyNoSQL.h"

int main(void)
{
    DATABASE *MyDatabase = NewDatabase();
    HSET(MyDatabase, "test", "eric", "24");
    HSET(MyDatabase, "test1", "amy", "22");
    HSET(MyDatabase, "test2", "booklyn", "21");
    HSET(MyDatabase, "test3", "canon", "14");

    HGET(MyDatabase, "test3", "canon");
    HGET(MyDatabase, "test", "eric");
    HGET(MyDatabase, "test12", "asdasd");
    
    HDEL(MyDatabase, "test", "amy");
    HGET(MyDatabase, "test", "amy");
    FreeDatabase(MyDatabase);
    return 0;
}