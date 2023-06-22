cmake_minimum_required(VERSION 3.21)
project(myDnn)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(myDnn
        0_abstract_NN_example.cpp
        Pattern.h base.h common.h adaline.h)
