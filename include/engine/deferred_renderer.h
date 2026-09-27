#pragma once
#include <bgfx/bgfx.h>

#include "shader_program.h"

class DeferredRenderer {
public:
    DeferredRenderer();
    ~DeferredRenderer();

    void render();

    bgfx::ViewId geometryView() const { return m_geometryView; }
private:
    ShaderProgram m_compositeShader;

    bgfx::ViewId m_geometryView = 0;
    bgfx::ViewId m_compositeView = 1;
    bgfx::VertexBufferHandle m_viewTri = BGFX_INVALID_HANDLE;
    bgfx::VertexLayout m_viewTriLayout;
    bgfx::FrameBufferHandle m_gBuffer = BGFX_INVALID_HANDLE;
    bgfx::TextureHandle m_gPosition = BGFX_INVALID_HANDLE;
    bgfx::TextureHandle m_gNormal = BGFX_INVALID_HANDLE;
    bgfx::TextureHandle m_gAlbedo = BGFX_INVALID_HANDLE;
    bgfx::TextureHandle m_depth = BGFX_INVALID_HANDLE;
    bgfx::UniformHandle m_gPositionSampler = BGFX_INVALID_HANDLE;
    bgfx::UniformHandle m_gNormalSampler = BGFX_INVALID_HANDLE;
    bgfx::UniformHandle m_gAlbedoSampler = BGFX_INVALID_HANDLE;
    bgfx::UniformHandle m_depthSampler = BGFX_INVALID_HANDLE;
};
