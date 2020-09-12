#include "block.hpp"

#include "../helper/mesh.hpp"

// -----------------------------------------------------------------------------

bool SBlock::isBroken() {
    switch (this->type) {
    case EBlockType::BED_ROCK:
        return false;

    case EBlockType::NORMAL:
        return this->hits >= 1;

    case EBlockType::HARD:
        return this->hits >= 2;
    }

    return false;
}

void SBlock::onCollision() {
    this->hits++;

    if (this->type == EBlockType::HARD) {
        this->texture = ETexture::BLOCK_CRACKED;
    }
}

// -----------------------------------------------------------------------------

const float SPEC_EXP = 40.0f;

const float SIZE = 1.0f;

ETexture getBlockTexture(EBlockType type) {
    switch (type) {
        case EBlockType::BED_ROCK: return ETexture::BED_ROCK;
        case EBlockType::NORMAL:   return ETexture::BLOCK;
        case EBlockType::HARD:     return ETexture::BLOCK_HARD;
        default: return ETexture::BED_ROCK;
    }
}

SBlock CreateBlock(gfx::BHandle* blockMesh, EBlockType type, float position[3]) {
    SBlock block;
    block.mesh = blockMesh;
    block.texture = getBlockTexture(type);
    block.specularExponent = SPEC_EXP;

    block.position[0] = position[0];
    block.position[1] = position[1];
    block.position[2] = position[2];

    block.hits = 0;
    block.size = SIZE;
    block.type = type;

    block.updateWorldMatrix();

    return block;
}

// --- Mesh --------------------------------------------------------------------

gfx::BHandle CreateBlockMesh(gfx::BHandle &material) {
    float e = SIZE / 2.0f; // half-edge length

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

    return CreateMesh(24, &vertices[0][0], 12, &triangles[0][0], material);
}