#pragma once
#include <BS_thread_pool.hpp>
#include <unordered_set>

#include "chunk_builder.h"
#include "world.h"
#include "bx/math.h"

class ChunkStreamer {
public:
    ChunkStreamer(World& world, ChunkBuilder& builder);

    void update(bx::Vec3 playerPosition, int loadRadius);

private:
    World& m_world;
    ChunkBuilder& m_builder;
    BS::thread_pool<> m_pool;

    std::unordered_set<ChunkCoord, ChunkCoordHash> m_loading;
};
