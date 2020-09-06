#include "meshes.hpp"

#include "yoshix.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

// -----------------------------------------------------------------------------

const float TEX_MIN = 0.0f;
const float TEX_MAX = 1.0f;

// -----------------------------------------------------------------------------

const int INDICES_PER_TRIANGLE = 3;

gfx::BHandle CreateMesh(int numOfVertices, float *vertices, int numOfTriangles, int *triangles, gfx::BHandle &material) {
    gfx::SMeshInfo meshInfo;

    meshInfo.m_NumberOfVertices = numOfVertices;
    meshInfo.m_pVertices = vertices;

    meshInfo.m_NumberOfIndices = numOfTriangles * INDICES_PER_TRIANGLE;
    meshInfo.m_pIndices = triangles;
    
    meshInfo.m_pMaterial = material;

    gfx::BHandle mesh = nullptr;
    gfx::CreateMesh(meshInfo, &mesh);
    return mesh;
}

// -----------------------------------------------------------------------------

void CreateTriangleMesh(gfx::BHandle &material, gfx::BHandle &mesh) {
    float e = 0.5f;

    float vertices[][8] = {
        {0.0f, e, 0.0f,   0.0f, 0.0f,-1.0f,  .5*TEX_MAX, TEX_MIN },
        {  -e,-e, 0.0f,   0.0f, 0.0f,-1.0f,     TEX_MIN, TEX_MAX },
        {   e,-e, 0.0f,   0.0f, 0.0f,-1.0f,     TEX_MAX, TEX_MAX },
    };

    int triangles[][3] = {
        { 0, 1, 2 },
    };

    mesh = CreateMesh(3, &vertices[0][0], 1, &triangles[0][0], material);
}

// -----------------------------------------------------------------------------

void CreateCubeMesh(gfx::BHandle &material, gfx::BHandle &mesh) {
    float e = 0.5f; // half-edge length

    float vertices[][8] = {
        // front
        { -e, e,-e,    0.0f, 0.0f,-1.0f,   TEX_MIN, TEX_MIN },
        { -e,-e,-e,    0.0f, 0.0f,-1.0f,   TEX_MIN, TEX_MAX },
        {  e,-e,-e,    0.0f, 0.0f,-1.0f,   TEX_MAX, TEX_MAX },
        {  e, e,-e,    0.0f, 0.0f,-1.0f,   TEX_MAX, TEX_MIN },
        // back
        { -e,-e, e,    0.0f, 0.0f, 1.0f,   TEX_MIN, TEX_MIN },
        { -e, e, e,    0.0f, 0.0f, 1.0f,   TEX_MIN, TEX_MAX },
        {  e, e, e,    0.0f, 0.0f, 1.0f,   TEX_MAX, TEX_MAX },
        {  e,-e, e,    0.0f, 0.0f, 1.0f,   TEX_MAX, TEX_MIN },
        // top
        { -e, e, e,    0.0f, 1.0f, 0.0f,   TEX_MIN, TEX_MIN },
        { -e, e,-e,    0.0f, 1.0f, 0.0f,   TEX_MIN, TEX_MAX },
        {  e, e,-e,    0.0f, 1.0f, 0.0f,   TEX_MAX, TEX_MAX },
        {  e, e, e,    0.0f, 1.0f, 0.0f,   TEX_MAX, TEX_MIN },
        // bottom
        { -e,-e,-e,    0.0f,-1.0f, 0.0f,   TEX_MIN, TEX_MIN },
        { -e,-e, e,    0.0f,-1.0f, 0.0f,   TEX_MIN, TEX_MAX },
        {  e,-e, e,    0.0f,-1.0f, 0.0f,   TEX_MAX, TEX_MAX },
        {  e,-e,-e,    0.0f,-1.0f, 0.0f,   TEX_MAX, TEX_MIN },
        // right
        {  e, e,-e,    1.0f, 0.0f, 0.0f,   TEX_MIN, TEX_MIN },
        {  e,-e,-e,    1.0f, 0.0f, 0.0f,   TEX_MIN, TEX_MAX },
        {  e,-e, e,    1.0f, 0.0f, 0.0f,   TEX_MAX, TEX_MAX },
        {  e, e, e,    1.0f, 0.0f, 0.0f,   TEX_MAX, TEX_MIN },
        // left
        { -e, e, e,   -1.0f, 0.0f, 0.0f,   TEX_MIN, TEX_MIN },
        { -e,-e, e,   -1.0f, 0.0f, 0.0f,   TEX_MIN, TEX_MAX },
        { -e,-e,-e,   -1.0f, 0.0f, 0.0f,   TEX_MAX, TEX_MAX },
        { -e, e,-e,   -1.0f, 0.0f, 0.0f,   TEX_MAX, TEX_MIN },
    };

    int triangles[][3] = {
        {  0, 1, 2 }, {  0, 2, 3 },
        {  4, 5, 6 }, {  4, 6, 7 },
        {  8, 9,10 }, {  8,10,11 },
        { 12,13,14 }, { 12,14,15 },
        { 16,17,18 }, { 16,18,19 },
        { 20,21,22 }, { 20,22,23 },
    };

    mesh = CreateMesh(24, &vertices[0][0], 12, &triangles[0][0], material);
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

        vertices[i][6] = texWidth;
        vertices[i][7] = texHeight;

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

void addNormalsFromPosition(size_t n, float vertices[][8]) {
    for (unsigned int i = 0; i < n; i++) {
        float normal[3] = {vertices[i][0], vertices[i][1], vertices[i][2]};
        gfx::GetNormalizedVector(normal, normal);
        vertices[i][3] = normal[0];
        vertices[i][4] = normal[1];
        vertices[i][5] = normal[2];
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

    addNormalsFromPosition(N*M, vertices);

    int triangles[2*N*(M-1)][3];
    triangulateSphere(triangles);

    mesh = CreateMesh(N*M, &vertices[0][0], 2*N*(M-1), &triangles[0][0], material);
}

// -----------------------------------------------------------------------------

void calcVertexInMiddle(float vert1[3], float vert2[3], float result[3]) {
    // position
    result[0] = (vert1[0] + vert2[0]) / 2.0f;
    result[1] = (vert1[1] + vert2[1]) / 2.0f;
    result[2] = (vert1[2] + vert2[2]) / 2.0f;
    // texture coordinate
    result[6] = (vert1[6] + vert2[6]) / 2.0f;
    result[7] = (vert1[7] + vert2[7]) / 2.0f;
}

void subdivideTriangle(std::vector<float[8]> vertices, std::vector<int[3]> triangles, int indexOfTriangleToSubdivide) {
    int *origTriangle = triangles.at(indexOfTriangleToSubdivide);
    float *origVerts[] = {
        vertices.at(origTriangle[0]),
        vertices.at(origTriangle[1]),
        vertices.at(origTriangle[2]),
    };

    int currentNumOfVerts = vertices.size();
    int newVertsIndex[3] = { currentNumOfVerts, currentNumOfVerts + 1, currentNumOfVerts + 2 };

    /*
           2
           /\
         2/--\1
         /\  /\
        0------1
           0
    */

    float newVerts[3][8];
    calcVertexInMiddle(origVerts[0], origVerts[1], newVerts[0]);
    calcVertexInMiddle(origVerts[1], origVerts[2], newVerts[1]);
    calcVertexInMiddle(origVerts[0], origVerts[2], newVerts[2]);

    int newTriangles[][3] = {
        { origTriangle[0], newVertsIndex[0], newVertsIndex[2] },
        { origTriangle[1], newVertsIndex[1], newVertsIndex[0] },
        { origTriangle[2], newVertsIndex[2], newVertsIndex[1] },
        {newVertsIndex[0], newVertsIndex[1], newVertsIndex[2] },
    };

    vertices.push_back(newVerts[0]);
    vertices.push_back(newVerts[1]);
    vertices.push_back(newVerts[2]);

    triangles.erase(triangles.begin() + indexOfTriangleToSubdivide);
    triangles.push_back(newTriangles[0]);
    triangles.push_back(newTriangles[1]);
    triangles.push_back(newTriangles[2]);
    triangles.push_back(newTriangles[3]);
}

gfx::BHandle CreateGeoSphereMesh(gfx::BHandle &material) {
    // std::vector<float[8]> vertices;
    // std::vector<int[3]> triangles;

    float initVerts[][8] = { // normals are set later automatically
        // Top verts
        {  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.125f, 0.0f }, // 0
        {  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.375f, 0.0f },
        {  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.675f, 0.0f },
        {  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.875f, 0.0f },

        {  0.0f, 0.0f,-1.0f,   0.0f, 0.0f, 0.0f,   0.0f , 0.5f }, // 4
        {  1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.25f, 0.5f },
        {  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.5f , 0.5f },
        { -1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.75f, 0.5f },
        {  0.0f, 0.0f,-1.0f,   0.0f, 0.0f, 0.0f,   1.0f , 0.5f },

        // Bottom verts
        {  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.125f, 1.0f }, // 9
        {  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.375f, 1.0f },
        {  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.675f, 1.0f },
        {  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.875f, 1.0f },
    };

    int initTriangles[][3] = {
        { 0,4,5 }, { 1,5,6 }, { 2,6,7 }, { 3,7,8 }, // upper half of diamond
        { 9,5,4 }, {10,6,5 }, {11,7,6 }, {12,8,7 }, // lower half of diamond
    };

    return CreateMesh(13, &initVerts[0][0], 8, &initTriangles[0][0], material);
}