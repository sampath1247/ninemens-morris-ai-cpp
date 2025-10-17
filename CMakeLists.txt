cmake_minimum_required(VERSION 3.18)
project(NineMensMorrisAI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Output
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Includes
include_directories(${CMAKE_SOURCE_DIR}/include)

# Core library for board representation
add_library(morris STATIC
    src/classboard.cpp
)
target_compile_options(morris PRIVATE -O2 -Wall -Wextra)

# Unit tests
enable_testing()
add_executable(unit_tests tests/unittesting.cpp)
target_link_libraries(unit_tests PRIVATE morris)
add_test(NAME unit_tests COMMAND unit_tests)

# NOTE: After you implement later parts, add executables like:
# add_executable(MiniMaxOpening src/MiniMaxOpening.cpp)
# target_link_libraries(MiniMaxOpening PRIVATE morris)
# ... and so on for MiniMaxGame, ABOpening, ABGame, etc.
