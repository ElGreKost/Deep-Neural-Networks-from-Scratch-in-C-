cmake_minimum_required(VERSION 3.21)
project(myDnn)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(myDnn
        Pattern.h base.h common.h adaline.h test.cpp main.cpp Examples/abstractNetwork/abstract_Network.h Examples/adaline/adaline_ex1.h Examples/adaline/adaline_ex2.h plot_test.cpp backprop.h Examples/FeedForward/ffn_ex1.h Examples/FeedForward/ffn_ex1_simple.h Examples/adaline/inheritance_tests.h)
