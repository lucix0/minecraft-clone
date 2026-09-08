#include <iostream>
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

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        bgfx::touch(kClearView);

        bgfx::frame();
    }

    bgfx::shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}