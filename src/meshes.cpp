#include "meshes.hpp"

#include "yoshix.h"

#define _USE_MATH_DEFINES
#include <cmath>

// -----------------------------------------------------------------------------

const float TEX_MIN = 0.0f;
const float TEX_MAX = 1.0f;

// -----------------------------------------------------------------------------

const int INDICES_PER_TRIANGLE = 3;

void CreateMesh(int numOfVertices, float *vertices, int numOfTriangles, int *triangles, gfx::BHandle &material, gfx::BHandle &mesh) {
    gfx::SMeshInfo meshInfo;

    meshInfo.m_NumberOfVertices = numOfVertices;
    meshInfo.m_pVertices = vertices;

    meshInfo.m_NumberOfIndices = numOfTriangles * INDICES_PER_TRIANGLE;
    meshInfo.m_pIndices = triangles;
    
    meshInfo.m_pMaterial = material;

    gfx::CreateMesh(meshInfo, &mesh);
}

// -----------------------------------------------------------------------------

void CreateTriangleMesh(gfx::BHandle &material, gfx::BHandle &mesh) {
    float e = 0.5f;

    float vertices[][8] = {
        {  0.0f, e, 0.0f, .5*TEX_MAX, TEX_MAX,  0.0f, 0.0f,-1.0f },
        { -e, -e, 0.0f,   TEX_MIN, TEX_MIN,  0.0f, 0.0f,-1.0f  },
        {  e, -e, 0.0f,   TEX_MAX, TEX_MIN,  0.0f, 0.0f,-1.0f  },
    };

    int triangles[][3] = {
        { 0, 1, 2 },
    };

    CreateMesh(3, &vertices[0][0], 1, &triangles[0][0], material, mesh);
}

// -----------------------------------------------------------------------------

void CreateCubeMesh(gfx::BHandle &material, gfx::BHandle &mesh) {
    float e = 0.5f; // half-edge length

    float vertices[][8] = {
        // front
        { -e, e,-e,  TEX_MIN, TEX_MIN,  0.0f, 0.0f,-1.0f },
        { -e,-e,-e,  TEX_MIN, TEX_MAX,  0.0f, 0.0f,-1.0f },
        {  e,-e,-e,  TEX_MAX, TEX_MAX,  0.0f, 0.0f,-1.0f },
        {  e, e,-e,  TEX_MAX, TEX_MIN,  0.0f, 0.0f,-1.0f },
        // back
        { -e,-e, e,  TEX_MIN, TEX_MIN,  0.0f, 0.0f, 1.0f },
        { -e, e, e,  TEX_MIN, TEX_MAX,  0.0f, 0.0f, 1.0f },
        {  e, e, e,  TEX_MAX, TEX_MAX,  0.0f, 0.0f, 1.0f },
        {  e,-e, e,  TEX_MAX, TEX_MIN,  0.0f, 0.0f, 1.0f },
        // top
        { -e, e, e,  TEX_MIN, TEX_MIN,  0.0f, 1.0f, 0.0f },
        { -e, e,-e,  TEX_MIN, TEX_MAX,  0.0f, 1.0f, 0.0f },
        {  e, e,-e,  TEX_MAX, TEX_MAX,  0.0f, 1.0f, 0.0f },
        {  e, e, e,  TEX_MAX, TEX_MIN,  0.0f, 1.0f, 0.0f },
        // bottom
        { -e,-e,-e,  TEX_MIN, TEX_MIN,  0.0f,-1.0f, 0.0f },
        { -e,-e, e,  TEX_MIN, TEX_MAX,  0.0f,-1.0f, 0.0f },
        {  e,-e, e,  TEX_MAX, TEX_MAX,  0.0f,-1.0f, 0.0f },
        {  e,-e,-e,  TEX_MAX, TEX_MIN,  0.0f,-1.0f, 0.0f },
        // right
        {  e, e,-e,  TEX_MIN, TEX_MIN,  1.0f, 0.0f, 0.0f },
        {  e,-e,-e,  TEX_MIN, TEX_MAX,  1.0f, 0.0f, 0.0f },
        {  e,-e, e,  TEX_MAX, TEX_MAX,  1.0f, 0.0f, 0.0f },
        {  e, e, e,  TEX_MAX, TEX_MIN,  1.0f, 0.0f, 0.0f },
        // left
        { -e, e, e,  TEX_MIN, TEX_MIN, -1.0f, 0.0f, 0.0f },
        { -e,-e, e,  TEX_MIN, TEX_MAX, -1.0f, 0.0f, 0.0f },
        { -e,-e,-e,  TEX_MAX, TEX_MAX, -1.0f, 0.0f, 0.0f },
        { -e, e,-e,  TEX_MAX, TEX_MIN, -1.0f, 0.0f, 0.0f },
    };

    int triangles[][3] = {
        {  0, 1, 2 }, {  0, 2, 3 },
        {  4, 5, 6 }, {  4, 6, 7 },
        {  8, 9,10 }, {  8,10,11 },
        { 12,13,14 }, { 12,14,15 },
        { 16,17,18 }, { 16,18,19 },
        { 20,21,22 }, { 20,22,23 },
    };

    CreateMesh(24, &vertices[0][0], 12, &triangles[0][0], material, mesh);
}

// -----------------------------------------------------------------------------

const float RADIUS = 1.0f; // radius of sphere
const int N = 12; // number of vertical meridians
const int M = 8; // number of equatorial meridians

const float PI = float(M_PI);

void makeCircleVertices(float radius, float height, float texHeight, float vertices[N][8]) {
    float texWidth = 0.0f;
    float texWidthStep = 1.0f / (N-1);

    float angle = 0.0f;
    float angleStep = 2*PI / (N-1);

    for (int i = 0; i < N; i++) {
        vertices[i][0] = cos(angle) * radius;
        vertices[i][1] = height;
        vertices[i][2] = sin(angle) * radius;

        vertices[i][3] = texWidth;
        vertices[i][4] = texHeight;

        texWidth += texWidthStep;
        angle += angleStep;
    }
}

void makeSphereVertices(float vertices[][8]) {
    float angle = PI / 2.0f;
    float angleStep = PI / (M-1);

    float texHeight = 0.0f;
    float texHeightStep = 1.0f / (M-1);

    for (int i = 0; i < M; i++) {
        makeCircleVertices(cos(angle)*RADIUS, sin(angle)*RADIUS, texHeight, &vertices[i*N]);

        angle -= angleStep;
        texHeight += texHeightStep;
    }
}

void addNormalsFromPosition(float vertices[][8], size_t n) {
    for (unsigned int i = 0; i < n; i++) {
        float normal[3] = {vertices[i][0], vertices[i][1], vertices[i][2]};
        gfx::GetNormalizedVector(normal, normal);
        vertices[i][5] = normal[0];
        vertices[i][6] = normal[1];
        vertices[i][7] = normal[2];
    }
}

void triangulateSphere(int triangles[][3]) {
    int index = 0;
    for (int row = 0; row < (M-1); row++) {
        for (int i = 1; i < N; i++) {
            triangles[index][0] = (row+1)*N + i - 1;
            triangles[index][1] = (row+1)*N + i;
            triangles[index][2] = row*N + i;
            index++;
            triangles[index][0] = (row+1)*N + i - 1;
            triangles[index][1] = row*N + i;
            triangles[index][2] = row*N + i - 1;
            index++;
        }
    }
}

void CreateSphereMesh(gfx::BHandle &material, gfx::BHandle &mesh) {
    float vertices[N*M][8];
    makeSphereVertices(vertices);

    addNormalsFromPosition(vertices, N*M);

    int triangles[2*N*(M-1)][3];
    triangulateSphere(triangles);

    CreateMesh(N*M, &vertices[0][0], 2*N*(M-1), &triangles[0][0], material, mesh);
}