#pragma once

#include "yoshix.h"

// -----------------------------------------------------------------------------

enum ETexture { TEX_BALL, TEX_PADDLE, TEX_BED_ROCK, TEX_BLOCK, TEX_BLOCK_HARD, TEX_BLOCK_CRACKED };

enum ECollision { TOP, BOTTOM, LEFT, RIGHT };

struct SEntity {
    gfx::BHandle *mesh;
    ETexture texture;

    float position[3];
    float worldMatrix[16];

    void updateWorldMatrix();
};

// -----------------------------------------------------------------------------

gfx::BHandle createMaterial(
    gfx::BHandle textures[],  int numOfTex,
    gfx::BHandle vsBuffers[], int numOfVSB,
    gfx::BHandle psBuffers[], int numOfPSB,
    gfx::BHandle &vertexShader, gfx::BHandle &pixelShader
);

// -----------------------------------------------------------------------------

const int FLOATS_IN_VERTEX = 8; // postion 3, normal 3, texture 2
const int INTS_IN_TRIANGLE = 3;

gfx::BHandle createMesh(int numOfVerts, float *verts, int numOfTriangles, int *triangles, gfx::BHandle &material);