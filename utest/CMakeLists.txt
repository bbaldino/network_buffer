# Set the Catch include directory
set(CATCH_INCLUDE_DIR ${THIRDPARTY_DIR}/Catch/single_include)
# Create an INTERFACE library (target) named 'Catch'
add_library(Catch INTERFACE)
# Set the include directories for the Catch target
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

# Fill out TEST_SOURCES to all .cc files in this directory
file(GLOB TEST_SOURCES "*.cc")

# Create the unit test executable
add_executable(utests ${TEST_SOURCES})

# Link the unit tests to Catch
target_link_libraries(utests Catch)

add_test(NAME utests 
         COMMAND ${CMAKE_BINARY_DIR}/utest/utests --force-colour)
