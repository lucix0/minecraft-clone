#include <bx/bx.h>
#include <bgfx/bgfx.h>

#include "engine/application.h"

#include "game/camera.h"
#include "game/chunk_mesher.h"
#include "game/chunk_renderer.h"
#include "game/chunk_streamer.h"
#include "game/world.h"

class MinecraftClone : public application {
public:
    Camera camera;
    World world;

    ChunkMesher cBuilder;
    ChunkGenerator cGenerator;
    std::unique_ptr<ChunkRenderer> cRenderer;
    std::unique_ptr<ChunkStreamer> cStreamer;

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
        bgfx::setDebug(BGFX_DEBUG_STATS);

        cRenderer = std::make_unique<ChunkRenderer>();
        cStreamer = std::make_unique<ChunkStreamer>(world, cBuilder, cGenerator, *cRenderer);
    }

    void onUpdate(float dt) override {
        processInput(window, camera, dt);

        cStreamer->update(camera.position, 2);
    }

    void onRender() override {
        camera.getViewMatrix();
        camera.getProjMatrix(true);
        bgfx::setViewTransform(kMainView, camera.viewMatrix.data(), camera.projMatrix.data());

        cRenderer->renderAll(kMainView);
    }

    void onShutdown() override {

    }
};

int main() {
    MinecraftClone game;
    if (!game.init()) return 1;
    game.run();
    game.shutdown();
    return 0;
}