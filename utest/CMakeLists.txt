file(GLOB TEST_SOURCES "*.cc")

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests Catch)
add_test(NAME tests 
         COMMAND ${CMAKE_BINARY_DIR}/utest/tests --force-colour)
