#include "paddle.hpp"

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
    return CreateCuboidMesh(material, SIZE[0], SIZE[1], SIZE[2]);
}