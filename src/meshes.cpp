#include "meshes.hpp"

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#include "yoshix.h"

// -----------------------------------------------------------------------------

const float texMin = 0.0f;
const float texMax = 1.0f;

void normalizeVertex(SVertex &vertex) {
    float normal[3] = { vertex.normal.x, vertex.normal.y, vertex.normal.z };
    gfx::GetNormalizedVector(normal, normal);
    vertex.normal.x = normal[0];
    vertex.normal.y = normal[1];
    vertex.normal.z = normal[2];
}

void normalizeVerts(size_t n, SVertex vertices[]) {
    for (int i = 0; i < n; i++) {
        normalizeVertex(vertices[i]);
    }
}

void addNormal(SVertex &vertex) {
    float normal[3] = { vertex.position.x, vertex.position.y, vertex.position.z };
    gfx::GetNormalizedVector(normal, normal);
    vertex.normal.x = normal[0];
    vertex.normal.y = normal[1];
    vertex.normal.z = normal[2];
}

void addNormals(size_t n, SVertex vertices[]) {
    for (int i = 0; i < n; i++) {
        addNormal(vertices[i]);
    }
}

void getFloatArrays(size_t n, SVertex vertices[], float result[][8]) {
    for (int i = 0; i < n; i++) {
        result[i][0] = vertices[i].position.x;
        result[i][1] = vertices[i].position.y;
        result[i][2] = vertices[i].position.z;
        result[i][3] = vertices[i].texture.u;
        result[i][4] = vertices[i].texture.v;
        result[i][5] = vertices[i].normal.x;
        result[i][6] = vertices[i].normal.y;
        result[i][7] = vertices[i].normal.z;
    }
}

// -----------------------------------------------------------------------------

void CreateTriangleMesh(gfx::BHandle &material, gfx::BHandle* mesh) {
    float e = 0.5f;

    SVertex verts[] = {
        {  0.0f, e, 0.0f, .5*texMax, texMin,  0.0f, 0.0f,-1.0f },
        { -e, -e, 0.0f,   texMin, texMax,  0.0f, 0.0f,-1.0f  },
        {  e, -e, 0.0f,   texMax, texMax,  0.0f, 0.0f,-1.0f  },
    };

    float vertices[3][8];
    getFloatArrays(3, verts, vertices);

    int triangles[][3] = {
        { 0, 1, 2 },
    };

    gfx::SMeshInfo meshInfo;

    meshInfo.m_NumberOfVertices = 3;
    meshInfo.m_pVertices = &vertices[0][0];

    meshInfo.m_NumberOfIndices = 3;
    meshInfo.m_pIndices = &triangles[0][0];
    
    meshInfo.m_pMaterial = material;

    gfx::CreateMesh(meshInfo, mesh);
}

// -----------------------------------------------------------------------------

void CreateCubeMesh(gfx::BHandle &material, gfx::BHandle &mesh) {
    int numOfVerts = 8;
    float e = 0.5f; // half-edge length

    SVertex verts[] = {
        { -e, e,-e,  texMin, texMin },
        {  e, e,-e,  texMax, texMin },
        { -e,-e,-e,  texMin, texMax },
        {  e,-e,-e,  texMax, texMax },
        { -e, e, e,  texMax, texMin },
        {  e, e, e,  texMin, texMin },
        { -e,-e, e,  texMax, texMax },
        {  e,-e, e,  texMin, texMax },
    };
    addNormals(8, verts);

    int triangles[][3] = {
        { 0, 2, 3 }, { 0, 3, 1 },
        { 1, 3, 7 }, { 1, 7, 5 },
        { 0, 2, 6 }, { 0, 6, 4 },
        { 0, 1, 5 }, { 0, 5, 4 },
        { 2, 6, 7 }, { 2, 7, 3 },
        { 4, 5, 7 }, { 4, 7, 6 },
    };

    float vertices[numOfVerts][8];
    getFloatArrays(numOfVerts, verts, vertices);

    gfx::SMeshInfo meshInfo;

    meshInfo.m_NumberOfVertices = numOfVerts;
    meshInfo.m_pVertices = &vertices[0][0];

    meshInfo.m_NumberOfIndices = 12*3;
    meshInfo.m_pIndices = &triangles[0][0];
    
    meshInfo.m_pMaterial = material;

    gfx::CreateMesh(meshInfo, &mesh);
}

// -----------------------------------------------------------------------------

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

void CreateSphereMesh(gfx::BHandle &material, gfx::BHandle &mesh) {
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

    gfx::CreateMesh(meshInfo, &mesh);
}