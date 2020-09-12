#include "material.hpp"

// -----------------------------------------------------------------------------

gfx::BHandle CreateMaterial(
    int numOfTex, gfx::BHandle textures[],
    int numOfVSB, gfx::BHandle vsBuffers[],
    int numOfPSB, gfx::BHandle psBuffers[],
    int numOfInputs, gfx::SInputElement inputElements[],
    gfx::BHandle &vertexShader, gfx::BHandle &pixelShader)
{
    gfx::SMaterialInfo materialInfo;

    materialInfo.m_NumberOfTextures = numOfTex;
    for (int i = 0; i < numOfTex; i++)
    {
        materialInfo.m_pTextures[i] = textures[i];
    }

    materialInfo.m_NumberOfVertexConstantBuffers = numOfVSB;
    for (int i = 0; i < numOfVSB; i++)
    {
        materialInfo.m_pVertexConstantBuffers[i] = vsBuffers[i];
    }

    materialInfo.m_NumberOfPixelConstantBuffers = numOfPSB;
    for (int i = 0; i < numOfPSB; i++)
    {
        materialInfo.m_pPixelConstantBuffers[i] = psBuffers[i];
    }

    materialInfo.m_NumberOfInputElements = numOfInputs;
    for (int i = 0; i < numOfInputs; i++)
    {
        materialInfo.m_InputElements[i] = inputElements[i];
    }

    materialInfo.m_pVertexShader = vertexShader;
    materialInfo.m_pPixelShader = pixelShader;

    gfx::BHandle material = nullptr;
    gfx::CreateMaterial(materialInfo, &material);
    return material;
}