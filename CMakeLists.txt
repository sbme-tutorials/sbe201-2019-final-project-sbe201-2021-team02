cmake_minimum_required(VERSION 3.14)
project(sbe201_2019_final_project_sbe201_2021_team02)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(sbe201_2019_final_project_sbe201_2021_team02
        huffman.hpp
        huffmanCoding.cpp
        huffmanDecoding.cpp)
