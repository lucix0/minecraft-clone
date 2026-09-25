#include "game/chunk_streamer.h"

ChunkStreamer::ChunkStreamer(World& world, ChunkMesher& builder, ChunkGenerator& generator, ChunkRenderer& renderer)
    : m_world(world), m_builder(builder), m_generator(generator), m_renderer(renderer) {  }

void ChunkStreamer::update(bx::Vec3 playerPosition, int loadRadius) {
    drain();

    int unRadiusDelta = 0;

    ChunkCoord playerChunk = World::worldToChunkCoord(playerPosition);
    int xMin = playerChunk.x - loadRadius;
    int xMax = playerChunk.x + loadRadius;
    int yMin = playerChunk.y - loadRadius;
    int yMax = playerChunk.y + loadRadius;
    int zMin = playerChunk.z - loadRadius;
    int zMax = playerChunk.z + loadRadius;

    std::vector<ChunkCoord> queuedCoords{};

    // Add chunks to m_states within loading range.
    for (int x = xMin; x <= xMax; ++x) {
        for (int y = yMin; y <= yMax; ++y) {
            for (int z = zMin; z <= zMax; ++z) {
                if (!m_states.contains({x, y, z})) {
                    m_states.insert({{x, y, z}, ChunkState::Queued});
                    queuedCoords.push_back({x, y, z});
                }
            }
        }
    }

    // Remove any chunks outside of loading radius
    std::erase_if(m_states, [&](const auto& entry) {
        const auto& [coord, state] = entry;
        if ((coord.x < xMin - unRadiusDelta || coord.x > xMax + unRadiusDelta ||
             coord.y < yMin - unRadiusDelta || coord.y > yMax + unRadiusDelta ||
             coord.z < zMin - unRadiusDelta || coord.z > zMax + unRadiusDelta) && (state == ChunkState::Queued || state == ChunkState::Generated || state == ChunkState::Ready)) {
            // Clean up other data related to this chunk that's to be removed
            m_world.removeChunk(coord);
            m_renderer.unload(coord);

            return true;
        }

        return false;
    });

    for (auto c : queuedCoords) {
        dispatchGeneration(c);
    }

    for (const auto& s : m_states) {
        if (s.second == ChunkState::Generated)
            dispatchMeshing(s.first);
    }
}

void ChunkStreamer::drain() {
    std::vector<CompletedTask> done;
    {
        std::lock_guard lock(m_completedMutex);
        done.swap(m_completed);
    }

    for (auto& task : done) {
        if (task.kind == CompletedTask::Kind::Generated) {
            m_states.at(task.coord) = ChunkState::Generated;
        } else {
            m_renderer.upload(task.coord, task.vertices);
            m_states.at(task.coord) = ChunkState::Ready;
        }
    }
}

void ChunkStreamer::dispatchGeneration(ChunkCoord coord) {
    Chunk* chunk = &m_world.getOrCreateChunk(coord);
    m_states.at(coord) = ChunkState::Generating;

    m_pool.detach_task([this, coord, chunk] {
        m_generator.generateChunk(*chunk, coord);

        std::lock_guard lock(m_completedMutex);
        m_completed.push_back({coord, CompletedTask::Kind::Generated});
    });
}

void ChunkStreamer::dispatchMeshing(ChunkCoord coord) {
    Chunk* chunk = &m_world.getOrCreateChunk(coord);
    m_states.at(coord) = ChunkState::Meshing;

    // Get neighbor chunks for chunk face calculations.
    std::vector<Chunk*> neighbors{};
    neighbors.push_back(&m_world.getOrCreateChunk({coord.x+1, coord.y, coord.z}));
    neighbors.push_back(&m_world.getOrCreateChunk({coord.x-1, coord.y, coord.z}));
    neighbors.push_back(&m_world.getOrCreateChunk({coord.x, coord.y+1, coord.z}));
    neighbors.push_back(&m_world.getOrCreateChunk({coord.x, coord.y-1, coord.z}));
    neighbors.push_back(&m_world.getOrCreateChunk({coord.x, coord.y, coord.z+1}));
    neighbors.push_back(&m_world.getOrCreateChunk({coord.x, coord.y, coord.z-1}));

    m_pool.detach_task([this, coord, chunk, neighbors] {
        std::vector<Vertex> verts = m_builder.buildMesh(*chunk, neighbors);

        std::lock_guard lock(m_completedMutex);
        m_completed.push_back({coord, CompletedTask::Kind::Meshed, std::move(verts)});
    });
}
