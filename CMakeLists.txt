cmake_minimum_required(VERSION 3.17)
project (tinyjson_test)

set(CMAKE_CXX_STANDARD 14)

add_library(tinyjson tinyjson.cpp)
add_executable(tinyjson_test test.cpp)
target_link_libraries(tinyjson_test gtest)
target_link_libraries(tinyjson_test tinyjson)
