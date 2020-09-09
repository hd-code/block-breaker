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

    bool handleCollision(SBlock &block);
    bool handleCollision(const SPaddle &paddle);

    bool isOnGround(float groundLevel);

    void move();

private:
    void changeDirection(bool horizontalCollision);
};

SBall createBall(gfx::BHandle* ballMesh);

gfx::BHandle createBallMesh(gfx::BHandle &material);