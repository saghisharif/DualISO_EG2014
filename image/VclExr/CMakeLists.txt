CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(VclExr)

add_library(VclExr VclExr.cpp)
target_include_directories(VclExr PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})

