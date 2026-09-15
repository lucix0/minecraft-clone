#include "game/chunk.h"
#include <cassert>

void Chunk::setBlock(uint8_t x, uint8_t y, uint8_t z, BlockType blockType) {
    blocks[coordToIndex(x, y, z)] = blockType;
    dirty = true;
}

BlockType Chunk::getBlock(uint8_t x, uint8_t y, uint8_t z) const {
    assert(x < CHUNK_SIZE && y < CHUNK_SIZE && z < CHUNK_SIZE);
    return blocks[coordToIndex(x, y, z)];
}

int Chunk::coordToIndex(uint8_t x, uint8_t y, uint8_t z) {
    assert(x < CHUNK_SIZE && y < CHUNK_SIZE && z < CHUNK_SIZE);
    return x + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_SIZE);
}

void Chunk::indexToCoord(int index, uint8_t& x, uint8_t& y, uint8_t& z) {
    x = index % CHUNK_SIZE;
    y = (index / CHUNK_SIZE) % CHUNK_SIZE;
    z = index / (CHUNK_SIZE * CHUNK_SIZE);
}