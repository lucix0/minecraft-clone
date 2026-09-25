#include "game/chunk_mesher.h"

std::vector<Vertex> ChunkMesher::buildMesh(Chunk& chunk) {
    std::vector<Vertex> chunkVertices = {};

    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int y = 0; y < CHUNK_SIZE; ++y) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                BlockType currentBlock = chunk.getBlock(x, y, z);

                if (currentBlock == AIR)
                    continue;

                for (auto& face : faces) {
                    std::transform(face.begin(), face.end(), std::back_inserter(chunkVertices),
                        [&](Vertex v) {
                            v.x += x;
                            v.y += y;
                            v.z += z;
                            return v;
                        }
                    );
                }
            }
        }
    }

    return chunkVertices;
}