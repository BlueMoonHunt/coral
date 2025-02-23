cmake_minimum_required(VERSION 3.10.0)
project(glad VERSION 1.0.0 LANGUAGES C)

file(GLOB SOURCES "src/glad.c")

add_library(glad STATIC ${SOURCES})
target_include_directories(glad PRIVATE include)
