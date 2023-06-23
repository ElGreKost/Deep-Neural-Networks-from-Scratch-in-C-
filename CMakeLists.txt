cmake_minimum_required(VERSION 3.21)
project(myDnn)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(myDnn
        Pattern.h base.h common.h adaline.h test.cpp main.cpp Examples/abstract_Network.h Examples/adaline_Network.h)
