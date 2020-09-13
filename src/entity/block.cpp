#include "block.hpp"

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

SBlock::SBlock() {}

SBlock::SBlock(gfx::BHandle* blockMesh, EBlockType type, float position[3]) {
    this->mesh = blockMesh;
    this->texture = getBlockTexture(type);
    this->specularExponent = SPEC_EXP;

    this->position[0] = position[0];
    this->position[1] = position[1];
    this->position[2] = position[2];

    this->hits = 0;
    this->size = SIZE;
    this->type = type;

    this->updateWorldMatrix();
}

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

// --- Mesh --------------------------------------------------------------------

gfx::BHandle CreateBlockMesh(gfx::BHandle &material) {
    return CreateCuboidMesh(material, SIZE, SIZE, SIZE);
}