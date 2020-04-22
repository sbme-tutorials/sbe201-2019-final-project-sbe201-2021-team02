cmake_minimum_required(VERSION 3.10.2 )

### c++11 standards
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project( huffman_compression )

include_directories( include )

add_executable( compress apps/test_huffman_code.cpp )
add_executable( decompress apps/test_huffman_decode.cpp )

