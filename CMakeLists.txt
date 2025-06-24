cmake_minimum_required(VERSION 3.15)
project(HuffPressor)

# Use modern C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set executable target
add_executable(huffpressor
    src/main.cpp
    src/compressor.cpp
    src/decompressor.cpp
    src/utils.cpp
    src/huffmanTree.cpp
    src/bitWriter.cpp     
)

# Add include directory for header files
target_include_directories(huffpressor
    PRIVATE ${CMAKE_SOURCE_DIR}/include
)

# Enable useful compiler warnings and optimization
target_compile_options(huffpressor
    PRIVATE -Wall -Wextra -pedantic -O2
)
