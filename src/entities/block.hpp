#pragma once

#include "entity.hpp"
#include "yoshix.h"

// -----------------------------------------------------------------------------

enum EBlockType { BLOCK_BED_ROCK, BLOCK_NORMAL, BLOCK_HARD };

struct SBlock : SEntity {
    EBlockType type;
    unsigned int hits;
    float size;
};

SBlock createBlock(gfx::BHandle &blockMesh, EBlockType type, float position[3]);

gfx::BHandle createBlockMesh(gfx::BHandle &material);