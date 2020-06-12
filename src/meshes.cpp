#include "yoshix.h"

// -----------------------------------------------------------------------------

gfx::SMeshInfo GetTriangleMesh(gfx::BHandle &material) {
    gfx::SMeshInfo meshInfo;

    float vertices[][3] = {
        { -0.5f, -0.5f, 0.0f },
        {  0.5f, -0.5f, 0.0f },
        {  0.0f,  0.5f, 0.0f },
    };

    int triangles[][3] = {
        { 0, 1, 2 },
    };

    meshInfo.m_NumberOfVertices = 3;
    meshInfo.m_pVertices = &vertices[0][0];

    meshInfo.m_NumberOfIndices = 3;
    meshInfo.m_pIndices = &triangles[0][0];
    
    meshInfo.m_pMaterial = material;

    return meshInfo;
}

void CreateTriangleMesh(gfx::BHandle &material, gfx::BHandle* mesh) {
    gfx::SMeshInfo meshInfo;

    float vertices[][3] = {
        { -0.5f, -0.5f, 0.0f },
        {  0.5f, -0.5f, 0.0f },
        {  0.0f,  0.5f, 0.0f },
    };

    int triangles[][3] = {
        { 0, 1, 2 },
    };

    meshInfo.m_NumberOfVertices = 3;
    meshInfo.m_pVertices = &vertices[0][0];

    meshInfo.m_NumberOfIndices = 3;
    meshInfo.m_pIndices = &triangles[0][0];
    
    meshInfo.m_pMaterial = material;

    CreateMesh(meshInfo, mesh);
}