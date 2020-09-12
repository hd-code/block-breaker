#pragma once

#include "entity.hpp"
#include "yoshix.h"

// -----------------------------------------------------------------------------

enum class EBlockType { BED_ROCK, NORMAL, HARD };

struct SBlock : SEntity {
    EBlockType type;
    unsigned int hits;
    float size;

    bool isBroken();
    void onCollision();
};

// -----------------------------------------------------------------------------

SBlock CreateBlock(gfx::BHandle* blockMesh, EBlockType type, float position[3]);

// -----------------------------------------------------------------------------

gfx::BHandle CreateBlockMesh(gfx::BHandle &material);