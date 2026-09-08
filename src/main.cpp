#include <iostream>
#include <vector>
#include <bx/bx.h>
#include <bgfx/bgfx.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#if BX_PLATFORM_LINUX
    #define GLFW_EXPOSE_NATIVE_X11
    #include <GLFW/glfw3native.h>
#elif BX_PLATFORM_WINDOWS
    #define GLFW_EXPOSE_NATIVE_WIN32
    #include <GLFW/glfw3native.h>
#elif BX_PLATFORM_OSX
    #define GLFW_EXPOSE_NATIVE_COCOA
    #include <GLFW/glfw3native.h>
#endif

struct Vertex {
    float x, y, z;
    float r, g, b;
};

std::vector<Vertex> triangle = {
    Vertex { 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
    Vertex { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f },
    Vertex { 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f },
};

#include <bgfx/bgfx.h>
#include <fstream>
#include <vector>
#include <string>

bgfx::ShaderHandle loadShader(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << path << "\n";
        return BGFX_INVALID_HANDLE;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    // bgfx::copy makes an internal copy bgfx owns and frees itself —
    // safest option so you don't have to manage the buffer's lifetime.
    const bgfx::Memory* mem = bgfx::alloc(static_cast<uint32_t>(size) + 1);
    if (!file.read(reinterpret_cast<char*>(mem->data), size)) {
        std::cerr << "Failed to read shader file: " << path << "\n";
        return BGFX_INVALID_HANDLE;
    }
    mem->data[size] = '\0'; // bgfx expects a null terminator on shader binaries

    return bgfx::createShader(mem);
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    const int width = 1280;
    const int height = 720;

    GLFWwindow* window = glfwCreateWindow(width, height, "Minecraft Clone", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    bgfx::renderFrame();

    bgfx::Init init;
    init.swapChain.nwh = glfwGetCocoaWindow(window);
    init.type = bgfx::RendererType::Count;
    init.swapChain.width = 1280;
    init.swapChain.height = 720;
    init.reset = BGFX_RESET_VSYNC;

    if (!bgfx::init(init)) {
        std::cerr << "Failed to initialize BGFX\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    const bgfx::ViewId kClearView = 0;
    bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
    bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);

    bgfx::VertexLayout layout;
    layout
        .begin() 
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 3, bgfx::AttribType::Float)
        .end();
    
    const bgfx::Memory* mem = bgfx::copy(triangle.data(), static_cast<uint32_t>(triangle.size() * sizeof(Vertex)));
    bgfx::VertexBufferHandle vbHandle = bgfx::createVertexBuffer(mem, layout);

    bgfx::ShaderHandle vsh = loadShader("shaders/bin/vs_triangle.bin");
    bgfx::ShaderHandle fsh = loadShader("shaders/bin/fs_triangle.bin");
    bgfx::ProgramHandle program = bgfx::createProgram(vsh, fsh, true);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        bgfx::touch(kClearView);

        bgfx::setVertexBuffer(0, vbHandle);

        bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS);

        bgfx::submit(kClearView, program);

        bgfx::frame();
    }

    bgfx::shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}