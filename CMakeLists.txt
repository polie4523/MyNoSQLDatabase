cmake_minimum_required(VERSION 3.18)

project(MyNoSQL)
set(SOURCE_FILES
    MyNoSQL.c 
    DataBase.c
    String.c
    List.c 
    Sorted_set.c
    Hash.c
)
add_library(MyNoSQL ${SOURCE_FILES})
add_executable(test_MyNoSQL test_MyNoSQL.c)
target_link_libraries(test_MyNoSQL MyNoSQL)