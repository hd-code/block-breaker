#include "app.hpp"

#include "camera.hpp"
#include "entities/ball.hpp"
#include "entities/block.hpp"
#include "entities/entity.hpp"
#include "entities/paddle.hpp"
#include "yoshix.h"

#include <iostream>

using namespace gfx;

// -----------------------------------------------------------------------------

CApplication::CApplication() :
    // Textures
      textures()
    // Constant Buffers
    , generalVSBuffer(nullptr)
    , generalPSBuffer(nullptr)
    , entityVSBuffer(nullptr)
    , entityPSBuffer(nullptr)
    // Shaders
    , vertexShader(nullptr)
    , pixelShader(nullptr)
    // Materials
    , material(nullptr)
    // Meshes
    , ballMesh(nullptr)
    , blockMesh(nullptr)
    , paddleMesh(nullptr)
    // Game
    , game(nullptr)
    , key(EKey::NONE)
{}

CApplication::~CApplication() {}

// -----------------------------------------------------------------------------

bool CApplication::InternOnStartup() {
    this->game = new CGame(&this->ballMesh, &this->blockMesh, &this->paddleMesh);
    return true;
}

bool CApplication::InternOnShutdown() {
    this->game->~CGame();
    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateTextures() {
    CreateTexture("..\\data\\ball.png", &this->textures[0]);
    CreateTexture("..\\data\\paddle.jpg", &this->textures[1]);
    CreateTexture("..\\data\\bed-rock.jpg", &this->textures[2]);
    CreateTexture("..\\data\\paddle.jpg", &this->textures[3]);
    CreateTexture("..\\data\\block-hard.jpg", &this->textures[4]);
    CreateTexture("..\\data\\block-cracked.jpg", &this->textures[5]);

    return true;
}

bool CApplication::InternOnReleaseTextures() {
    for (int i = 0; i < NUM_OF_TEXTURES; i++) {
        ReleaseTexture(this->textures[i]);
    }

    return true;
}

// -----------------------------------------------------------------------------

struct SGeneralVSBuffer {
    float viewProjectionMatrix[16];
};

struct SGeneralPSBuffer {
    float cameraPosition[3];
    float _padding0;
    float lightDir[3];
    float _padding1;
    float ambientLight[3];
    float _padding2;
};

struct SEntityVSBuffer {
    float worldMatrix[16];
};

struct SEntityPSBuffer {
    float texture;
    float _padding[3];
};

bool CApplication::InternOnCreateConstantBuffers() {
    CreateConstantBuffer(sizeof(SGeneralVSBuffer), &this->generalVSBuffer);
    CreateConstantBuffer(sizeof(SGeneralPSBuffer), &this->generalPSBuffer);
    CreateConstantBuffer(sizeof(SEntityVSBuffer), &this->entityVSBuffer);
    CreateConstantBuffer(sizeof(SEntityPSBuffer), &this->entityPSBuffer);

    return true; 
}

bool CApplication::InternOnReleaseConstantBuffers() {
    ReleaseConstantBuffer(this->generalVSBuffer);
    ReleaseConstantBuffer(this->generalPSBuffer);
    ReleaseConstantBuffer(this->entityVSBuffer);
    ReleaseConstantBuffer(this->entityPSBuffer);

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
    BHandle vsBuffers[2] = { this->generalVSBuffer, this->entityVSBuffer };
    BHandle psBuffers[2] = { this->generalPSBuffer, this->entityPSBuffer };

    this->material = createMaterial(
        NUM_OF_TEXTURES, this->textures,
        2, vsBuffers,
        2, psBuffers,
        this->vertexShader, this->pixelShader
    );

    return true;
}

bool CApplication::InternOnReleaseMaterials() {
    ReleaseMaterial(this->material);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateMeshes() {
    this->ballMesh   = createBallMesh(this->material);
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

bool CApplication::InternOnResize(int width, int height) {
    SCamera cam;
    SLight light;

    float viewMatrix[16];
    GetViewMatrix(cam.position, cam.target, cam.up, viewMatrix);

    float projectionMatrix[16];
    float aspectRatio = (float) width / (float) height;
    GetProjectionMatrix(cam.aperture, aspectRatio, cam.nearClip, cam.farClip, projectionMatrix);

    SGeneralVSBuffer vsBuffer;
    MulMatrix(viewMatrix, projectionMatrix, vsBuffer.viewProjectionMatrix);
    UploadConstantBuffer(&vsBuffer, this->generalVSBuffer);

    SGeneralPSBuffer psBuffer;
    memcpy(psBuffer.cameraPosition, cam.position, sizeof(float)*3);
    GetNormalizedVector(light.direction, psBuffer.lightDir);
    memcpy(psBuffer.ambientLight, light.ambient, sizeof(float)*3);
    UploadConstantBuffer(&psBuffer, this->generalPSBuffer);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnKeyEvent(unsigned int key, bool isDown, bool altDown) {
    switch (key) {
    case KEY_SPACE:
        if (!isDown) { // trigger only when key was released
            this->key = EKey::SPACE;
            return true;
        }
        break;

    case KEY_LEFT:
        this->key = EKey::LEFT;
        break;

    case KEY_RIGHT:
        this->key = EKey::RIGHT;
        break;

    default:
        this->key = EKey::NONE;
    }

    if (!isDown) {
        this->key = EKey::NONE;
    }

    return true;
}

bool CApplication::InternOnUpdate() {
    this->game->onUpdate(this->key);
    if (this->key == EKey::SPACE) {
        this->key = EKey::NONE;
    }
    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnFrame() {
    SEntityVSBuffer vsBuffer;
    SEntityPSBuffer psBuffer;

    std::vector<SEntity*>* entities  = this->game->getEntities();
    
    for (auto entity : *entities) {
        memcpy(vsBuffer.worldMatrix, entity->worldMatrix, sizeof(float)*16);
        psBuffer.texture = float(entity->texture);

        UploadConstantBuffer(&vsBuffer, this->entityVSBuffer);
        UploadConstantBuffer(&psBuffer, this->entityPSBuffer);

        DrawMesh(*entity->mesh);
    }

    return true;
}