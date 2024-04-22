CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(arg)

add_library(arg arg.c expr.c)
target_include_directories(arg PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})
