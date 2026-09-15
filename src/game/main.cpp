#include <iostream>
#include <vector>
#include <fstream>
#include <bx/bx.h>
#include <bgfx/bgfx.h>

#include "engine/mesh.h"
#include "engine/application.h"
#include "engine/material.h"

#include "game/camera.h"
#include "game/chunk.h"
#include "game/chunk_builder.h"
#include "game/chunk_mesh.h"

class MinecraftClone : public application {
public:
    Camera camera;
    bgfx::VertexLayout layout;
    std::unique_ptr<Material> material;
    ChunkMesh mesh;

    double lastMouseX = 0.0, lastMouseY = 0.0;
    bool firstMouse = true;

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

        Chunk chunk;
        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int y = 0; y < CHUNK_SIZE; y++) {
                for (int z = 0; z < CHUNK_SIZE; z++) {
                    chunk.setBlock(x, y, z, STONE);
                }
            }
        }

        auto chunkVertices = ChunkBuilder::generateMesh(chunk);
        mesh.mesh.upload(chunkVertices.data(), chunkVertices.size(), layout);

        material = std::make_unique<Material>("test.png", "triangle");
        material->render_state = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS;
    }

    void onUpdate(float dt) override {
        processInput(window, camera, dt);
    }

    void onRender() override {
        camera.getViewMatrix();
        camera.getProjMatrix(true);
        bgfx::setViewTransform(kMainView, camera.viewMatrix.data(), camera.projMatrix.data());

        mesh.mesh.stage(kMainView);
        material->bind();
        bgfx::submit(kMainView, material->program.handle());
    }

    void onShutdown() override {
        material.reset();
        mesh.mesh.destroy();
    }
};

int main() {
    MinecraftClone game;
    if (!game.init()) return 1;
    game.run();
    game.shutdown();
    return 0;
}