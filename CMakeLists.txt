cmake_minimum_required(VERSION 3.15)
project(HuffPressor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(huffpressor
    src/main.cpp
    src/compressor.cpp
    src/decompressor.cpp
    src/utils.cpp
)

# Add compiler warnings (like -Wall -Wextra -pedantic)
target_compile_options(huffpressor PRIVATE -Wall -Wextra -pedantic -O2)