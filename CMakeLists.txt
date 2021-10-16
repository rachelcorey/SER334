cmake_minimum_required(VERSION 3.20)
project(SER334 C)

set(CMAKE_C_STANDARD 11)

add_executable(SER334
        Module-1/CoreyCollatz.c
        Module-1/CoreyComputation.c
        Module-1/CoreyDecrypt.c
        Module-1/test.c)

target_link_libraries(SER334 PRIVATE m)