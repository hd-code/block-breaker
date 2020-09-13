#pragma once

#include "block.hpp"
#include "entity.hpp"
#include "paddle.hpp"
#include "yoshix.h"

// -----------------------------------------------------------------------------

struct SBall : SEntity {
    SBall();
    SBall(gfx::BHandle* ballMesh);

    bool handleCollision(float topBorder, float leftBorder, float rightBorder);
    bool handleCollision(SBlock &block);
    bool handleCollision(const SPaddle &paddle);

    bool isOnGround(float groundLevel);

    void move();

private:
    float direction[3];
    float radius;
    float speed;

    enum class ECollisionAt { TOP, BOTTOM, LEFT, RIGHT };
    void changeDirection(ECollisionAt collisionAt);
};

// -----------------------------------------------------------------------------

gfx::BHandle CreateBallMesh(gfx::BHandle &material);