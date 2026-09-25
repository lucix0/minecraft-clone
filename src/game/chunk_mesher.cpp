#include "game/chunk_mesher.h"

std::vector<Vertex> ChunkMesher::buildMesh(Chunk& chunk, std::vector<Chunk*> neighbors) {
    std::vector<Vertex> chunkVertices = {};

    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int y = 0; y < CHUNK_SIZE; ++y) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                BlockType currentBlock = chunk.getBlock(x, y, z);

                if (currentBlock == AIR)
                    continue;

                // TODO: Implement blocks with different face textures on one block.
                int i = 0;
                for (auto& face : faces) {
                    if (i == 0) {
                        if (x != CHUNK_SIZE - 1) {
                            if (chunk.getBlock(x+1, y, z) != AIR) {
                                ++i;
                                continue;
                            }
                        }
                    } else if (i == 1) {
                        if (x != 0) {
                            if (chunk.getBlock(x-1, y, z) != AIR) {
                                ++i;
                                continue;
                            }
                        }
                    } else if (i == 2) {
                        if (y != CHUNK_SIZE - 1) {
                            if (chunk.getBlock(x, y+1, z) != AIR) {
                                ++i;
                                continue;
                            }
                        }
                    } else if (i == 3) {
                        if (y != 0) {
                            if (chunk.getBlock(x, y-1, z) != AIR) {
                                ++i;
                                continue;
                            }
                        }
                    } else if (i == 4) {
                        if (z != CHUNK_SIZE - 1) {
                            if (chunk.getBlock(x, y, z+1) != AIR) {
                                ++i;
                                continue;
                            }
                        }
                    } else if (i == 5) {
                        if (z != 0) {
                            if (chunk.getBlock(x, y, z-1) != AIR) {
                                ++i;
                                continue;
                            }
                        }
                    }

                    int textureRowOffset = 0;
                    int textureColumnOffset = 0;

                    switch (currentBlock) {
                        case GRASS:
                            textureRowOffset = 0;
                            textureColumnOffset = 0;
                            break;
                        case DIRT:
                            textureRowOffset = 2;
                            textureColumnOffset = 0;
                            break;
                        case STONE:
                            textureRowOffset = 3;
                            textureColumnOffset = 0;
                            break;
                        case AIR:
                            break;
                    }

                    std::transform(face.begin(), face.end(), std::back_inserter(chunkVertices),
                        [&](Vertex v) {
                            v.x += x;
                            v.y += y;
                            v.z += z;
                            v.u += textureRowOffset / 16.0f;
                            v.v += textureColumnOffset / 16.0f;
                            return v;
                        }
                    );

                    ++i;
                }
            }
        }
    }

    return chunkVertices;
}