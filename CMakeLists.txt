cmake_minimum_required(VERSION 3.8.2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(network_buffer)

# Directory for thirdparty code
set(THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/thirdparty)

include_directories(include)

enable_testing(true)
add_subdirectory(utest)

# Can't find any way to pass the 'verbose' flag to ctest using the
#  normal 'make test' target
add_custom_target(check 
                  COMMAND ${CMAKE_CTEST_COMMAND} --verbose
                  DEPENDS utests)
