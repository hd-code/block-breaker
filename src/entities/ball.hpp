#pragma once

#include "entity.hpp"
#include "yoshix.h"

// -----------------------------------------------------------------------------

struct SBall : SEntity {
    float direction[3];
    float radius;
    float speed;

    void changeDirection(ECollision collision);
    void move();
};

SBall createBall(gfx::BHandle ballMesh, float position[3]);

gfx::BHandle createBallMesh(gfx::BHandle &material);