#include "paddle.hpp"

// -----------------------------------------------------------------------------

const float PADDLE_HEIGHT = 0.5f;
const float PADDLE_WIDTH  = 2.0f;
const float PADDLE_DEPTH  = 1.0f;

SPaddle createPaddle(gfx::BHandle &paddleMesh, float position[3]) {
    SPaddle paddle;

    paddle.mesh = &paddleMesh;
    paddle.texture = TEX_PADDLE;

    paddle.position[0] = position[0];
    paddle.position[1] = position[1];
    paddle.position[2] = position[2];

    paddle.height = PADDLE_HEIGHT;
    paddle.width  = PADDLE_WIDTH;

    return paddle;
}

gfx::BHandle createPaddleMesh(gfx::BHandle &material) {
    float x = PADDLE_WIDTH  / 2.0f; // half width length
    float y = PADDLE_HEIGHT / 2.0f; // half height length
    float z = PADDLE_DEPTH  / 2.0f; // half depth length

    float vertices[][8] = {
        // front
        { -x, y,-z,    0.0f, 0.0f,-1.0f,   0.0f, 0.0f },
        { -x,-y,-z,    0.0f, 0.0f,-1.0f,   0.0f, 1.0f },
        {  x,-y,-z,    0.0f, 0.0f,-1.0f,   1.0f, 1.0f },
        {  x, y,-z,    0.0f, 0.0f,-1.0f,   1.0f, 0.0f },
        // back
        { -x,-y, z,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f },
        { -x, y, z,    0.0f, 0.0f, 1.0f,   0.0f, 1.0f },
        {  x, y, z,    0.0f, 0.0f, 1.0f,   1.0f, 1.0f },
        {  x,-y, z,    0.0f, 0.0f, 1.0f,   1.0f, 0.0f },
        // top
        { -x, y, z,    0.0f, 1.0f, 0.0f,   0.0f, 0.0f },
        { -x, y,-z,    0.0f, 1.0f, 0.0f,   0.0f, 1.0f },
        {  x, y,-z,    0.0f, 1.0f, 0.0f,   1.0f, 1.0f },
        {  x, y, z,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f },
        // bottom
        { -x,-y,-z,    0.0f,-1.0f, 0.0f,   0.0f, 0.0f },
        { -x,-y, z,    0.0f,-1.0f, 0.0f,   0.0f, 1.0f },
        {  x,-y, z,    0.0f,-1.0f, 0.0f,   1.0f, 1.0f },
        {  x,-y,-z,    0.0f,-1.0f, 0.0f,   1.0f, 0.0f },
        // right
        {  x, y,-z,    1.0f, 0.0f, 0.0f,   0.0f, 0.0f },
        {  x,-y,-z,    1.0f, 0.0f, 0.0f,   0.0f, 1.0f },
        {  x,-y, z,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f },
        {  x, y, z,    1.0f, 0.0f, 0.0f,   1.0f, 0.0f },
        // left
        { -x, y, z,   -1.0f, 0.0f, 0.0f,   0.0f, 0.0f },
        { -x,-y, z,   -1.0f, 0.0f, 0.0f,   0.0f, 1.0f },
        { -x,-y,-z,   -1.0f, 0.0f, 0.0f,   1.0f, 1.0f },
        { -x, y,-z,   -1.0f, 0.0f, 0.0f,   1.0f, 0.0f },
    };

    int triangles[][3] = {
        {  0, 1, 2 }, {  0, 2, 3 },
        {  4, 5, 6 }, {  4, 6, 7 },
        {  8, 9,10 }, {  8,10,11 },
        { 12,13,14 }, { 12,14,15 },
        { 16,17,18 }, { 16,18,19 },
        { 20,21,22 }, { 20,22,23 },
    };

    return createMesh(24, &vertices[0][0], 12, &triangles[0][0], material);
}