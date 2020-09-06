#include "paddle.hpp"

// -----------------------------------------------------------------------------

const float PADDLE_HEIGHT = 0.5f;
const float PADDLE_WIDTH  = 2.0f;

SPaddle createPaddle(gfx::BHandle &paddleMesh, float position[3]) {
    SPaddle paddle;

    paddle.mesh = paddleMesh;
    paddle.texture = TEX_PADDLE;

    paddle.position[0] = position[0];
    paddle.position[1] = position[1];
    paddle.position[2] = position[2];

    paddle.height = PADDLE_HEIGHT;
    paddle.width  = PADDLE_WIDTH;

    return paddle;
}

gfx::BHandle createPaddleMesh(gfx::BHandle &material);