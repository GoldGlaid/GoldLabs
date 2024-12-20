cmake_minimum_required(VERSION 3.22.1)
project(GoldLabs C CXX)

set(CMAKE_C_STANDARD 23)

add_executable(GoldLabs
        lab1/task5/task1_5.c
        lab5/task3/task5_3.cpp
)
add_executable(lab1.5 lab1/task5/task1_5.c)
add_executable(lab1.9_1 "lab1/task9/task1_9(1).c")
add_executable(lab1.9_2 "lab1/task9/task1_9(2).c")
add_executable(lab2.9 lab2/task9/task2_9.c)
add_executable(lab5.2 lab5/task2/task5_2.cpp)
add_executable(lab5.3 lab5/task3/task5_3.cpp)
add_executable(lab5.1 lab5/task1/task5_1.cpp
        lab5/task1/functions.cpp)
add_executable(lab2.8 lab2/task8/task2_8.c)
add_executable(lab2.3 lab2/task3/task2_3.c)
add_executable(lab2.4 lab2/task4/task2_4.c)
add_executable(lab3.10 lab3/task10/task3_10.c)

add_executable(lab2.10 lab2/task10/task2_10.c)
add_executable(lab1.2 lab1/task2/task1_2.c
        lab1/task2/functions.c
        lab1/task2/functions.h)
add_executable(lab5.7 lab5/task7/task5_7.cpp)
add_executable(lab3.2 lab3/task2/task3_2.c
        lab3/task2/functions.h
        lab3/task2/functions.c)

target_link_libraries(lab2.10 m)
target_link_libraries(lab1.2 m)
target_link_libraries(lab3.2 m)
