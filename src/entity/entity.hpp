#pragma once

#include "yoshix.h"

// -----------------------------------------------------------------------------

enum class ETexture;

struct SEntity {
    gfx::BHandle *mesh;
    ETexture texture;
    float specularExponent;

    float position[3];
    float worldMatrix[16];

    void updateWorldMatrix();
};

// -----------------------------------------------------------------------------

enum class ETexture {
    BALL, PADDLE, BED_ROCK, BLOCK, BLOCK_HARD, BLOCK_CRACKED, // entities
    START, PAUSE, LOSS, WIN, // dialog
    LENGTH // number of textures
};

static const char* TEXTURES[] = {
    "ball.png",
    "paddle.jpg",
    "bed-rock.jpg",
    "block.jpg",
    "block-hard.jpg",
    "block-cracked.jpg",
    "dialog-start.png",
    "dialog-pause.png",
    "dialog-lost.png",
    "dialog-won.png",
};

// -----------------------------------------------------------------------------

const int FLOATS_IN_VERTEX = 8; // postion 3, normal 3, texture 2

const int NUM_OF_INPUTS = 3;
static gfx::SInputElement INPUT_ELEMENTS[] = {
    "POSITION", gfx::SInputElement::Float3,
    "NORMAL",   gfx::SInputElement::Float3,
    "TEXCOORD", gfx::SInputElement::Float2,
};

// -----------------------------------------------------------------------------

gfx::BHandle CreateCuboidMesh(gfx::BHandle &material, float width, float height, float depth);