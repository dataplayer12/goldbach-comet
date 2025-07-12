cmake_minimum_required(VERSION 3.10)

# Project name and language
project(goldbach_comet LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include/ /usr/local/include)

add_library(gb lib/goldbach/goldbach.cpp)
target_link_libraries(gb m)

# Add the main executable
add_executable(compute src/main.cpp)

# Link the gb library
target_link_libraries(compute PRIVATE gb)

add_executable(recursive src/recursive.cpp)
target_link_libraries(recursive PRIVATE gb)