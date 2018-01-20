cmake_minimum_required(VERSION 3.8)
project(matamush)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -Werror -pedantic-errors -std=c++11")

set(SOURCE_FILES Group.h #[[Group.cpp]] exceptions.h Group_test.cpp MtmSet.h Area.h Clan.h World.h testMacros.h
        #[[main.cpp]] #[[examplePart1.cpp]] examplePart2.cpp #[[examplePart3.cpp]])
add_executable(matamush ${SOURCE_FILES})