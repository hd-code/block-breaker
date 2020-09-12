#include "mesh.hpp"

// -----------------------------------------------------------------------------

gfx::BHandle CreateMesh(
    int numOfVerts, float *vertices,
    int numOfTriangles, int *triangles,
    gfx::BHandle &material
) {
    gfx::SMeshInfo meshInfo;

    meshInfo.m_NumberOfVertices = numOfVerts;
    meshInfo.m_pVertices = vertices;

    meshInfo.m_NumberOfIndices = numOfTriangles * INDICES_PER_TRIANGLE;
    meshInfo.m_pIndices = triangles;

    meshInfo.m_pMaterial = material;

    gfx::BHandle mesh = nullptr;
    gfx::CreateMesh(meshInfo, &mesh);
    return mesh;
}