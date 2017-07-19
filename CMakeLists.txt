cmake_minimum_required(VERSION 3.8.2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(network_buffer)

# Setup Catch unit tests
set(CATCH_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/Catch/single_include)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})
enable_testing(true)
# Can't find any way to pass the 'verbose' flag to ctest using the
#  normal 'make test' target
add_custom_target(check 
                  COMMAND ${CMAKE_CTEST_COMMAND} --verbose
                  DEPENDS tests)
add_subdirectory(utest)

include_directories(include)
file(GLOB SOURCES "src/*.cc")
