#include "game/camera.h"

void Camera::getViewMatrix() {
    bx::Vec3 forward = getForward();
    bx::Vec3 at = bx::add(position, forward);
    bx::Vec3 up = { 0.0f, 1.0f, 0.0f };
    bx::mtxLookAt(viewMatrix.data(), position, at, up);
}

void Camera::getProjMatrix(bool homogenousDepth) {
    bx::mtxProj(projMatrix.data(), fov, aspectRatio, nearPlane, farPlane, homogenousDepth);
}