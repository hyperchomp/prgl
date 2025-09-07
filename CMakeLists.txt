cmake_minimum_required(VERSION 3.30)
project(prgl VERSION 0.2.0
        DESCRIPTION "Pixel Retro GL Framework"
        LANGUAGES C)
set(CMAKE_C_STANDARD 99)

# Search the local directory first for libs
set(CMAKE_PREFIX_PATH "/usr/local" ${CMAKE_PREFIX_PATH})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(PRGL_ENABLE_ASAN "Enable Address Sanitizer for memory issues" OFF)

add_library(${CMAKE_PROJECT_NAME})

# BUILD_INTERFACE is for the files in the source tree
# INSTALL_INTERFACE is for the files in the install (e.g. /usr/)
target_include_directories(${CMAKE_PROJECT_NAME} 
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/extern/include>
        $<INSTALL_INTERFACE:include/${CMAKE_PROJECT_NAME}>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/extern>
)

set(PRGL_SOURCES
    "${CMAKE_SOURCE_DIR}/src/camera.c"
    "${CMAKE_SOURCE_DIR}/src/input.c"
    "${CMAKE_SOURCE_DIR}/src/game.c"
    "${CMAKE_SOURCE_DIR}/src/game_object.c"
    "${CMAKE_SOURCE_DIR}/src/mathx.c"
    "${CMAKE_SOURCE_DIR}/src/mesh.c"
    "${CMAKE_SOURCE_DIR}/src/render.c"
    "${CMAKE_SOURCE_DIR}/src/screen.c"
    "${CMAKE_SOURCE_DIR}/src/shaders.c"
    "${CMAKE_SOURCE_DIR}/src/shaders_init.c"
    "${CMAKE_SOURCE_DIR}/src/texture.c"
    "${CMAKE_SOURCE_DIR}/src/transform.c"
)

set(PRGL_EXTERN_SOURCES
    "${CMAKE_SOURCE_DIR}/extern/glad.c"
    "${CMAKE_SOURCE_DIR}/extern/stb_image.c"
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${PRGL_SOURCES} ${PRGL_EXTERN_SOURCES})

# Compiler flags for extra warnings/errors
set(PRGL_ERROR_FLAGS -pedantic -Wall -Wextra -Werror)
if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND PRGL_ENABLE_ASAN)
    message(STATUS "Enabling Debug build flags...")
    set(ASAN_FLAGS -fsanitize=address -g -fno-omit-frame-pointer)
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE ${PRGL_ERROR_FLAGS} ${ASAN_FLAGS})
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE -fsanitize=address)
else()
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE ${PRGL_ERROR_FLAGS})
endif()

# Don't set extra flags for external libraries
set_source_files_properties(
    ${PRGL_EXTERN_SOURCES} PROPERTIES COMPILE_FLAGS ""
)

include(GNUInstallDirs)
set(
    PRGL_PUBLIC_HEADERS "${CMAKE_SOURCE_DIR}/include/camera.h"
                        "${CMAKE_SOURCE_DIR}/include/common_macros.h"
                        "${CMAKE_SOURCE_DIR}/include/game.h"
                        "${CMAKE_SOURCE_DIR}/include/game_object.h"
                        "${CMAKE_SOURCE_DIR}/include/input.h"
                        "${CMAKE_SOURCE_DIR}/include/mathx.h"
                        "${CMAKE_SOURCE_DIR}/include/mesh.h"
                        "${CMAKE_SOURCE_DIR}/include/render.h"
                        "${CMAKE_SOURCE_DIR}/include/screen.h"
                        "${CMAKE_SOURCE_DIR}/include/shaders.h"
                        "${CMAKE_SOURCE_DIR}/include/texture.h"
                        "${CMAKE_SOURCE_DIR}/include/types.h"
)
set_target_properties(
    ${CMAKE_PROJECT_NAME} PROPERTIES
                          VERSION ${PROJECT_VERSION}
                          PUBLIC_HEADER "${PRGL_PUBLIC_HEADERS}"                                        
                          LIBRARY_OUTPUT_DIRECTORY ${CMAKE_INSTALL_LIBDIR}/${CMAKE_PROJECT_NAME}
)

# Link external libraries
find_package(glfw3 REQUIRED) # Generates imported target glfw

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glfw) 

# Install the includes and lib files, export targets needed for find_package()
install(
    TARGETS ${CMAKE_PROJECT_NAME}
    EXPORT ${CMAKE_PROJECT_NAME}-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${CMAKE_PROJECT_NAME}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/${CMAKE_PROJECT_NAME}
)

# Install cglm library files into a cglm subdirectory
install(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/extern/include/cglm"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME}"
)

# CMake install
install(
	EXPORT ${CMAKE_PROJECT_NAME}-targets
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}
)

# Install the CMakeConfig file for using find_package()
# Include CMakePackageConfigHelpers for generating config files
include(CMakePackageConfigHelpers)

# Configure the package config file
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}"
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

# Install the Config.cmake file
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}"
)
