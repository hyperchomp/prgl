cmake_minimum_required(VERSION 3.30...4.0)
project(pr3d VERSION 0.0.1
        DESCRIPTION "Pixel Retro 3D Game Framework"
        LANGUAGES C)
set(CMAKE_C_STANDARD 23)

# Compiler flags for extra warnings/errors
set(C_FLAGS "-pedantic -Wall -Wextra -Werror")

add_executable(${CMAKE_PROJECT_NAME})
target_sources(
    ${CMAKE_PROJECT_NAME} PRIVATE main.c
                                  window.c
                                  input.c
)

# Libraries, if local to the project need add_subdirectory
find_package(glfw3 REQUIRED) # Generates imported target glfw

# Libs stored local to project
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/glad/include)
add_library(
    glad 
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c
)

# Add compiler flags only to this project, libs in subdirectories with
# CMakeLists files can be added right after the main project name
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES COMPILE_FLAGS ${C_FLAGS})

# Link external libraries
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glfw glad)
