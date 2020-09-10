#include "entity.hpp"

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

gfx::BHandle createMaterial(
    int numOfTex, gfx::BHandle textures[],
    int numOfVSB, gfx::BHandle vsBuffers[],
    int numOfPSB, gfx::BHandle psBuffers[],
    gfx::BHandle &vertexShader, gfx::BHandle &pixelShader
) {
    gfx::SMaterialInfo materialInfo;

    materialInfo.m_NumberOfTextures = numOfTex;
    for (int i = 0; i < numOfTex; i++) {
        materialInfo.m_pTextures[i] = textures[i];
    }

    materialInfo.m_NumberOfVertexConstantBuffers = numOfVSB;
    for (int i = 0; i < numOfVSB; i++) {
        materialInfo.m_pVertexConstantBuffers[i] = vsBuffers[i];
    }

    materialInfo.m_NumberOfPixelConstantBuffers = numOfPSB;
    for (int i = 0; i < numOfPSB; i++) {
        materialInfo.m_pPixelConstantBuffers[i] = psBuffers[i];
    }

    materialInfo.m_pVertexShader = vertexShader;
    materialInfo.m_pPixelShader  = pixelShader;

    materialInfo.m_NumberOfInputElements = 3;
    materialInfo.m_InputElements[0].m_pName = "POSITION";
    materialInfo.m_InputElements[0].m_Type  = gfx::SInputElement::Float3;
    materialInfo.m_InputElements[1].m_pName = "NORMAL";
    materialInfo.m_InputElements[1].m_Type  = gfx::SInputElement::Float3;
    materialInfo.m_InputElements[2].m_pName = "TEXCOORD";
    materialInfo.m_InputElements[2].m_Type  = gfx::SInputElement::Float2;

    gfx::BHandle material = nullptr;
    gfx::CreateMaterial(materialInfo, &material);
    return material;
}

// -----------------------------------------------------------------------------

gfx::BHandle createMesh(int numOfVerts, float *verts, int numOfTriangles, int *triangles, gfx::BHandle &material) {
    gfx::SMeshInfo meshInfo;

    meshInfo.m_NumberOfVertices = numOfVerts;
    meshInfo.m_pVertices = verts;

    meshInfo.m_NumberOfIndices = numOfTriangles * INTS_IN_TRIANGLE;
    meshInfo.m_pIndices = triangles;
    
    meshInfo.m_pMaterial = material;

    gfx::BHandle mesh = nullptr;
    gfx::CreateMesh(meshInfo, &mesh);
    return mesh;
}