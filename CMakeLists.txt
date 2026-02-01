cmake_minimum_required(VERSION 3.30)
project(prgl VERSION 0.2.2
        DESCRIPTION "Pixel Retro GL Framework"
        LANGUAGES C)
set(CMAKE_C_STANDARD 99)

# Search the local directory first for libs
if (UNIX AND NOT APPLE)
    set(CMAKE_PREFIX_PATH "/usr/local" ${CMAKE_PREFIX_PATH})
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(PRGL_ENABLE_ASAN "Enable Address Sanitizer for memory issues" OFF)

option(PRGL_BUILD_EXAMPLES "Build example projects" ON)

add_library(${CMAKE_PROJECT_NAME})

# BUILD_INTERFACE is for the files in the source tree
# INSTALL_INTERFACE is for the files in the install (e.g. /usr/)
target_include_directories(${CMAKE_PROJECT_NAME} 
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/prgl>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/extern/include>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/extern>
)

set(PRGL_SOURCES
    "${CMAKE_SOURCE_DIR}/src/camera.c"
    "${CMAKE_SOURCE_DIR}/src/game.c"
    "${CMAKE_SOURCE_DIR}/src/game_object.c"
    "${CMAKE_SOURCE_DIR}/src/input.c"
    "${CMAKE_SOURCE_DIR}/src/lighting.c"
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
set(PRGL_ERROR_FLAGS -Wpedantic -Wall -Wextra -Werror)

# Disable Windows default error flags which conflict with common C99 coding practices
list(APPEND PRGL_ERROR_FLAGS 
    -Wno-documentation                   # Ignore Documentation comment syntax errors
    -Wno-documentation-unknown-command   # Ignore unknown @commands in comments
    -Wno-unsafe-buffer-usage             # Ignore C++ safe buffer suggestions
    -Wno-declaration-after-statement     # Allow C99 same line variable declarations
    -Wno-sign-conversion                 # Allow implicit conversions from signed to unsigned
    -Wno-implicit-int-float-conversion   # Allow implicit conversions from int to float
    -Wno-implicit-float-conversion       # Allow implicit conversions from double to float
    -Wno-double-promotion                # Allow float to double implicit conversion
    -Wno-covered-switch-default          # Allow 'default' in switches already covering enums
    -Wno-switch-enum                     # Don't require every enum case in every switch
    -Wno-nonportable-system-include-path # Ignore case-sensitivity in Windows file paths
    -Wno-cast-function-type-strict       # Allow casts for function types, needed with GLAD/GLFW
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND PRGL_ENABLE_ASAN)
    message(STATUS "Enabling Debug build flags...")
    set(ASAN_FLAGS -fsanitize=address -g -fno-omit-frame-pointer)
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE ${PRGL_ERROR_FLAGS} ${ASAN_FLAGS})
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE -fsanitize=address)
else()
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE ${PRGL_ERROR_FLAGS})
endif()

# Don't set extra flags for external libraries
if (MSVC OR CMAKE_C_SIMULATE_ID STREQUAL "MSVC")
    set_source_files_properties(
        ${PRGL_EXTERN_SOURCES} PROPERTIES COMPILE_FLAGS "/W0"
    )
else()
    set_source_files_properties(
        ${PRGL_EXTERN_SOURCES} PROPERTIES COMPILE_FLAGS ""
    )
endif()

include(GNUInstallDirs)
set(
    PRGL_PUBLIC_HEADERS "${CMAKE_SOURCE_DIR}/include/prgl/camera.h"
                        "${CMAKE_SOURCE_DIR}/include/prgl/common_macros.h"
                        "${CMAKE_SOURCE_DIR}/include/prgl/game.h"
                        "${CMAKE_SOURCE_DIR}/include/prgl/game_object.h"
                        "${CMAKE_SOURCE_DIR}/include/prgl/input.h"
                        "${CMAKE_SOURCE_DIR}/include/prgl/lighting.h"
                        "${CMAKE_SOURCE_DIR}/include/prgl/mathx.h"
                        "${CMAKE_SOURCE_DIR}/include/prgl/mesh.h"
                        "${CMAKE_SOURCE_DIR}/include/prgl/render.h"
                        "${CMAKE_SOURCE_DIR}/include/prgl/screen.h"
                        "${CMAKE_SOURCE_DIR}/include/prgl/shaders.h"
                        "${CMAKE_SOURCE_DIR}/include/prgl/texture.h"
                        "${CMAKE_SOURCE_DIR}/include/prgl/types.h"
)
set_target_properties(
    ${CMAKE_PROJECT_NAME} PROPERTIES
                          VERSION ${PROJECT_VERSION}
                          PUBLIC_HEADER "${PRGL_PUBLIC_HEADERS}"                                        
                          LIBRARY_OUTPUT_DIRECTORY ${CMAKE_INSTALL_LIBDIR}/${CMAKE_PROJECT_NAME}
)

# Link external libraries
if (WIN32)
    # Create necessary library sub-folders to search
    set(GLFW_ROOT_DIR ${CMAKE_PREFIX_PATH}/GLFW)

    # Find GLFW library and headers
    find_path(GLFW_INCLUDE_DIR NAMES GLFW/glfw3.h PATHS ${GLFW_ROOT_DIR} PATH_SUFFIXES include REQUIRED)
    find_library(GLFW_LIBRARY NAMES glfw3 PATHS ${GLFW_ROOT_DIR} REQUIRED)

    target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIR})
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${GLFW_LIBRARY} opengl32)
else()
    find_package(glfw3 REQUIRED)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glfw)
endif()

# Install the includes and lib files, export targets needed for find_package()
install(
    TARGETS ${CMAKE_PROJECT_NAME}
    EXPORT ${CMAKE_PROJECT_NAME}-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME}
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

if (PRGL_BUILD_EXAMPLES)
    add_subdirectory(examples EXCLUDE_FROM_ALL)
endif()
