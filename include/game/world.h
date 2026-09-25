#pragma once
#include <functional>

#include "chunk.h"
#include "bx/math.h"

struct ChunkCoord {
    int x, y, z;
    bool operator==(const ChunkCoord&) const = default;
};

struct ChunkCoordHash {
    size_t operator()(const ChunkCoord& c) const {
        return std::hash<int>()(c.x) ^ (std::hash<int>()(c.z) * 0x9e3779b9);
    }
};

class World {
public:
    Chunk& getOrCreateChunk(ChunkCoord coord);
    Chunk* getChunk(ChunkCoord coord);
    void removeChunk(ChunkCoord coord);

    static ChunkCoord worldToChunkCoord(bx::Vec3 position);
private:
    std::unordered_map<ChunkCoord, Chunk, ChunkCoordHash> m_chunks;
};