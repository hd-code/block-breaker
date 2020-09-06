#include "app.hpp"

// #include "ball.hpp"
#include "entities/block.hpp"
#include "entities/entity.hpp"
#include "entities/paddle.hpp"
#include "yoshix.h"

using namespace gfx;

// -----------------------------------------------------------------------------

CApplication::CApplication() :
    // Textures
      textures()
    // Constant Buffers
    , CB_VS_WorldMatrix(nullptr)
    , CB_VS_ViewProjectionMatrix(nullptr)
    // Shaders
    , vertexShader(nullptr)
    , pixelShader(nullptr)
    // Materials
    , material(nullptr)
    // Meshes
    , ballMesh(nullptr)
    , blockMesh(nullptr)
    , paddleMesh(nullptr)
{}

CApplication::~CApplication() {}

// -----------------------------------------------------------------------------

bool CApplication::InternOnStartup() {
    SEntity entities[] = {
        { &this->blockMesh, TEX_BALL, -5.0f, 0.0f, 0.0f },
        { &this->blockMesh, TEX_BALL, -2.0f, 0.0f, 0.0f },
        { &this->paddleMesh, TEX_BALL, 2.0f, 0.0f, 0.0f },
        { &this->blockMesh, TEX_BALL, 5.0f, 0.0f, 0.0f },
    };

    int numOfEntities = sizeof(entities) / sizeof(SEntity);

    for (int i = 0; i < numOfEntities; i++) {
        this->dynamicEntities.push_back(entities[i]);
    }

    return true;
}

bool CApplication::InternOnShutdown() {
    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateTextures() {
    CreateTexture("..\\data\\earth.dds" , &this->textures[0]);
    CreateTexture("..\\data\\earth.dds" , &this->textures[1]);
    CreateTexture("..\\data\\earth.dds" , &this->textures[2]);
    CreateTexture("..\\data\\earth.dds" , &this->textures[3]);
    CreateTexture("..\\data\\earth.dds" , &this->textures[4]);
    CreateTexture("..\\data\\earth.dds" , &this->textures[5]);

    return true;
}

bool CApplication::InternOnReleaseTextures() {
    for (int i = 0; i < 6; i++) {
        ReleaseTexture(this->textures[i]);
    }

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateConstantBuffers() {
    float dummy[16];

    CreateConstantBuffer(sizeof(dummy), &this->CB_VS_WorldMatrix);
    CreateConstantBuffer(sizeof(dummy), &this->CB_VS_ViewProjectionMatrix);

    return true; 
}

bool CApplication::InternOnReleaseConstantBuffers() {
    ReleaseConstantBuffer(this->CB_VS_WorldMatrix);
    ReleaseConstantBuffer(this->CB_VS_ViewProjectionMatrix);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateShader() {
    CreateVertexShader("..\\src\\shader.fx", "VShader", &this->vertexShader);
    CreatePixelShader ("..\\src\\shader.fx", "PShader", &this->pixelShader);

    return true;
}

bool CApplication::InternOnReleaseShader() {
    ReleaseVertexShader(this->vertexShader);
    ReleasePixelShader (this->pixelShader);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateMaterials() {
    BHandle vsBuffers[2] = { this->CB_VS_WorldMatrix, this->CB_VS_ViewProjectionMatrix};

    this->material = createMaterial(this->textures, 6, vsBuffers, 2, nullptr, 0, this->vertexShader, this->pixelShader);

    return true;
}

bool CApplication::InternOnReleaseMaterials() {
    ReleaseMaterial(this->material);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateMeshes() {
    // this->ballMesh   = createBallMesh(this->material);
    this->blockMesh  = createBlockMesh(this->material);
    this->paddleMesh = createPaddleMesh(this->material);

    return true;
}

bool CApplication::InternOnReleaseMeshes() {
    ReleaseMesh(this->ballMesh);
    ReleaseMesh(this->blockMesh);
    ReleaseMesh(this->paddleMesh);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnResize(int _Width, int _Height) {
    float viewMatrix[16];

    float cameraPosition[3] = { 0.0f, 0.0f, -8.0f };
    float cameraTarget[3] = { 0.0f, 0.0f, 0.0f };
    float cameraUp[3] = { 0.0f, 1.0f, 0.0f };

    GetViewMatrix(cameraPosition, cameraTarget, cameraUp, viewMatrix);

    float projectionMatrix[16];
    GetProjectionMatrix(
        60.0f,
        (float) _Width / (float) _Height,
        0.1f, 100.0f, projectionMatrix
    );

    float viewProjectionMatrix[16];
    MulMatrix(viewMatrix, projectionMatrix, viewProjectionMatrix);
    UploadConstantBuffer(viewProjectionMatrix, this->CB_VS_ViewProjectionMatrix);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnUpdate() {

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnFrame() {
    for (auto &entity : this->staticEntities) {
        UploadConstantBuffer(entity.worldMatrix, this->CB_VS_WorldMatrix);
        DrawMesh(*entity.mesh);
    }

    for (auto &entity : this->dynamicEntities) {
        entity.updateWorldMatrix();
        UploadConstantBuffer(entity.worldMatrix, this->CB_VS_WorldMatrix);
        DrawMesh(*entity.mesh);
    }

    return true;
}