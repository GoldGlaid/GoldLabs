cmake_minimum_required(VERSION 3.22.1)
project(GoldLabs C)

set(CMAKE_C_STANDARD 23)

add_executable(GoldLabs
        lab1/task5/task1_5.c
        "lab1/task9/task1_9(1).c"
        lab5/task3/task5_3.cpp
)
add_executable(lab1.5 lab1/task5/task1_5.c)
add_executable(lab1.9_1 "lab1/task9/task1_9(1).c")
add_executable(lab1.9_2 "lab1/task9/task1_9(2).c")
add_executable(lab2.9 lab2/task9/task2_9.c)
add_executable(lab5.2 lab5/task2/task5_2.cpp)
add_executable(lab5.3 lab5/task3/task5_3.cpp)
