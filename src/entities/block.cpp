#include "block.hpp"

// -----------------------------------------------------------------------------

const float BLOCK_SIZE = 1.0f;

ETexture getBlockTexture(EBlockType type) {
    switch (type) {
        case BLOCK_BED_ROCK: return TEX_BED_ROCK;
        case BLOCK_NORMAL:   return TEX_BLOCK;
        case BLOCK_HARD:     return TEX_BLOCK_HARD;
        default: return TEX_BED_ROCK;
    }
}

SBlock createBlock(gfx::BHandle &blockMesh, EBlockType type, float position[3]) {
    SBlock block;

    block.mesh = blockMesh;
    block.texture = getBlockTexture(type);

    block.position[0] = position[0];
    block.position[1] = position[1];
    block.position[2] = position[2];

    block.hits = 0;
    block.size = BLOCK_SIZE;
    block.type = type;

    return block;
}

gfx::BHandle createBlockMesh(gfx::BHandle &material) {
    float e = BLOCK_SIZE / 2.0f; // half-edge length

    float vertices[][8] = {
        // front
        { -e, e,-e,    0.0f, 0.0f,-1.0f,   0.0f, 0.0f },
        { -e,-e,-e,    0.0f, 0.0f,-1.0f,   0.0f, 1.0f },
        {  e,-e,-e,    0.0f, 0.0f,-1.0f,   1.0f, 1.0f },
        {  e, e,-e,    0.0f, 0.0f,-1.0f,   1.0f, 0.0f },
        // back
        { -e,-e, e,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f },
        { -e, e, e,    0.0f, 0.0f, 1.0f,   0.0f, 1.0f },
        {  e, e, e,    0.0f, 0.0f, 1.0f,   1.0f, 1.0f },
        {  e,-e, e,    0.0f, 0.0f, 1.0f,   1.0f, 0.0f },
        // top
        { -e, e, e,    0.0f, 1.0f, 0.0f,   0.0f, 0.0f },
        { -e, e,-e,    0.0f, 1.0f, 0.0f,   0.0f, 1.0f },
        {  e, e,-e,    0.0f, 1.0f, 0.0f,   1.0f, 1.0f },
        {  e, e, e,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f },
        // bottom
        { -e,-e,-e,    0.0f,-1.0f, 0.0f,   0.0f, 0.0f },
        { -e,-e, e,    0.0f,-1.0f, 0.0f,   0.0f, 1.0f },
        {  e,-e, e,    0.0f,-1.0f, 0.0f,   1.0f, 1.0f },
        {  e,-e,-e,    0.0f,-1.0f, 0.0f,   1.0f, 0.0f },
        // right
        {  e, e,-e,    1.0f, 0.0f, 0.0f,   0.0f, 0.0f },
        {  e,-e,-e,    1.0f, 0.0f, 0.0f,   0.0f, 1.0f },
        {  e,-e, e,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f },
        {  e, e, e,    1.0f, 0.0f, 0.0f,   1.0f, 0.0f },
        // left
        { -e, e, e,   -1.0f, 0.0f, 0.0f,   0.0f, 0.0f },
        { -e,-e, e,   -1.0f, 0.0f, 0.0f,   0.0f, 1.0f },
        { -e,-e,-e,   -1.0f, 0.0f, 0.0f,   1.0f, 1.0f },
        { -e, e,-e,   -1.0f, 0.0f, 0.0f,   1.0f, 0.0f },
    };

    int triangles[][3] = {
        {  0, 1, 2 }, {  0, 2, 3 },
        {  4, 5, 6 }, {  4, 6, 7 },
        {  8, 9,10 }, {  8,10,11 },
        { 12,13,14 }, { 12,14,15 },
        { 16,17,18 }, { 16,18,19 },
        { 20,21,22 }, { 20,22,23 },
    };

    return createMesh(24, &vertices[0][0], 12, &triangles[0][0], material);
}