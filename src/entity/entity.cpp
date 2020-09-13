#include "entity.hpp"

#include "../helper/mesh.hpp"
#include "yoshix.h"

// -----------------------------------------------------------------------------

void SEntity::updateWorldMatrix() {
     gfx::GetTranslationMatrix(
        this->position[0],
        this->position[1],
        this->position[2],
        this->worldMatrix
    );
}

// -----------------------------------------------------------------------------

gfx::BHandle CreateCuboidMesh(gfx::BHandle &material, float width, float height, float depth) {
    float x = width  / 2.0f; // half width  length
    float y = height / 2.0f; // half height length
    float z = depth  / 2.0f; // half depth  length

    float vertices[][8] = {
        // front
        { -x, y,-z,    0.0f, 0.0f,-1.0f,   0.0f, 0.0f },
        { -x,-y,-z,    0.0f, 0.0f,-1.0f,   0.0f, 1.0f },
        {  x,-y,-z,    0.0f, 0.0f,-1.0f,   1.0f, 1.0f },
        {  x, y,-z,    0.0f, 0.0f,-1.0f,   1.0f, 0.0f },
        // back
        { -x,-y, z,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f },
        { -x, y, z,    0.0f, 0.0f, 1.0f,   0.0f, 1.0f },
        {  x, y, z,    0.0f, 0.0f, 1.0f,   1.0f, 1.0f },
        {  x,-y, z,    0.0f, 0.0f, 1.0f,   1.0f, 0.0f },
        // top
        { -x, y, z,    0.0f, 1.0f, 0.0f,   0.0f, 0.0f },
        { -x, y,-z,    0.0f, 1.0f, 0.0f,   0.0f, 1.0f },
        {  x, y,-z,    0.0f, 1.0f, 0.0f,   1.0f, 1.0f },
        {  x, y, z,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f },
        // bottom
        { -x,-y,-z,    0.0f,-1.0f, 0.0f,   0.0f, 0.0f },
        { -x,-y, z,    0.0f,-1.0f, 0.0f,   0.0f, 1.0f },
        {  x,-y, z,    0.0f,-1.0f, 0.0f,   1.0f, 1.0f },
        {  x,-y,-z,    0.0f,-1.0f, 0.0f,   1.0f, 0.0f },
        // right
        {  x, y,-z,    1.0f, 0.0f, 0.0f,   0.0f, 0.0f },
        {  x,-y,-z,    1.0f, 0.0f, 0.0f,   0.0f, 1.0f },
        {  x,-y, z,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f },
        {  x, y, z,    1.0f, 0.0f, 0.0f,   1.0f, 0.0f },
        // left
        { -x, y, z,   -1.0f, 0.0f, 0.0f,   0.0f, 0.0f },
        { -x,-y, z,   -1.0f, 0.0f, 0.0f,   0.0f, 1.0f },
        { -x,-y,-z,   -1.0f, 0.0f, 0.0f,   1.0f, 1.0f },
        { -x, y,-z,   -1.0f, 0.0f, 0.0f,   1.0f, 0.0f },
    };

    int triangles[][3] = {
        {  0, 1, 2 }, {  0, 2, 3 },
        {  4, 5, 6 }, {  4, 6, 7 },
        {  8, 9,10 }, {  8,10,11 },
        { 12,13,14 }, { 12,14,15 },
        { 16,17,18 }, { 16,18,19 },
        { 20,21,22 }, { 20,22,23 },
    };

    return CreateMesh(24, &vertices[0][0], 12, &triangles[0][0], material);
}