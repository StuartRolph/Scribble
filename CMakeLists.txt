cmake_minimum_required(VERSION 3.21)
project(scribble)

set(CMAKE_CXX_STANDARD 14)

include_directories(external)
include_directories(external/stb)
include_directories(src/Image)

add_executable(scribble
        external/stb/stb_image.h
        external/stb/stb_image_write.h
        src/Image/Image.cpp
        src/Image/Image.h
        src/main.cpp)
