

struct SVertex {
    float position[3];
    float normal[3];
    float texCoords[2];
};

struct STriangle {
    int indices[3];
};


const int NUM_OF_SUBDIVISIONS = 3;

void calcVertexInMiddle(float vert1[8], float vert2[8], float result[8]) {
    // position
    result[0] = (vert1[0] + vert2[0]) / 2.0f;
    result[1] = (vert1[1] + vert2[1]) / 2.0f;
    result[2] = (vert1[2] + vert2[2]) / 2.0f;
    // texture coordinate
    result[6] = (vert1[6] + vert2[6]) / 2.0f;
    result[7] = (vert1[7] + vert2[7]) / 2.0f;
}
/*
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
    *

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
} //*/

void normalizePositionAndUseAsNormal(float vertex[8]) {
    float position[3];

    position[0] = vertex[0];
    position[1] = vertex[1];
    position[2] = vertex[2];

    gfx::GetNormalizedVector(position, position);

    vertex[0] = position[0];
    vertex[1] = position[1];
    vertex[2] = position[2];
    vertex[3] = position[0];
    vertex[4] = position[1];
    vertex[5] = position[2];
}

void normalizePositionAndUseAsNormal(size_t numOfVertices, float vertices[][8]) {
    for (size_t i = 0; i < numOfVertices; i++) {
        normalizePositionAndUseAsNormal(vertices[i]);
    }
}

gfx::BHandle CreateGeoSphereMesh(gfx::BHandle &material) {
    // std::vector<float[8]> vertices;
    // std::vector<int[3]> triangles;

    float initVerts[][8] = { // normals are set later automatically
        // Top verts
        {  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.000f, 0.0f }, // 0
        {  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.333f, 0.0f },
        {  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.667f, 0.0f },
        {  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   1.000f, 0.0f },

        {  0.0f, 0.0f,-1.0f,   0.0f, 0.0f, 0.0f,   0.0f , 0.5f }, // 4
        {  1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.25f, 0.5f },
        {  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.5f , 0.5f },
        { -1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.75f, 0.5f },
        {  0.0f, 0.0f,-1.0f,   0.0f, 0.0f, 0.0f,   1.0f , 0.5f },

        // Bottom verts
        {  0.0f,-1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.000f, 1.0f }, // 9
        {  0.0f,-1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.333f, 1.0f },
        {  0.0f,-1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.667f, 1.0f },
        {  0.0f,-1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   1.000f, 1.0f },
    };

    normalizePositionAndUseAsNormal(13, initVerts);

    int initTriangles[][3] = {
        { 0,4,5 }, { 1,5,6 }, { 2,6,7 }, { 3,7,8 }, // upper half of diamond
        { 9,5,4 }, {10,6,5 }, {11,7,6 }, {12,8,7 }, // lower half of diamond
    };

    return createMesh(13, &initVerts[0][0], 8, &initTriangles[0][0], material);
}