#pragma once

#include "../key.hpp"
#include "entity.hpp"
#include "yoshix.h"

// -----------------------------------------------------------------------------

struct SPaddle : SEntity {
    float height;
    float width;
    
    SPaddle();
    SPaddle(gfx::BHandle* paddleMesh);

    void move(EKey key, float leftLimit, float rightLimit);

private:
    float speed;
};

// -----------------------------------------------------------------------------

gfx::BHandle CreatePaddleMesh(gfx::BHandle &material);