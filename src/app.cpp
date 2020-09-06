#include "app.hpp"

#include "entity.hpp"
#include "yoshix.h"

using namespace gfx;

// -----------------------------------------------------------------------------

CApplication::CApplication()
    : m_FieldOfViewY(60.0f)        // Set the vertical view angle of the camera to 60 degrees.
    // Textures
    , textures({nullptr, nullptr, nullptr, nullptr, nullptr, nullptr})
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
    // SEntity entities[] = {
    //     { &this->TriangleMesh, -5.0f, 0.0f, 0.0f },
    //     { &this->GeoSphereMesh, -2.0f, 0.0f, 0.0f },
    //     { &this->SphereMesh, 2.0f, 0.0f, 0.0f },
    //     { &this->CubeMesh, 5.0f, 0.0f, 0.0f },
    // };

    // int numOfEntities = sizeof(entities) / sizeof(SEntity);

    // for (int i = 0; i < numOfEntities; i++) {
    //     this->dynamicEntities.push_back(entities[i]);
    // }

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
/*
// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateMeshes() {
    CreateTriangleMesh(this->Material, this->TriangleMesh);
    CreateCubeMesh(this->Material, this->CubeMesh);
    CreateSphereMesh(this->Material, this->SphereMesh);

    return true;
}

bool CApplication::InternOnReleaseMeshes() {
    ReleaseMesh(this->TriangleMesh);
    ReleaseMesh(this->CubeMesh);
    ReleaseMesh(this->SphereMesh);

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
        this->m_FieldOfViewY,
        (float) _Width / (float) _Height,
        0.1f, 100.0f, projectionMatrix
    );

    float viewProjectionMatrix[16];
    MulMatrix(viewMatrix, projectionMatrix, viewProjectionMatrix);
    UploadConstantBuffer(viewProjectionMatrix, this->CB_VS_ViewProjectionMatrix);

    return true;
}

// -----------------------------------------------------------------------------

// temporary stuff
// just to see, how the meshes look like, will soon be replaced by the logic
float angle = 0.0f;
float angleStep = 1.1f;
float maxAngle = 360.0f;

bool CApplication::InternOnUpdate() {
    angle += angleStep;
    if (angle > maxAngle) angle = 0;

    return true;
}
// ---------------

// -----------------------------------------------------------------------------

bool CApplication::InternOnFrame() {
    for (auto &entity : this->staticEntities) {
        UploadConstantBuffer(entity.worldMatrix, this->CB_VS_WorldMatrix);
        DrawMesh(*entity.mesh);
    }

    // temporary stuff
    float RotationXMatrix[16];
    float RotationYMatrix[16];

    float WorldMatrix[16];
    // ---------------

    for (auto &entity : this->dynamicEntities) {
        updateWorldMatrix(entity);

        // temporary stuff
        GetRotationXMatrix(angle, RotationXMatrix);
        GetRotationYMatrix(angle * 0.9f, RotationYMatrix);

        MulMatrix(RotationXMatrix, RotationYMatrix, WorldMatrix);
        MulMatrix(WorldMatrix, entity.worldMatrix, WorldMatrix);
        // ---------------

        UploadConstantBuffer(WorldMatrix, this->CB_VS_WorldMatrix);

        DrawMesh(*entity.mesh);
    }

    return true;
}
//*/