cmake_minimum_required(VERSION 3.20)
project(SER334 C)

set(CMAKE_C_STANDARD 11)

add_executable(SER334
        Module-7/User.c)

include_directories(Module-7/)


target_link_libraries(SER334 PRIVATE m pthread)