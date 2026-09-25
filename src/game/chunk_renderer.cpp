#include "game/chunk_renderer.h"
#include "bx/math.h"
#include <assert.h>

ChunkRenderer::ChunkRenderer() {
    m_layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .end();

    assert(m_layout.getStride() == sizeof(Vertex));

    // TODO: Replace texture and shader program with actual chunk-specific ones
    m_material = std::make_unique<Material>("block_textures.png", "triangle");
    m_material->render_state = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS;
}

ChunkRenderer::~ChunkRenderer() {
    for (const auto& p : m_meshes)
        bgfx::destroy(p.second);
}

void ChunkRenderer::upload(ChunkCoord coord, std::span<const Vertex> vertices) {
    if (auto p = m_meshes.find(coord); p != m_meshes.end()) {
        bgfx::destroy(p->second);
    }

    const bgfx::Memory* mem = bgfx::copy(vertices.data(), vertices.size_bytes());
    m_meshes.insert_or_assign(coord, bgfx::createVertexBuffer(mem, m_layout));
}

void ChunkRenderer::unload(ChunkCoord coord) {
    if (auto p = m_meshes.find(coord); p != m_meshes.end()) {
        bgfx::destroy(p->second);
        m_meshes.erase(p);
    }
}

void ChunkRenderer::renderAll(bgfx::ViewId view) const {
    for (auto& [coord, vbh] : m_meshes) {
        // Transform mesh to correct world position
        std::array<float, 16> transform;
        bx::mtxTranslate(transform.data(),
            static_cast<float>(coord.x*CHUNK_SIZE),
            static_cast<float>(coord.y*CHUNK_SIZE),
            static_cast<float>(coord.z*CHUNK_SIZE));

        bgfx::setTransform(transform.data());
        bgfx::setVertexBuffer(0, vbh);

        m_material->bind();

        bgfx::submit(view, m_material->program.handle());
    }
}
