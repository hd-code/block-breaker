#pragma once

#include "entity.hpp"
#include "yoshix.h"

// -----------------------------------------------------------------------------

struct SPaddle : SEntity {
    float depth;
    float height;
    float width;
};

SPaddle createPaddle(gfx::BHandle &paddleMesh);

gfx::BHandle createPaddleMesh(gfx::BHandle &material);