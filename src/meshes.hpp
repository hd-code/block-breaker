#include "yoshix.h"

// -----------------------------------------------------------------------------

struct SVector3 {
    int x, y, z;
};

struct SVector2 {
    int u, v;
};

struct SVertex {
    SVector3 position;
    SVector2 texture;
    SVector3 normal;
};

void CreateTriangleMesh(gfx::BHandle &material, gfx::BHandle &mesh);

void CreateCubeMesh(gfx::BHandle &material, gfx::BHandle &mesh);

void CreateSphereMesh(gfx::BHandle &material, gfx::BHandle &mesh);