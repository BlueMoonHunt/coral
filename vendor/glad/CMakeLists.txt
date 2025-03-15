cmake_minimum_required(VERSION 3.26)

project(glad VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(${PROJECT_NAME} src/glad.c)
target_include_directories(${PROJECT_NAME} PUBLIC include)