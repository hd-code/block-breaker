#include "app.hpp"

#include "camera.hpp"
#include "entities/ball.hpp"
#include "entities/block.hpp"
#include "entities/entity.hpp"
#include "entities/paddle.hpp"
#include "helper/material.hpp"
#include "yoshix.h"

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
    int numOfTextures = int(ETexture::LENGTH);
    const char* TEXTURES[] = {
        "..\\data\\ball.png",
        "..\\data\\paddle.jpg",
        "..\\data\\bed-rock.jpg",
        "..\\data\\block.jpg",
        "..\\data\\block-hard.jpg",
        "..\\data\\block-cracked.jpg"
    };

    for (int i = 0; i < numOfTextures; i++) {
        gfx::CreateTexture(TEXTURES[i], &this->textures[i]);
    }

    return true;
}

bool CApplication::InternOnReleaseTextures() {
    for (int i = 0; i < int(ETexture::LENGTH); i++) {
        gfx::ReleaseTexture(this->textures[i]);
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
    float specularExponent;
    float _padding[2];
};

bool CApplication::InternOnCreateConstantBuffers() {
    gfx::CreateConstantBuffer(sizeof(SGeneralVSBuffer), &this->generalVSBuffer);
    gfx::CreateConstantBuffer(sizeof(SGeneralPSBuffer), &this->generalPSBuffer);
    gfx::CreateConstantBuffer(sizeof(SEntityVSBuffer), &this->entityVSBuffer);
    gfx::CreateConstantBuffer(sizeof(SEntityPSBuffer), &this->entityPSBuffer);

    return true; 
}

bool CApplication::InternOnReleaseConstantBuffers() {
    gfx::ReleaseConstantBuffer(this->generalVSBuffer);
    gfx::ReleaseConstantBuffer(this->generalPSBuffer);
    gfx::ReleaseConstantBuffer(this->entityVSBuffer);
    gfx::ReleaseConstantBuffer(this->entityPSBuffer);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateShader() {
    gfx::CreateVertexShader("..\\src\\shader.fx", "VShader", &this->vertexShader);
    gfx::CreatePixelShader ("..\\src\\shader.fx", "PShader", &this->pixelShader);

    return true;
}

bool CApplication::InternOnReleaseShader() {
    gfx::ReleaseVertexShader(this->vertexShader);
    gfx::ReleasePixelShader (this->pixelShader);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateMaterials() {
    gfx::BHandle vsBuffers[2] = { this->generalVSBuffer, this->entityVSBuffer };
    gfx::BHandle psBuffers[2] = { this->generalPSBuffer, this->entityPSBuffer };

    const int NUM_OF_INPUTS = 3;
    gfx::SInputElement INPUT_ELEMENTS[] = {
        "POSITION", gfx::SInputElement::Float3,
        "NORMAL",   gfx::SInputElement::Float3,
        "TEXCOORD", gfx::SInputElement::Float2,
    };

    this->material = CreateMaterial(
        int(ETexture::LENGTH), this->textures,
        2, vsBuffers,
        2, psBuffers,
        NUM_OF_INPUTS, INPUT_ELEMENTS,
        this->vertexShader, this->pixelShader
    );

    return true;
}

bool CApplication::InternOnReleaseMaterials() {
    gfx::ReleaseMaterial(this->material);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateMeshes() {
    this->ballMesh   = CreateBallMesh(this->material);
    this->blockMesh  = CreateBlockMesh(this->material);
    this->paddleMesh = CreatePaddleMesh(this->material);

    return true;
}

bool CApplication::InternOnReleaseMeshes() {
    gfx::ReleaseMesh(this->ballMesh);
    gfx::ReleaseMesh(this->blockMesh);
    gfx::ReleaseMesh(this->paddleMesh);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnResize(int width, int height) {
    SCamera cam;
    SLight light;

    float viewMatrix[16];
    gfx::GetViewMatrix(cam.position, cam.target, cam.up, viewMatrix);

    float projectionMatrix[16];
    float aspectRatio = (float) width / (float) height;
    gfx::GetProjectionMatrix(cam.aperture, aspectRatio, cam.nearClip, cam.farClip, projectionMatrix);

    SGeneralVSBuffer vsBuffer;
    gfx::MulMatrix(viewMatrix, projectionMatrix, vsBuffer.viewProjectionMatrix);
    gfx::UploadConstantBuffer(&vsBuffer, this->generalVSBuffer);

    SGeneralPSBuffer psBuffer;
    memcpy(psBuffer.cameraPosition, cam.position, sizeof(float)*3);
    gfx::GetNormalizedVector(light.direction, psBuffer.lightDir);
    memcpy(psBuffer.ambientLight, light.ambient, sizeof(float)*3);
    gfx::UploadConstantBuffer(&psBuffer, this->generalPSBuffer);

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
        psBuffer.specularExponent = entity->specularExponent;

        gfx::UploadConstantBuffer(&vsBuffer, this->entityVSBuffer);
        gfx::UploadConstantBuffer(&psBuffer, this->entityPSBuffer);

        gfx::DrawMesh(*entity->mesh);
    }

    return true;
}