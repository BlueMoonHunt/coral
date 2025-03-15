cmake_minimum_required(VERSION 3.26)

project(coral VERSION 0.1.0 LANGUAGES CXX)

# Vendor directory
set(VENDOR_DIR ${CMAKE_SOURCE_DIR}/vendor)

# Function to fetch and make available a package
function(fetch_and_make_available package_name git_repo git_tag)
    include(FetchContent)
    FetchContent_Declare(
        ${package_name}
        GIT_REPOSITORY ${git_repo}
        GIT_TAG ${git_tag}
        SOURCE_DIR "${VENDOR_DIR}/${package_name}"
    )
    FetchContent_MakeAvailable(${package_name})
endfunction()

# Try to find system GLFW
find_package(glfw3)

if(NOT glfw3_FOUND)
    message(STATUS "System GLFW not found, fetching from source.")
    fetch_and_make_available(glfw https://github.com/glfw/glfw.git v3.3.8)
    set(GLFW_INCLUDE_DIR ${VENDOR_DIR}/glfw/include)
    set(GLFW_LIBRARY glfw)
else()
    message(STATUS "System GLFW found.")
    set(GLFW_INCLUDE_DIR ${glfw3_INCLUDE_DIR})
    set(GLFW_LIBRARY glfw)
endif()

# Find OpenGL
find_package(OpenGL REQUIRED)

# Add GLAD as a subdirectory
add_subdirectory(${VENDOR_DIR}/glad)
set(GLAD_INCLUDE_DIR ${VENDOR_DIR}/glad/include)
set(GLAD_LIBRARY glad)

# Source files
file(GLOB_RECURSE SRC_FILES
src/*.cpp
)

# Create executable
add_library(${PROJECT_NAME} ${SRC_FILES})

# Include directories
target_include_directories(${PROJECT_NAME}
PUBLIC src
PRIVATE ${GLFW_INCLUDE_DIR}
PRIVATE ${GLAD_INCLUDE_DIR}
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
PRIVATE
${GLFW_LIBRARY}
${GLAD_LIBRARY}
OpenGL::OpenGL
)

# Compiler features and definitions
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

add_subdirectory(app)