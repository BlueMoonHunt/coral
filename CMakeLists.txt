cmake_minimum_required(VERSION 3.10)
project(coral VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Add external dependencies
add_subdirectory(vendor/glad)

# Collect source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "app/*.cpp")

# Define executable
add_executable(coral ${SOURCES})

# Include directories
target_include_directories(coral
    PRIVATE src
    PRIVATE app
    PRIVATE vendor/stb_image
    PRIVATE vendor/glad/include
    PRIVATE vendor/entt
)

# Try to find GLFW system-wide first
find_package(glfw3 QUIET)

if(NOT glfw3_FOUND)
    message(STATUS "GLFW not found, downloading as a submodule...")

    # Fetch GLFW if not already present
    include(FetchContent)
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.8  # Replace with the latest stable version if needed
    )
    FetchContent_MakeAvailable(glfw)

    # Use locally downloaded GLFW
    add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# Link GLFW (either system or downloaded version)
target_link_libraries(coral PRIVATE glfw PRIVATE glad)

# Platform-specific linking
if(WIN32)  # Windows
    target_link_libraries(coral PRIVATE opengl32)
elseif(APPLE)  # macOS (Cocoa)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)
    target_link_libraries(coral PRIVATE ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
elseif(UNIX AND NOT APPLE)  # Linux (X11)
    find_package(X11 REQUIRED)
    target_link_libraries(coral PRIVATE X11 Xrandr Xinerama Xcursor Xext)
endif()
