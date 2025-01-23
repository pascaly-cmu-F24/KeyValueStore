# Reference solution Makefile.
# NOTE: for larger C/C++ projects, the CMake build system is best.
# Author: Justin Ventura (jmventur@andrew.cmu.edu)

# g++ (clang on Mac), include path, C++ v20, all and extra warnings
CXX = g++
CXXFLAGS = -Iinclude -Isrc -I/opt/homebrew/include -std=c++20 -Wall -Wextra
LDFLAGS = -L/opt/homebrew/lib -lgmock -lgmock_main -lgtest -lgtest_main -pthread

OBJ = driver.o
TEST_OBJ = tests/test_key_value_store.o

all: driver

# Prereqs for creating the driver object file are after the colon
driver.o: src/driver.cc src/kvs/key_value_store.h
	$(CXX) $(CXXFLAGS) -c src/driver.cc -o driver.o

driver: $(OBJ)
	$(CXX) $(OBJ) -o driver

# Test compilation
tests/test_key_value_store.o: tests/test_key_value_store.cc src/kvs/key_value_store.h
	$(CXX) $(CXXFLAGS) -c tests/test_key_value_store.cc -o tests/test_key_value_store.o

test: $(TEST_OBJ)
	$(CXX) $(TEST_OBJ) -o test $(LDFLAGS)
	./test

# Clean up before submitting assignment
clean:
	rm -f $(OBJ) $(TEST_OBJ) driver test
