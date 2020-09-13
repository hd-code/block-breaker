#pragma once

#include "yoshix.h"

// -----------------------------------------------------------------------------

const int INDICES_PER_TRIANGLE = 3;

gfx::BHandle CreateMesh(
    int numOfVerts, float *vertices,
    int numOfTriangles, int *triangles,
    gfx::BHandle &material
);