#include "game/chunk_generator.h"

ChunkGenerator::ChunkGenerator() {
    m_noise = FastNoiseLite{m_seed};
    m_noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    m_noise.SetFrequency(0.01f);

    m_noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    m_noise.SetFractalOctaves(4);
    m_noise.SetFractalLacunarity(2.0f);
    m_noise.SetFractalGain(0.5f);
}

void ChunkGenerator::generateChunk(Chunk& chunk, ChunkCoord coord) const {
    for (int z = 0; z < CHUNK_SIZE; ++z) {
        int zWorld = z + coord.z * CHUNK_SIZE;

        for (int x = 0; x < CHUNK_SIZE; ++x) {
            int xWorld = x + coord.x * CHUNK_SIZE;
            auto n = m_noise.GetNoise(static_cast<float>(xWorld), static_cast<float>(zWorld));
            int height = static_cast<int>(std::floor(n * m_nHeightRange) + m_baseHeight);

            for (int y = 0; y < CHUNK_SIZE; ++y) {
                int yWorld = y + coord.y * CHUNK_SIZE;

                int depth = height - yWorld;

                BlockType block;
                if (depth < 0) {
                    block = AIR;
                } else if (depth == 0) {
                    block = GRASS;
                } else if (depth < 3) {
                    block = DIRT;
                } else {
                    block = STONE;
                }

                chunk.setBlock(x, y, z, block);
            }
        }
    }
}
