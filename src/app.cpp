#include "app.hpp"

#include "entities/ball.hpp"
#include "entities/block.hpp"
#include "entities/entity.hpp"
#include "entities/paddle.hpp"
#include "game.hpp"
#include "yoshix.h"

using namespace gfx;

// -----------------------------------------------------------------------------

CApplication::CApplication() :
    // Textures
      Textures()
    // Constant Buffers
    , EntityBuffer(nullptr)
    , GeneralVSBuffer(nullptr)
    , GeneralPSBuffer(nullptr)
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
    this->game = new CGame();
    return true;
}

bool CApplication::InternOnShutdown() {
    this->game->~CGame();
    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateTextures() {
    CreateTexture("..\\data\\ball.png" , &this->Textures[0]);
    CreateTexture("..\\data\\paddle.jpg" , &this->Textures[1]);
    CreateTexture("..\\data\\bed-rock.jpg" , &this->Textures[2]);
    CreateTexture("..\\data\\moon.dds" , &this->Textures[3]);
    CreateTexture("..\\data\\block-hard.jpg" , &this->Textures[4]);
    CreateTexture("..\\data\\block-cracked.jpg" , &this->Textures[5]);

    return true;
}

bool CApplication::InternOnReleaseTextures() {
    for (int i = 0; i < 6; i++) {
        ReleaseTexture(this->Textures[i]);
    }

    return true;
}

// -----------------------------------------------------------------------------

struct SGeneralVSBuffer {
    float viewProjectionMatrix[16];
};

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

struct SGeneralPSBuffer {
    float cameraPosition[3];
    float _padding0;
    float lightDir[3];
    float _padding1;
};

bool CApplication::InternOnCreateConstantBuffers() {
    CreateConstantBuffer(sizeof(SGeneralVSBuffer), &this->GeneralVSBuffer);
    CreateConstantBuffer(sizeof(SEntityBuffer), &this->EntityBuffer);
    CreateConstantBuffer(sizeof(SGeneralPSBuffer), &this->GeneralPSBuffer);

    return true; 
}

bool CApplication::InternOnReleaseConstantBuffers() {
    ReleaseConstantBuffer(this->GeneralVSBuffer);
    ReleaseConstantBuffer(this->EntityBuffer);
    ReleaseConstantBuffer(this->GeneralPSBuffer);

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
    BHandle vsBuffers[2] = { this->GeneralVSBuffer, this->EntityBuffer };
    BHandle psBuffers[1] = { this->GeneralPSBuffer };

    this->Material = createMaterial(
        6, this->Textures,
        2, vsBuffers,
        1, psBuffers,
        this->VertexShader, this->PixelShader
    );

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
    
    float lightDir[3] = { -1.0f, -0.7f, 2.0f };

    float viewMatrix[16];
    GetViewMatrix(cameraPosition, cameraTarget, cameraUp, viewMatrix);

    float projectionMatrix[16];
    GetProjectionMatrix(lenseAngle, (float) _Width / (float) _Height, near, far, projectionMatrix);

    SGeneralVSBuffer vsBuffer;
    MulMatrix(viewMatrix, projectionMatrix, vsBuffer.viewProjectionMatrix);
    UploadConstantBuffer(&vsBuffer, this->GeneralVSBuffer);

    SGeneralPSBuffer psBuffer;
    psBuffer.cameraPosition[0] = cameraPosition[0];
    psBuffer.cameraPosition[1] = cameraPosition[1];
    psBuffer.cameraPosition[2] = cameraPosition[2];
    GetNormalizedVector(lightDir, psBuffer.lightDir);
    UploadConstantBuffer(&psBuffer, this->GeneralPSBuffer);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnKeyEvent(unsigned int key, bool isDown, bool altDown) {
    switch (key) {
    case 32: // space
        this->key = EKey::SPACE;
        break;
    case 27: // left arrow
        this->key = EKey::LEFT;
        break;
    case 26: // right arrow
        this->key = EKey::RIGHT;
    default:
        this->key = EKey::NONE;
    }

    return true;
}

bool CApplication::InternOnUpdate() {
    this->game->onUpdate(this->key);
    this->key = EKey::NONE;

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnFrame() {
    std::vector<SEntity*> staticEntities  = this->game->getStaticEntities();
    std::vector<SEntity*> dynamicEntities = this->game->getDynamicEntities();;

    for (auto &entity : staticEntities) {
        this->drawEntity(*entity);
    }

    for (auto &entity : dynamicEntities) {
        entity->updateWorldMatrix();
        this->drawEntity(*entity);
    }

    return true;
}

// -----------------------------------------------------------------------------

void CApplication::drawEntity(SEntity &entity) {
    SEntityBuffer buffer;
    buffer.setWorldMatrix(entity.worldMatrix);
    buffer.texture = float(entity.texture);
    UploadConstantBuffer(&buffer, this->EntityBuffer);
    DrawMesh(*entity.mesh);
}