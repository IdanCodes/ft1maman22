cmake_minimum_required(VERSION 3.26)
project(maman22 C)

set(CMAKE_C_STANDARD 90)

add_executable(maman22 mainmat.c
        mymat.c
        inputhelper.c
        errorhelper.c
        mathelper.c)
