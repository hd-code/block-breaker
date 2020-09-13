#include "block.hpp"

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
    return CreateCuboidMesh(material, SIZE, SIZE, SIZE);
}