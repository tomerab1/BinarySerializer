cmake_minimum_required(VERSION 3.26)
project(BinSer)

set(CMAKE_CXX_STANDARD 17)

add_library(BinSer INTERFACE
        tests/test_primitives.cpp)
target_include_directories(BinSer INTERFACE ${CMAKE_SOURCE_DIR}/include)

######################################

enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(TestBinSer ${CMAKE_SOURCE_DIR}/tests/test_primitives.cpp)

find_package(GTest CONFIG REQUIRED)
target_link_libraries(TestBinSer PRIVATE GTest::gtest GTest::gtest_main)
