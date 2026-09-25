#pragma once
#include <unordered_map>
#include <span>

#include "vertex.h"
#include "world.h"
#include "engine/material.h"

class ChunkRenderer {
public:
    ChunkRenderer();
    ~ChunkRenderer();

    void upload(ChunkCoord coord, std::span<const Vertex> vertices);
    void unload(ChunkCoord coord);

    void renderAll(bgfx::ViewId view) const;
private:
    std::unique_ptr<Material> m_material;
    bgfx::VertexLayout m_layout;
    std::unordered_map<ChunkCoord, bgfx::VertexBufferHandle, ChunkCoordHash> m_meshes;
};
