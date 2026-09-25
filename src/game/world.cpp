#include "game/world.h"

Chunk& World::getOrCreateChunk(ChunkCoord coord) {
    return m_chunks[coord];
}

Chunk* World::getChunk(ChunkCoord coord) {
    const auto it = m_chunks.find(coord);
    return it != m_chunks.end() ? &it->second : nullptr;
}

void World::removeChunk(ChunkCoord coord) {
    m_chunks.erase(coord);
}

ChunkCoord World::worldToChunkCoord(const bx::Vec3 position) {
    return {
        static_cast<int>(std::floor(position.x / CHUNK_SIZE)),
        static_cast<int>(std::floor(position.y / CHUNK_SIZE)),
        static_cast<int>(std::floor(position.z / CHUNK_SIZE))
    };
}