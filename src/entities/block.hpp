#pragma once

#include "entity.hpp"
#include "yoshix.h"

// -----------------------------------------------------------------------------

enum EBlockType { BED_ROCK, NORMAL, HARD };

struct SBlock : SEntity {
    EBlockType type;
    unsigned int hits;
    float size;

    void onCollision();
};

SBlock createBlock(gfx::BHandle &blockMesh, EBlockType type, float position[3]);

gfx::BHandle createBlockMesh(gfx::BHandle &material);