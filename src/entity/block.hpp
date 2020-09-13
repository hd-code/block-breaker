#pragma once

#include "entity.hpp"
#include "yoshix.h"

// -----------------------------------------------------------------------------

enum class EBlockType { BED_ROCK, NORMAL, HARD };

struct SBlock : SEntity {
    float size;
    
    SBlock();
    SBlock(gfx::BHandle* blockMesh, EBlockType type, float position[3]);

    bool isBroken();
    void onCollision();

private:
    EBlockType type;
    unsigned int hits;
};

// -----------------------------------------------------------------------------

gfx::BHandle CreateBlockMesh(gfx::BHandle &material);