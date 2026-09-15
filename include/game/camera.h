#pragma once
#include <bx/math.h>
#include <array>

struct Camera {
    bx::Vec3 position = { 0.0f, 0.0f, -5.0f };
    float yaw = 0.0f, pitch = 0.0f, fov = 90.0f, aspectRatio = 16.0f / 9.0f, nearPlane = 0.1f, farPlane = 100.0f;
    std::array<float, 16> viewMatrix{}, projMatrix{};

    bx::Vec3 getForward() const {
        return {
            bx::cos(pitch) * bx::sin(yaw),
            bx::sin(pitch),
            bx::cos(pitch) * bx::cos(yaw)
        };
    }

    bx::Vec3 getRight() const {
        return { bx::cos(yaw), 0.0f, -bx::sin(yaw) };
    }

    void getViewMatrix();

    void getProjMatrix(bool homogenousDepth);
};