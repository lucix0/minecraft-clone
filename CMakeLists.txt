cmake_minimum_required(VERSION 3.20)
project(MinecraftClone LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(BGFX_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/third_party/bgfx)

add_library(bgfx STATIC IMPORTED GLOBAL)
set_target_properties(bgfx PROPERTIES
    IMPORTED_LOCATION
        ${BGFX_ROOT}/lib/libbgfxRelease.a
    INTERFACE_INCLUDE_DIRECTORIES
        ${BGFX_ROOT}/include
)

add_library(bx STATIC IMPORTED GLOBAL)
set_target_properties(bx PROPERTIES
    IMPORTED_LOCATION
        ${BGFX_ROOT}/lib/libbxRelease.a
    INTERFACE_INCLUDE_DIRECTORIES
        ${BGFX_ROOT}/include
    INTERFACE_COMPILE_DEFINITIONS
        BX_CONFIG_DEBUG=0
)

add_library(bimg STATIC IMPORTED GLOBAL)
set_target_properties(bimg PROPERTIES
    IMPORTED_LOCATION
        ${BGFX_ROOT}/lib/libbimgRelease.a
    INTERFACE_INCLUDE_DIRECTORIES
        ${BGFX_ROOT}/include
)

include(FetchContent)

# --- GLFW ---
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.5.1
)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# --- BS::thread_pool ---
FetchContent_Declare(
        thread_pool
        GIT_REPOSITORY https://github.com/bshoshany/thread-pool.git
        GIT_TAG        v5.1.0
)
FetchContent_MakeAvailable(thread_pool)

add_executable(${PROJECT_NAME}
        src/game/main.cpp
        src/engine/shader_program.cpp
        src/engine/material.cpp
        src/engine/texture.cpp
        src/engine/stb_image.cpp
        src/engine/application.cpp
        include/engine/application.h
        src/engine/mesh.cpp
        include/engine/mesh.h
        src/game/camera.cpp
        include/game/camera.h
        src/game/Chunk.cpp
        include/game/chunk.h
        src/game/chunk_builder.cpp
        include/game/chunk_builder.h
        include/game/block_type.h
        include/game/vertex.h
        include/game/chunk_mesh.h
        src/game/world.cpp
        include/game/world.h
        src/game/chunk_streamer.cpp
        include/game/chunk_streamer.h
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src ${thread_pool_SOURCE_DIR}/include include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    bgfx
    bx
    bimg
    glfw
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    "-framework Cocoa"
    "-framework Metal"
    "-framework QuartzCore"
    "-framework IOKit"
    "-framework CoreMedia"
    "-framework VideoToolbox"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders/bin
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
    COMMENT "Copying shader binaries to output directory"
)