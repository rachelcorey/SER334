cmake_minimum_required(VERSION 3.20)
project(SER334 C)

set(CMAKE_C_STANDARD 11)

add_executable(SER334
        Module-3/CoreyImageProcessor.c
        Module-3/BmpProcessor.c
        Module-3/PixelProcessor.c
        Module-3/PpmProcessor.c
        Module-3/BmpProcessor.h
        Module-3/PixelProcessor.h
        Module-3/PpmProcessor.h)

include_directories(Module-3/)

target_link_libraries(SER334 PRIVATE m)