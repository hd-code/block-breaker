#include "ball.hpp"

#include "../helper/math.hpp"
#include "../helper/mesh.hpp"
#include "yoshix.h"

#include <cmath>
#include <vector>

// -----------------------------------------------------------------------------

const float WEIGHT_OF_PADDLE_ANGLE = 0.8f;

void SBall::move() {
    this->position[0] += this->speed * this->direction[0];
    this->position[1] += this->speed * this->direction[1];
    this->position[2] += this->speed * this->direction[2];

    this->updateWorldMatrix();
}

bool SBall::handleCollision(float topBorder, float leftBorder, float rightBorder) {
    if (topBorder < this->position[1] + this->radius) {
        this->changeDirection(ECollisionAt::TOP);
        return true;
    }

    if (leftBorder > this->position[0] - this->radius) {
        this->changeDirection(ECollisionAt::LEFT);
        return true;
    }

    if (rightBorder < this->position[0] + this->radius) {
        this->changeDirection(ECollisionAt::RIGHT);
        return true;
    }

    return false;
}

bool SBall::handleCollision(SBlock &block) {
    float xDiff = std::fabs(this->position[0] - block.position[0]);
    float yDiff = std::fabs(this->position[1] - block.position[1]);

    float offset = this->radius + (block.size / 2.0f);

    xDiff -= offset;
    yDiff -= offset;

    // collision
    if (xDiff <= 0 && yDiff <= 0) {
        ECollisionAt collAt;
        if (xDiff > yDiff) { // horizontal collision
            if (this->position[0] < block.position[0]) {
                collAt = ECollisionAt::RIGHT;
            } else {
                collAt = ECollisionAt::LEFT;
            }
        } else { // vertical collision
            if (this->position[1] < block.position[1]) {
                collAt = ECollisionAt::TOP;
            } else {
                collAt = ECollisionAt::BOTTOM;
            }
        }

        this->changeDirection(collAt);
        block.onCollision();

        return true;
    }

    return false;
}

bool SBall::handleCollision(const SPaddle &paddle) {
    if (this->position[1] - this->radius > paddle.position[1] + paddle.height / 2.0f) {
        return false;
    }

    float xDiff = this->position[0] - paddle.position[0];
    float xOffset = this->radius + (paddle.width  / 2.0f);

    if (std::fabs(xDiff) < xOffset) { // collision
        this->changeDirection(ECollisionAt::BOTTOM); // deflect ball upwards

        float xDirection = xDiff / xOffset;
        this->direction[0] += WEIGHT_OF_PADDLE_ANGLE * (xDirection - this->direction[0]);
        gfx::GetNormalizedVector(this->direction, this->direction);

        return true;
    }

    return false;
}

bool SBall::isOnGround(float groundLevel) {
    return groundLevel > this->position[1];
}

void SBall::changeDirection(ECollisionAt collisionAt) {
    switch (collisionAt) {
    case ECollisionAt::TOP:
        if (this->direction[1] > 0) {
            this->direction[1] *= -1.0f;
        }
        break;

    case ECollisionAt::BOTTOM:
        if (this->direction[1] < 0) {
            this->direction[1] *= -1.0f;
        }
        break;

    case ECollisionAt::LEFT:
        if (this->direction[0] < 0) {
            this->direction[0] *= -1.0f;
        }
        break;

    case ECollisionAt::RIGHT:
        if (this->direction[0] > 0) {
            this->direction[0] *= -1.0f;
        }
        break;
    }
}

// -----------------------------------------------------------------------------

const ETexture TEXTURE = ETexture::BALL;
const float SPEC_EXP   = 80.0f;
const float POSITION[] = { 0.0f, 0.0f, 0.0f };

const float RADIUS = 0.3f;
const float SPEED = 0.15f;

const float DIRECTION[] = { 0.0f, -1.0f, 0.0f };
const float DIR_MIN_X = 0.1f;
const float DIR_MAX_X = 0.3f;

SBall::SBall() {}

SBall::SBall(gfx::BHandle* ballMesh) {
    this->mesh = ballMesh;
    this->texture = TEXTURE;
    this->specularExponent = SPEC_EXP;

    this->position[0] = POSITION[0];
    this->position[1] = POSITION[1];
    this->position[2] = POSITION[2];

    this->radius = RADIUS;
    this->speed  = SPEED;

    // ball will always start with a slight random angle
    this->direction[0] = GetRandom(DIR_MIN_X, DIR_MAX_X);
    if (GetRandom() < 0.5f) {
        this->direction[0] *= -1.0f;
    }
    this->direction[1] = -1.0f;
    this->direction[2] = 0.0f;

    gfx::GetNormalizedVector(this->direction, this->direction);

    this->updateWorldMatrix();
}

// --- Mesh --------------------------------------------------------------------

struct SVertex {
    float position[3];
    float texCoords[2];
};

struct STriangle {
    int indices[3];
};

void scaleVector(float vec[3], float scale, float result[3]) {
    result[0] = vec[0] * scale;
    result[1] = vec[1] * scale;
    result[2] = vec[2] * scale;
}

void calcVertexInMiddle(const SVertex &vert1, const SVertex &vert2, SVertex &result) {
    // position
    result.position[0] = (vert1.position[0] + vert2.position[0]) / 2.0f;
    result.position[1] = (vert1.position[1] + vert2.position[1]) / 2.0f;
    result.position[2] = (vert1.position[2] + vert2.position[2]) / 2.0f;
    // texture coordinate
    result.texCoords[0] = (vert1.texCoords[0] + vert2.texCoords[0]) / 2.0f;
    result.texCoords[1] = (vert1.texCoords[1] + vert2.texCoords[1]) / 2.0f;
}

void subdivideTriangle(std::vector<SVertex> &verts, std::vector<STriangle> &triangles, int indexOfTriangle) {
    STriangle origTriangle = triangles.at(indexOfTriangle);
    SVertex origVerts[] = {
        verts.at(origTriangle.indices[0]),
        verts.at(origTriangle.indices[1]),
        verts.at(origTriangle.indices[2]),
    };

    int currentNumOfVerts = verts.size();
    int newVertsIndex[3] = { currentNumOfVerts, currentNumOfVerts + 1, currentNumOfVerts + 2 };

    /*
           2
           /\
         2/--\1
         /\  /\
        0------1
           0
    */

    SVertex newVerts[3];
    calcVertexInMiddle(origVerts[0], origVerts[1], newVerts[0]);
    calcVertexInMiddle(origVerts[1], origVerts[2], newVerts[1]);
    calcVertexInMiddle(origVerts[0], origVerts[2], newVerts[2]);

    STriangle newTriangles[4] = {
        { origTriangle.indices[0], newVertsIndex[0], newVertsIndex[2] },
        { origTriangle.indices[1], newVertsIndex[1], newVertsIndex[0] },
        { origTriangle.indices[2], newVertsIndex[2], newVertsIndex[1] },
        {        newVertsIndex[0], newVertsIndex[1], newVertsIndex[2] },
    };

    verts.push_back(newVerts[0]);
    verts.push_back(newVerts[1]);
    verts.push_back(newVerts[2]);

    triangles.erase(triangles.begin() + indexOfTriangle);
    triangles.push_back(newTriangles[0]);
    triangles.push_back(newTriangles[1]);
    triangles.push_back(newTriangles[2]);
    triangles.push_back(newTriangles[3]);
}

void subdivideTriangles(std::vector<SVertex> &verts, std::vector<STriangle> &triangles) {
    int numOfTriangles = triangles.size();
    for (int i = 0; i < numOfTriangles; i++) {
        subdivideTriangle(verts, triangles, 0); // original triangle is delted and new ones are appended
    }
}

float* toVertexArray(std::vector<SVertex> verts, float tmpArray[], int numOfVerts) {
    for (int i = 0; i < numOfVerts; i++) {
        SVertex vertex = verts[i];

        float normal[3];
        gfx::GetNormalizedVector(vertex.position, normal);

        scaleVector(normal, RADIUS, vertex.position);

        int index = i * FLOATS_IN_VERTEX;
        tmpArray[index + 0] = vertex.position[0];
        tmpArray[index + 1] = vertex.position[1];
        tmpArray[index + 2] = vertex.position[2];
        tmpArray[index + 3] = normal[0];
        tmpArray[index + 4] = normal[1];
        tmpArray[index + 5] = normal[2];
        tmpArray[index + 6] = vertex.texCoords[0];
        tmpArray[index + 7] = vertex.texCoords[1];
    }
    return &tmpArray[0];
}

int* toTriangleArray(std::vector<STriangle> triangles, int tmpArray[], int numOfTriangles) {
    for (int i = 0; i < numOfTriangles; i++) {
        STriangle triangle = triangles[i];

        int index = i * INDICES_PER_TRIANGLE;
        tmpArray[index + 0] = triangle.indices[0];
        tmpArray[index + 1] = triangle.indices[1];
        tmpArray[index + 2] = triangle.indices[2];
    }
    return &tmpArray[0];
}

gfx::BHandle CreateBallMesh(gfx::BHandle &material) {
    std::vector<SVertex> verts;
    // top verts
    verts.push_back({ 0.0f, 1.0f, 0.0f,   0.000f, 0.0f}); // 0
    verts.push_back({ 0.0f, 1.0f, 0.0f,   0.333f, 0.0f});
    verts.push_back({ 0.0f, 1.0f, 0.0f,   0.667f, 0.0f});
    verts.push_back({ 0.0f, 1.0f, 0.0f,   1.000f, 0.0f});
    // center verts
    verts.push_back({ 0.0f, 0.0f,-1.0f,   0.0f , 0.5f}); // 4
    verts.push_back({ 1.0f, 0.0f, 0.0f,   0.25f, 0.5f});
    verts.push_back({ 0.0f, 0.0f, 1.0f,   0.5f , 0.5f});
    verts.push_back({-1.0f, 0.0f, 0.0f,   0.75f, 0.5f});
    verts.push_back({ 0.0f, 0.0f,-1.0f,   1.0f , 0.5f});
    // bottom verts
    verts.push_back({ 0.0f,-1.0f, 0.0f,   0.000f, 1.0f}); // 9
    verts.push_back({ 0.0f,-1.0f, 0.0f,   0.333f, 1.0f});
    verts.push_back({ 0.0f,-1.0f, 0.0f,   0.667f, 1.0f});
    verts.push_back({ 0.0f,-1.0f, 0.0f,   1.000f, 1.0f});

    std::vector<STriangle> triangles;
    // upper half of diamond
    triangles.push_back({ 0,4,5});
    triangles.push_back({ 1,5,6});
    triangles.push_back({ 2,6,7});
    triangles.push_back({ 3,7,8});
    // lower half of diamond
    triangles.push_back({ 9,5,4});
    triangles.push_back({10,6,5});
    triangles.push_back({11,7,6});
    triangles.push_back({12,8,7});

    subdivideTriangles(verts, triangles);
    subdivideTriangles(verts, triangles);

    int numOfVerts = verts.size();
    float *tmpVerts = new float[numOfVerts * FLOATS_IN_VERTEX];

    int numOfTri = triangles.size();
    int *tmpTri = new int[numOfTri * INDICES_PER_TRIANGLE];

    gfx::BHandle mesh = CreateMesh(
        numOfVerts, toVertexArray(verts, tmpVerts, numOfVerts),
        numOfTri, toTriangleArray(triangles, tmpTri, numOfTri),
        material
    );

    delete[] tmpVerts;
    delete[] tmpTri;

    return mesh;
}