cmake_minimum_required(VERSION 3.8)
project(matamush)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Group.h)
add_executable(matamush ${SOURCE_FILES})