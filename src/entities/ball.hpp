#pragma once

#include "entity.hpp"
#include "yoshix.h"

// -----------------------------------------------------------------------------

struct SBall : SEntity {
    float radius = 0.8f;
};

gfx::BHandle createBallMesh(gfx::BHandle &material);