#pragma once
#include <vector>
#include "vertex.h"
#include "chunk.h"

// +X face (right)
inline std::array<Vertex, 6> faceRight = {
    {
        { 1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f },
        { 1.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f },
        { 1.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f },
        { 1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f },
    }
};

// -X face (left)
inline std::array<Vertex, 6> faceLeft = {
    {
        { 0.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f },
        { 0.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f },
        { 0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f },
        { 0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f },
        { 0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f },
    }
};

// +Y face (top)
inline std::array<Vertex, 6> faceTop = {
    {
        { 0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f },
        { 0.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f },
        { 1.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f },
    }
};

// -Y face (bottom)
inline std::array<Vertex, 6> faceBottom = {
    {
        { 0.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f },
        { 1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f },
        { 1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f },
        { 1.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f },
    }
};

// +Z face (front)
inline std::array<Vertex, 6> faceFront = {
    {
        { 0.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f },
        { 1.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f },
        { 0.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f },
    }
};

// -Z face (back)
inline std::array<Vertex, 6> faceBack = {
    {
        { 1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f },
        { 0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f },
        { 1.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f },
        { 1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f },
    }
};

inline std::array faces = { faceRight, faceLeft, faceTop, faceBottom, faceFront, faceBack };

class ChunkMesher {
public:
    static std::vector<Vertex> buildMesh(Chunk& chunk);
};