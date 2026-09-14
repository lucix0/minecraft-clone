#include "include/application.h"
#include <iostream>

bool application::init(int width, int height, const std::string& name) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return false;
    }

    bgfx::renderFrame();

    bgfx::Init init;

#if BX_PLATFORM_LINUX
    init.swapChain.ndt = glfwGetX11Display();
    init.swapChain.nwh = (void*)(uintptr_t)glfwGetX11Window(window);
#elif BX_PLATFORM_WINDOWS
    init.swapChain.nwh = glfwGetWin32Window(window);
#elif BX_PLATFORM_OSX
    init.swapChain.nwh = glfwGetCocoaWindow(window);
#endif

    init.type = bgfx::RendererType::Count;
    init.swapChain.width = width;
    init.swapChain.height = height;
    init.reset = BGFX_RESET_VSYNC;

    if (!bgfx::init(init)) {
        std::cerr << "Failed to initialize BGFX\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }

    const bgfx::ViewId kClearView = 0;
    bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
    bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);

    onInit();
}

void application::run() {
    double lastFrameTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        if (shouldClose) {
            glfwSetWindowShouldClose(window, true);
        }

        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastFrameTime);
        lastFrameTime = currentTime;

        onUpdate(deltaTime);
        onRender();

        bgfx::frame();
    }
}

void application::shutdown() {
    onShutdown();

    bgfx::shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
}
