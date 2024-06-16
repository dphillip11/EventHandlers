cmake_minimum_required(VERSION 3.5)
project(EventHandlersComparison)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories("${PROJECT_SOURCE_DIR}/CallbackRegistry")
include_directories("${PROJECT_SOURCE_DIR}/Observable")

# Add executable
add_executable(EventHandlersComparison main.cpp)
