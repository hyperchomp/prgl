add_executable(primitives main.c)

target_include_directories(primitives PRIVATE "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(primitives PRIVATE prgl)

# Put executable in same output folder as lib
set_target_properties(primitives PROPERTIES
    LINKER_LANGUAGE C
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

# List of assets to export 
set(PRIMITIVES_TEXTURES
    "crate.png"
)

# Copy each asset directly to your hardcoded bin folder
foreach(TEXTURE_FILE ${PRIMITIVES_TEXTURES})
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/${TEXTURE_FILE}"
        "${CMAKE_BINARY_DIR}/bin/${TEXTURE_FILE}"
        COPYONLY
    )
endforeach()

