cmake_minimum_required(VERSION 3.8.2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(network_buffer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(NetworkBuffer INTERFACE)
target_include_directories(NetworkBuffer INTERFACE include)

# Directory for thirdparty code
set(THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/thirdparty)

add_subdirectory(thirdparty/catch)

enable_testing(true)
add_subdirectory(utest)

# Can't find any way to pass the 'verbose' flag to ctest using the
#  normal 'make test' target
add_custom_target(check 
                  COMMAND ${CMAKE_CTEST_COMMAND} --verbose
                  DEPENDS utests)
