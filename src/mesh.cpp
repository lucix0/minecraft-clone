#include "mesh.h"

void Mesh::upload(const void* vertexData, uint32_t vertexCount, const bgfx::VertexLayout& layout, const uint16_t* indices, uint32_t indexCount) {
    const bgfx::Memory* mem = bgfx::copy(vertexData, vertexCount * layout.getStride());
    vbh = bgfx::createVertexBuffer(mem, layout);

    hasIndices = indices != nullptr && indexCount > 0;
    if (hasIndices) {
        mem = bgfx::copy(indices, indexCount * sizeof(uint16_t));
        ibh = bgfx::createIndexBuffer(mem);
    }
}

void Mesh::destroy() {
    if (bgfx::isValid(vbh)) {
        bgfx::destroy(vbh);
        vbh = BGFX_INVALID_HANDLE;
    }

    if (bgfx::isValid(ibh)) {
        bgfx::destroy(ibh);
        ibh = BGFX_INVALID_HANDLE;
    }
}

void Mesh::stage(bgfx::ViewId viewId) const {
    bgfx::setVertexBuffer(0, vbh);
    if (hasIndices) bgfx::setIndexBuffer(ibh);
}