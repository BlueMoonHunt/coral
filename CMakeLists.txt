cmake_minimum_required(VERSION 3.26) # Or higher if needed for specific features

project(coral VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

# Source files
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp src/*.c app/*.cpp app/*.c)

add_executable(${PROJECT_NAME} ${SRC_FILES})

# Include directories (modern target_include_directories)
target_include_directories(${PROJECT_NAME}
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include> # If you are installing.
)

# Link libraries (modern target_link_libraries)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        glfw
        Vulkan::Vulkan
)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# Optional: Install headers. If you have headers in your src folder.
install(DIRECTORY src/ DESTINATION include FILES_MATCHING PATTERN "*.h")

# Optional: Configure CTest
enable_testing()