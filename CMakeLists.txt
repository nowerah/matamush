cmake_minimum_required(VERSION 3.8)
project(matamush)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -Werror -pedantic-errors -std=c++11")

set(SOURCE_FILES Group.h Group.cpp exceptions.h Group_test.cpp testMacros.h
        Group_test.cpp)
add_executable(matamush ${SOURCE_FILES})