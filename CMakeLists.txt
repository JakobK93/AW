cmake_minimum_required(VERSION 3.0)
project(awp)

set(CMAKE_CXX_STANDARD 11)

file(MAKE_DIRECTORY ./build/log ${directory})
file(MAKE_DIRECTORY ./build/video ${directory})
file(MAKE_DIRECTORY ./build/photo ${directory})

find_library(LIBGPIOD gpiod)

add_executable(awp awp.cpp)

target_link_libraries(awp ${LIBGPIOD})

