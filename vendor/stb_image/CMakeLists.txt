cmake_minimum_required(VERSION 3.26)

project(stb_image VERSION 0.1.0 LANGUAGES C)

add_library(${PROJECT_NAME} stb_image.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)