cmake_minimum_required(VERSION 3.10)

project(regex_literals)
add_library(regex_literals INTERFACE)
target_include_directories(regex_literals INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")