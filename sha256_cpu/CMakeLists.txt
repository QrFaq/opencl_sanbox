cmake_minimum_required(VERSION 3.13)
project(simple_example)
set(CMAKE_CXX_STANDARD 14)

include_directories( ${MY_SOURCE_DIR} )
set(SOURCE_FILES main.cpp sha256.cpp)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(main ${SOURCE_FILES})