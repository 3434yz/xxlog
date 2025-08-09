cmake_minimum_required(VERSION 3.10)
project(xxlog)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include_directories(include)

add_library(xxlog STATIC
    src/logger.cpp
)

add_executable(example example/main.cpp)
target_link_libraries(example xxlog)
