#pragma once

#include "yoshix.h"

// -----------------------------------------------------------------------------

enum ETexture { BALL, PADDLE, BED_ROCK, BLOCK, BLOCK_HARD, BLOCK_CRACKED };

struct SEntity {
    gfx::BHandle *mesh;
    ETexture texture;

    float position[3];
    float worldMatrix[16];

    void updateWorldMatrix();
};

// -----------------------------------------------------------------------------

gfx::BHandle createMaterial(
    int numOfTex, gfx::BHandle textures[],
    int numOfVSB, gfx::BHandle vsBuffers[],
    int numOfPSB, gfx::BHandle psBuffers[],
    gfx::BHandle &vertexShader, gfx::BHandle &pixelShader
);

// -----------------------------------------------------------------------------

const int FLOATS_IN_VERTEX = 8; // postion 3, normal 3, texture 2
const int INTS_IN_TRIANGLE = 3;

gfx::BHandle createMesh(int numOfVerts, float *verts, int numOfTriangles, int *triangles, gfx::BHandle &material);