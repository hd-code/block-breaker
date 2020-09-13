#include "dialog.hpp"

// -----------------------------------------------------------------------------

const float SPEC_EXP   = 120.0f;
const float POSITION[] = { 0.0f, 0.0f, -2.0f };

const float SIZE[] = { 5.0f, 2.0f, 0.1f };

const float ANI_POSITION[] = { 0.0f, 0.0f, -3.0f }; // starting position of animation (relative position)
const float ANI_SCALE = 0.0f; // starting scale of animation (will be 1 in the end)
const unsigned int ANI_FRAMES = 10; // number of frames the animation takes

ETexture getDialogTexture(EDialogType type) {
    switch (type) {
        case EDialogType::START: return ETexture::START;
        case EDialogType::PAUSE: return ETexture::PAUSE;
        case EDialogType::LOSS:  return ETexture::LOSS;
        case EDialogType::WIN:   return ETexture::WIN;
        default: return ETexture::PAUSE;
    }
}

SDialog::SDialog() {}

SDialog::SDialog(gfx::BHandle* dialogMesh, EDialogType type) {
    this->mesh = dialogMesh;
    this->texture = getDialogTexture(type);
    this->specularExponent = SPEC_EXP;

    this->position[0] = POSITION[0];
    this->position[1] = POSITION[1];
    this->position[2] = POSITION[2];

    this->animation.position[0] = ANI_POSITION[0];
    this->animation.position[1] = ANI_POSITION[1];
    this->animation.position[2] = ANI_POSITION[2];
    this->animation.scale = ANI_SCALE;
    this->animation.frame = ANI_FRAMES;

    this->calcAnimationSteps();
}

// -----------------------------------------------------------------------------

void SDialog::onFrame() {
    if (this->animation.frame <= 0) {
        return;
    }

    this->animation.position[0] += this->positionStep[0];
    this->animation.position[1] += this->positionStep[1];
    this->animation.position[2] += this->positionStep[2];
    this->animation.scale += this->scaleStep;
    this->animation.frame--;

    this->calcWorldMatrix();
}

// -----------------------------------------------------------------------------

void SDialog::calcAnimationSteps() {
    this->positionStep[0] = - this->animation.position[0] / this->animation.frame;
    this->positionStep[1] = - this->animation.position[1] / this->animation.frame;
    this->positionStep[2] = - this->animation.position[2] / this->animation.frame;

    this->scaleStep = (1.0f - this->animation.scale) / this->animation.frame;
}

void SDialog::calcWorldMatrix() {
    float aniMatrix[16];

    gfx::GetScaleMatrix(this->animation.scale, this->worldMatrix);
    gfx::GetTranslationMatrix(
        this->animation.position[0],
        this->animation.position[1],
        this->animation.position[2],
        aniMatrix
    );
    gfx::MulMatrix(this->worldMatrix, aniMatrix, aniMatrix);

    this->updateWorldMatrix();

    gfx::MulMatrix(aniMatrix, this->worldMatrix, this->worldMatrix);
}

// -----------------------------------------------------------------------------

gfx::BHandle CreateDialogMesh(gfx::BHandle &material) {
    return CreateCuboidMesh(material, SIZE[0], SIZE[1], SIZE[2]);
}