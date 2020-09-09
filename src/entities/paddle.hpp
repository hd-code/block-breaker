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

SPaddle createPaddle(gfx::BHandle* paddleMesh);

gfx::BHandle createPaddleMesh(gfx::BHandle &material);