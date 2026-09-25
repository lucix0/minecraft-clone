#pragma once
#include <mutex>

#include <BS_thread_pool.hpp>

#include "chunk_generator.h"
#include "chunk_mesher.h"
#include "chunk_renderer.h"
#include "world.h"
#include "bx/math.h"

struct CompletedTask {
    enum class Kind { Generated, Meshed };
    ChunkCoord coord;
    Kind kind;
    std::vector<Vertex> vertices;
};

class ChunkStreamer {
public:
    ChunkStreamer(World& world, ChunkMesher& builder, ChunkGenerator& generator, ChunkRenderer& renderer);

    void update(bx::Vec3 playerPosition, int loadRadius);

private:
    World& m_world;
    ChunkMesher& m_builder;
    ChunkGenerator& m_generator;
    ChunkRenderer& m_renderer;

    std::mutex m_completedMutex;
    std::vector<CompletedTask> m_completed{};

    BS::thread_pool<> m_pool;

    std::unordered_map<ChunkCoord, ChunkState, ChunkCoordHash> m_states;

    void drain();
    void dispatchGeneration(ChunkCoord coord);
    void dispatchMeshing(ChunkCoord coord);
};
