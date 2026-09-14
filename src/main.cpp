#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <include/stb_image.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "mesh.h"
#include "include/application.h"

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

#include "include/camera.h"
#include "include/shader_program.h"
#include "material.h"

struct Vertex {
    float x, y, z;
    float r, g, b;
    float u, v;
};

class MinecraftClone : public application {
public:
    Camera camera;
    bgfx::VertexLayout layout;
    bgfx::VertexBufferHandle vbHandle = BGFX_INVALID_HANDLE;
    std::unique_ptr<Material> material;
    Mesh mesh;

    double lastMouseX = 0.0, lastMouseY = 0.0;
    bool firstMouse = true;

    std::vector<Vertex> triangle = {
        // Front face
        Vertex { -1.0f, 0.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f },
        Vertex {  1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f },
        Vertex {  0.0f, 1.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f },

        // Right face
        Vertex {  1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
        Vertex {  1.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f },
        Vertex {  0.0f, 1.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f },

        // Back face
        Vertex {  1.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f },
        Vertex { -1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f },
        Vertex {  0.0f, 1.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f },

        // Left face
        Vertex { -1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f },
        Vertex { -1.0f, 0.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f },
        Vertex {  0.0f, 1.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f },

        // Base triangle 1
        Vertex { -1.0f, 0.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
        Vertex {  1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f },
        Vertex {  1.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f },

        // Base triangle 2
        Vertex { -1.0f, 0.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
        Vertex {  1.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f },
        Vertex { -1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f },
    };

protected:
    void processInput(GLFWwindow* window, Camera& camera, float deltaTime) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            shouldClose = true;

        const float moveSpeed = 5.0f * deltaTime;
        const float lookSpeed = 0.0025f;

        bx::Vec3 forward = camera.getForward();
        bx::Vec3 right   = camera.getRight();

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.position = bx::add(camera.position, bx::mul(forward, moveSpeed));
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.position = bx::sub(camera.position, bx::mul(forward, moveSpeed));
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.position = bx::add(camera.position, bx::mul(right, moveSpeed));
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.position = bx::sub(camera.position, bx::mul(right, moveSpeed));

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        if (firstMouse) {
            lastMouseX = mouseX;
            lastMouseY = mouseY;
            firstMouse = false;
        }

        float deltaX = float(mouseX - lastMouseX);
        float deltaY = float(mouseY - lastMouseY);
        lastMouseX = mouseX;
        lastMouseY = mouseY;

        camera.yaw   += deltaX * lookSpeed;
        camera.pitch -= deltaY * lookSpeed;

        // Clamp pitch to avoid flipping over at the poles
        const float limit = bx::kPiHalf - 0.01f;
        camera.pitch = bx::clamp(camera.pitch, -limit, limit);
    }

    void onInit() override {
        layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .end();

        mesh.upload(triangle.data(), triangle.size(), layout);
        material = std::make_unique<Material>("test.png", "triangle");
    }

    void onUpdate(float dt) override {
        processInput(window, camera, dt);
    }

    void onRender() override {
        camera.getViewMatrix();
        camera.getProjMatrix(true);
        bgfx::setViewTransform(kMainView, camera.viewMatrix.data(), camera.projMatrix.data());

        bgfx::touch(kMainView);

        mesh.stage(kMainView);
        material->bind();
        bgfx::submit(kMainView, material->program.handle());
    }

    void onShutdown() override {
        if (bgfx::isValid(vbHandle)) {
            bgfx::destroy(vbHandle);
            vbHandle = BGFX_INVALID_HANDLE;
        }
        material.reset();
        mesh.destroy();
    }
};

int main() {
    MinecraftClone game;
    game.init();
    game.run();
    game.shutdown();
    return 0;
}