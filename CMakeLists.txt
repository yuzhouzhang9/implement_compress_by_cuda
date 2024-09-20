cmake_minimum_required(VERSION 3.10)
project(CompressionProject)

set(CMAKE_CXX_STANDARD 14)

# 添加源文件
set(SOURCES
    src/cpp_compression/cpp_compression.cpp
    src/cpp_compression/cpp_huffman_tree.cpp
    test/test_cpp_compress.cpp
)

# 添加头文件目录
include_directories(src/cpp_compression)

# 创建可执行文件
add_executable(test_cpp_compress ${SOURCES})

# 如果你的项目用到了pthread库
find_package(Threads REQUIRED)
target_link_libraries(test_cpp_compress Threads::Threads)