#include "game/chunk_streamer.h"

ChunkStreamer::ChunkStreamer(World& world, ChunkBuilder& builder)
    : m_world(world), m_builder(builder) {  }

void ChunkStreamer::update(bx::Vec3 playerPosition, int loadRadius) {
    // First, get player chunk position.
    // Second, iterate through all chunks within the load radius around the player.
    // Third, if a chunk is not being loaded, add it to the loading queue. Otherwise, leave it alone.

    ChunkCoord playerChunk = World::worldToChunkCoord(playerPosition);
    int xMin = playerChunk.x - loadRadius;
    int xMax = playerChunk.x + loadRadius;
    int yMin = playerChunk.y - loadRadius;
    int yMax = playerChunk.y + loadRadius;
    int zMin = playerChunk.z - loadRadius;
    int zMax = playerChunk.z + loadRadius;

    for (int x = xMin; x <= xMax; ++x) {
        for (int y = yMin; y <= yMax; ++y) {
            for (int z = zMin; z <= zMax; ++z) {
                if (!m_loading.contains({x, y, z})) {
                    m_loading.insert({x, y, z});
                    m_world.getOrCreateChunk({x, y, z});

                }
            }
        }
    }
}