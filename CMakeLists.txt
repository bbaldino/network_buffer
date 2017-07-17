cmake_minimum_required(VERSION 3.8.2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(network_buffer)

#find_package(Boost 1.61.0 REQUIRED system)

# Setup Catch unit tests
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/Catch/single_include)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

set(TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/utest/utest.cc)
add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests Catch)

include_directories(include)
file(GLOB SOURCES "src/*.cc" "test/*.cc")

#add_executable(titanic ${SOURCES})
