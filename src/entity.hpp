#pragma once

// -----------------------------------------------------------------------------

#include "yoshix.h"

// -----------------------------------------------------------------------------

struct SEntity {
    gfx::BHandle* mesh;
    float position[3];
    float worldMatrix[16];
};

void updateWorldMatrix(SEntity &entity);