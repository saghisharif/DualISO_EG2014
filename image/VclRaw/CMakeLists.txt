CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(VclRaw)

INCLUDE_DIRECTORIES(../../opensource/rawspeed)
add_library(VclRaw VclRaw.cpp)
target_include_directories(VclRaw PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})

