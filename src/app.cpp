#include "app.hpp"

#include "entities/ball.hpp"
#include "entities/block.hpp"
#include "entities/entity.hpp"
#include "entities/paddle.hpp"
#include "yoshix.h"

using namespace gfx;

// -----------------------------------------------------------------------------

CApplication::CApplication() :
    // Textures
      Textures()
    // Constant Buffers
    , EntityBuffer(nullptr)
    , GeneralBuffer(nullptr)
    // Shaders
    , VertexShader(nullptr)
    , PixelShader(nullptr)
    // Materials
    , Material(nullptr)
    // Meshes
    , BallMesh(nullptr)
    , BlockMesh(nullptr)
    , PaddleMesh(nullptr)
{}

CApplication::~CApplication() {}

// -----------------------------------------------------------------------------

bool CApplication::InternOnStartup() {
    SEntity entities[] = {
        { &this->BlockMesh, TEX_BALL, -5.0f, 0.0f, 0.0f },
        { &this->BlockMesh, TEX_BALL, -2.0f, 0.0f, 0.0f },
        { &this->PaddleMesh, TEX_BALL, 2.0f, 0.0f, 0.0f },
        { &this->BlockMesh, TEX_BALL, 5.0f, 0.0f, 0.0f },
    };

    int numOfEntities = sizeof(entities) / sizeof(SEntity);

    for (int i = 0; i < numOfEntities; i++) {
        this->DynamicEntities.push_back(entities[i]);
    }

    return true;
}

bool CApplication::InternOnShutdown() {
    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateTextures() {
    CreateTexture("..\\data\\cube.dds" , &this->Textures[0]);
    CreateTexture("..\\data\\earth.dds" , &this->Textures[1]);
    CreateTexture("..\\data\\leaf.dds" , &this->Textures[2]);
    CreateTexture("..\\data\\moon.dds" , &this->Textures[3]);
    CreateTexture("..\\data\\sky.dds" , &this->Textures[4]);
    CreateTexture("..\\data\\wall.dds" , &this->Textures[5]);

    return true;
}

bool CApplication::InternOnReleaseTextures() {
    for (int i = 0; i < 6; i++) {
        ReleaseTexture(this->Textures[i]);
    }

    return true;
}

// -----------------------------------------------------------------------------

struct SEntityBuffer {
    float worldMatrix[16];
    float texture;
    float _padding[3];

    void setWorldMatrix(float worldMatrix[16]) {
        for (int i = 0; i < 16; i++) {
            this->worldMatrix[i] = worldMatrix[i];
        }
    }
};

struct SGeneralBuffer {
    float viewProjectionMatrix[16];
    float cameraPosition[3];
    float _padding;
    float lightDir[3];
};

bool CApplication::InternOnCreateConstantBuffers() {
    CreateConstantBuffer(sizeof(SEntityBuffer), &this->EntityBuffer);
    CreateConstantBuffer(sizeof(SGeneralBuffer), &this->GeneralBuffer);

    return true; 
}

bool CApplication::InternOnReleaseConstantBuffers() {
    ReleaseConstantBuffer(this->EntityBuffer);
    ReleaseConstantBuffer(this->GeneralBuffer);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateShader() {
    CreateVertexShader("..\\src\\shader.fx", "VShader", &this->VertexShader);
    CreatePixelShader ("..\\src\\shader.fx", "PShader", &this->PixelShader);

    return true;
}

bool CApplication::InternOnReleaseShader() {
    ReleaseVertexShader(this->VertexShader);
    ReleasePixelShader (this->PixelShader);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateMaterials() {
    BHandle vsBuffers[2] = { this->EntityBuffer, this->GeneralBuffer};

    this->Material = createMaterial(this->Textures, 6, vsBuffers, 2, nullptr, 0, this->VertexShader, this->PixelShader);

    return true;
}

bool CApplication::InternOnReleaseMaterials() {
    ReleaseMaterial(this->Material);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateMeshes() {
    this->BallMesh   = createBallMesh(this->Material);
    this->BlockMesh  = createBlockMesh(this->Material);
    this->PaddleMesh = createPaddleMesh(this->Material);

    return true;
}

bool CApplication::InternOnReleaseMeshes() {
    ReleaseMesh(this->BallMesh);
    ReleaseMesh(this->BlockMesh);
    ReleaseMesh(this->PaddleMesh);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnResize(int _Width, int _Height) {
    float cameraPosition[3] = { 0.0f, 0.0f, -8.0f };
    float cameraTarget[3]   = { 0.0f, 0.0f, 0.0f };
    float cameraUp[3]       = { 0.0f, 1.0f, 0.0f };

    float lenseAngle = 60.0f;
    float near = 0.1f;
    float far  = 100.0f;
    
    float lightDir[3] = { 1.0f, 10.0f, -10.0f };

    float viewMatrix[16];
    GetViewMatrix(cameraPosition, cameraTarget, cameraUp, viewMatrix);

    float projectionMatrix[16];
    GetProjectionMatrix(lenseAngle, (float) _Width / (float) _Height, near, far, projectionMatrix);

    SGeneralBuffer buffer;

    MulMatrix(viewMatrix, projectionMatrix, buffer.viewProjectionMatrix);
    buffer.cameraPosition[0] = cameraPosition[0];
    buffer.cameraPosition[1] = cameraPosition[1];
    buffer.cameraPosition[2] = cameraPosition[2];
    GetNormalizedVector(lightDir, buffer.lightDir);

    UploadConstantBuffer(&buffer, this->GeneralBuffer);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnUpdate() {

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnFrame() {
    for (auto &entity : this->StaticEntities) {
        this->drawEntity(entity);
    }

    for (auto &entity : this->DynamicEntities) {
        entity.updateWorldMatrix();
        this->drawEntity(entity);
    }

    return true;
}

void CApplication::drawEntity(SEntity &entity) {
    SEntityBuffer buffer;
    buffer.setWorldMatrix(entity.worldMatrix);
    buffer.texture = float(entity.texture);
    UploadConstantBuffer(&buffer, this->EntityBuffer);
    DrawMesh(*entity.mesh);
}