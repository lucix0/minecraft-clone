#pragma once

#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <bgfx/bgfx.h>
#include <bx/bx.h>

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

class application {
public:
    bool init(int width = 1280, int height = 720, const std::string &name = "application");
    void run();
    void shutdown();
protected:
    static constexpr bgfx::ViewId kMainView = 0;

    virtual void onInit() = 0;
    virtual void onUpdate(float dt) = 0;
    virtual void onRender() = 0;
    virtual void onShutdown() = 0;

    GLFWwindow* window = nullptr;
    bool shouldClose = false;
};