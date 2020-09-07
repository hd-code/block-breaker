#pragma once

#include "block.hpp"
#include "entity.hpp"
#include "paddle.hpp"
#include "yoshix.h"

// -----------------------------------------------------------------------------

struct SBall : SEntity {
    float direction[3];
    float radius;
    float speed;

    void move();

    void handleCollision(SBlock &block);
    void handleCollision(const SPaddle &paddle);
};

SBall createBall(gfx::BHandle ballMesh, float position[3]);

gfx::BHandle createBallMesh(gfx::BHandle &material);