#include "engine/Application.h"
#include <iostream>

bool Application::init(int width, int height, const std::string& name) {
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

    m_dRender = std::make_unique<DeferredRenderer>();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    onInit();

    return true;
}

void Application::run() {
    double lastFrameTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        if (shouldClose) glfwSetWindowShouldClose(window, true);

        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastFrameTime);
        lastFrameTime = currentTime;

        bgfx::touch(m_dRender->geometryView());

        onUpdate(deltaTime);

        onRender();

        m_dRender->render();

        bgfx::frame();
    }
}

void Application::shutdown() {
    onShutdown();

    m_dRender.reset();

    bgfx::shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
}
