#pragma once

#include "../key.hpp"
#include "entity.hpp"
#include "yoshix.h"

// -----------------------------------------------------------------------------

struct SPaddle : SEntity {
    float height;
    float width;
    float speed;

    void move(EKey key, float leftLimit, float rightLimit);
};

// -----------------------------------------------------------------------------

SPaddle CreatePaddle(gfx::BHandle* paddleMesh);

// -----------------------------------------------------------------------------

gfx::BHandle CreatePaddleMesh(gfx::BHandle &material);