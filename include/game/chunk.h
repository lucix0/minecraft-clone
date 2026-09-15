#pragma once
#include <array>
#include <game/block_type.h>

// Chunks are cubic, so only one length is needed.
constexpr uint8_t CHUNK_SIZE = 32;

class Chunk {
public:
    void setBlock(uint8_t x, uint8_t y, uint8_t z, BlockType blockType);
    BlockType getBlock(uint8_t x, uint8_t y, uint8_t z) const;

    static int coordToIndex(uint8_t x, uint8_t y, uint8_t z);
    static void indexToCoord(int index, uint8_t& x, uint8_t& y, uint8_t& z);

    bool isDirty() const { return dirty; }
    void clearDirty() { dirty = false; }
private:
    std::array<BlockType, CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE> blocks = {};
    bool dirty = true;
};
