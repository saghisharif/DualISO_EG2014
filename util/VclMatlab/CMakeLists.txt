CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(VclMatlab)

INCLUDE_DIRECTORIES(/usr/local/include/eigen3)

add_library(VclMatlab VclMatlab.cpp)
target_include_directories(VclMatlab PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})
