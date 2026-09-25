#pragma once

#include "world.h"
#include "FastNoiseLite.h"

class ChunkGenerator {
public:
    ChunkGenerator();
    void generateChunk(Chunk& chunk, ChunkCoord coord) const;
private:
    int32_t m_seed{0x7EDC'BA98};
    int m_baseHeight{72};
    int m_nHeightRange{32};

    FastNoiseLite m_noise;
};
