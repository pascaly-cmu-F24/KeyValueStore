cmake_minimum_required(VERSION 3.20)
project(KeyValueStore LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

# NOTE: this may need to be tweaked for windows
# Include and link paths:
#  - /opt/homebrew/include
#  - /opt/homebrew/lib
include_directories(
    ${PROJECT_SOURCE_DIR}/include 
    ${PROJECT_SOURCE_DIR}/src
    /opt/homebrew/include
)
link_directories(/opt/homebrew/lib)

# ------------------------------------------------------------------------------
# Build the 'driver' executable
# ------------------------------------------------------------------------------
# Since key_value_store.h and transaction_block.h are header-only, we just need
# to compile the driver .cc file into an executable.
add_executable(driver
    ${PROJECT_SOURCE_DIR}/src/driver.cc
)

# ------------------------------------------------------------------------------
# Build and run tests
# ------------------------------------------------------------------------------
# Enable testing via CMake
enable_testing()

# Create test executable from your test_key_value_store.cc
add_executable(test_runner
    ${PROJECT_SOURCE_DIR}/tests/test_key_value_store.cc
)


# Need these for testing:
include(GoogleTest)
target_link_libraries(test_runner
    gtest
    gtest_main
    gmock
    gmock_main
    pthread
)

# Register this test executable with CTest so `ctest` can run it
gtest_discover_tests(
    test_runner
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    PROPERTIES DISCOVERY_TIMEOUT 60
)
