#pragma once
#include "bgfx/bgfx.h"

class Mesh {
public:
    void upload(const void* vertexData,
                uint32_t vertexCount,
                const bgfx::VertexLayout& layout,
                const uint16_t* indices = nullptr,
                uint32_t indexCount = 0);
    void destroy();
    void stage(bgfx::ViewId viewId) const;

    ~Mesh() { destroy(); }
private:
    bgfx::VertexBufferHandle vbh = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle ibh = BGFX_INVALID_HANDLE;
    bool hasIndices = false;
};
