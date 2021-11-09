cmake_minimum_required(VERSION 3.20)
project(SER334 C)

set(CMAKE_C_STANDARD 11)

add_executable(SER334
        Module-7/User.c Module-7/CoreyLoadBalancer.c Module-7/CoreyInstanceHost.c)

include_directories(Module-6/)


target_link_libraries(SER334 PRIVATE m)