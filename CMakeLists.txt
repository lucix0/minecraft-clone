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

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
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