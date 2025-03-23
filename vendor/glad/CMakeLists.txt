cmake_minimum_required(VERSION 3.26)

project(glad VERSION 0.1.0 LANGUAGES C)

add_library(${PROJECT_NAME} src/glad.c)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
