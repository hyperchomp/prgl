add_executable(primitives main.c)

target_include_directories(primitives PRIVATE "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(primitives PRIVATE prgl)

# Put executable in same output folder as lib
set_target_properties(primitives PROPERTIES
    LINKER_LANGUAGE C
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)