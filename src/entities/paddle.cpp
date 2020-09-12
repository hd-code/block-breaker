#include "paddle.hpp"

#include "../helper/mesh.hpp"

// -----------------------------------------------------------------------------

void SPaddle::move(EKey key, float leftLimit, float rightLimit) {
    float newX = this->position[0];
    float halfWidth = this->width / 2.0f;

    switch (key) {
    case EKey::LEFT:
        newX -= this->speed;
        if (newX - halfWidth > leftLimit) {
            this->position[0] = newX;
            this->updateWorldMatrix();
        }
        break;

    case EKey::RIGHT:
        newX += this->speed;
        if (newX + halfWidth < rightLimit) {
            this->position[0] = newX;
            this->updateWorldMatrix();
        }
        break;
    }
}

// -----------------------------------------------------------------------------

const ETexture TEXTURE = ETexture::PADDLE;
const float SPEC_EXP   = 2.0f;
const float POSITION[] = { 0.0f, -4.0f, 0.0f };

const float SIZE[] = { 1.5f, 0.2f, 1.0f };
const float SPEED  = 0.1f;

SPaddle CreatePaddle(gfx::BHandle* paddleMesh) {
    SPaddle paddle;
    paddle.mesh = paddleMesh;
    paddle.texture = TEXTURE;
    paddle.specularExponent = SPEC_EXP;

    paddle.position[0] = POSITION[0];
    paddle.position[1] = POSITION[1];
    paddle.position[2] = POSITION[2];

    paddle.width  = SIZE[0];
    paddle.height = SIZE[1];

    paddle.speed = SPEED;

    paddle.updateWorldMatrix();

    return paddle;
}

// --- Mesh --------------------------------------------------------------------

gfx::BHandle CreatePaddleMesh(gfx::BHandle &material) {
    float x = SIZE[0] / 2.0f; // half width  length
    float y = SIZE[1] / 2.0f; // half height length
    float z = SIZE[2] / 2.0f; // half depth  length

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

    return CreateMesh(24, &vertices[0][0], 12, &triangles[0][0], material);
}