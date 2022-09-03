cmake_minimum_required(VERSION 3.19)
project(logging VERSION 1.0 DESCRIPTION logging LANGUAGES CXX)

add_library(logging OBJECT src/logging.cpp)

include_directories("${PROJECT_BINARY_DIR}/include/")
include_directories("${PROJECT_SOURCE_DIR}/include/")
