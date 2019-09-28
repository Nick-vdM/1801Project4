cmake_minimum_required(VERSION 3.14)
project(Project4)

set(CMAKE_CXX_STANDARD 14)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(Task1 Task1.cpp)
add_executable(Task2 Task2.cpp)
add_executable(gendata gendata.cpp)
add_executable(gengraph gengraph.cpp)
add_executable(helloworld helloworld.cpp)