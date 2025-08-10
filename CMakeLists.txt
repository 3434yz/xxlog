cmake_minimum_required(VERSION 3.14)  # FetchContent 需要 3.14+
project(xxlog LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 头文件目录
include_directories(include)

# ------------------------
# xxlog 库
# ------------------------
add_library(xxlog STATIC
    src/logger.cpp
)
target_include_directories(xxlog PUBLIC include)

# ------------------------
# Example 可执行程序
# ------------------------
add_executable(example example/main.cpp)
target_link_libraries(example PRIVATE xxlog)

# ------------------------
# Google Test 下载与引入
# ------------------------
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
# 解决 Windows 上 MSVC 的编译选项问题
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# ------------------------
# 测试可执行程序
# ------------------------
add_executable(xxlog_tests tests/xxlog_test.cpp)
target_link_libraries(xxlog_tests
    PRIVATE
        xxlog
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(xxlog_tests)
