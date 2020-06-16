#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#include "yoshix.h"

// -----------------------------------------------------------------------------

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

    gfx::CreateMesh(meshInfo, mesh);
}

// -----------------------------------------------------------------------------

struct SVector3 {
    int x, y, z;
};

struct SVertex {
    SVector3 position;
};

const float RADIUS = 1.0f;
const float PI_2 = 2 * M_PI;
const int N = 8;

void makeCircleVertices(float radius, int n, float vertices[][3]) {
    float stepSize = PI_2 / n;
    float angle = 0;

    for (int i = 0; i < n; i++) {
        vertices[i][0] = cos(angle) * radius;
        vertices[i][1] = 0.0f;
        vertices[i][2] = sin(angle) * radius;

        std::cout << vertices[i][0] << " " << vertices[i][1] << " " << vertices[i][2] << std::endl;

        angle += stepSize;
    }
}

void triangulateCircle(int n, int triangles[][3]) {
    // connecting all vertices between start and end
    for (int i = 0; i < n; i++) {
        triangles[i][0] = 0;
        triangles[i][1] = i+1;
        triangles[i][2] = i+2;

        std::cout << triangles[i][0] << " " << triangles[i][1] << " " << triangles[i][2] << std::endl;
    }
}

void CreateSphereMesh(gfx::BHandle &material, gfx::BHandle* mesh) {
    gfx::SMeshInfo meshInfo;

    float vertices[N][3];
    makeCircleVertices(RADIUS, N, vertices);

    int triangles[N-2][3];
    triangulateCircle(N-2, triangles);

    meshInfo.m_NumberOfVertices = N;
    meshInfo.m_pVertices = &vertices[0][0];

    meshInfo.m_NumberOfIndices = (N-2) * 3;
    meshInfo.m_pIndices = &triangles[0][0];
    
    meshInfo.m_pMaterial = material;

    gfx::CreateMesh(meshInfo, mesh);
}