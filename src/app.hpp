#pragma once

#include "entity/entity.hpp"
#include "game/game.hpp"
#include "key.hpp"
#include "yoshix.h"

#include <vector>

// -----------------------------------------------------------------------------

class CApplication : public gfx::IApplication {
    public:
        CApplication();
        virtual ~CApplication();

    private:
        // Textures
        gfx::BHandle textures[int(ETexture::LENGTH)];

        // Constant Buffers
        gfx::BHandle generalVSBuffer;
        gfx::BHandle generalPSBuffer;
        gfx::BHandle entityVSBuffer;
        gfx::BHandle entityPSBuffer;

        // Shaders
        gfx::BHandle vertexShader;
        gfx::BHandle pixelShader;

        // Materials
        gfx::BHandle material;

        // Meshes
        gfx::BHandle ballMesh;
        gfx::BHandle blockMesh;
        gfx::BHandle dialogMesh;
        gfx::BHandle paddleMesh;

        // Game
        CGame* game;
        EKey key;

    private:
        virtual bool InternOnStartup();
        virtual bool InternOnShutdown();
        virtual bool InternOnCreateTextures();
        virtual bool InternOnReleaseTextures();
        virtual bool InternOnCreateConstantBuffers();
        virtual bool InternOnReleaseConstantBuffers();
        virtual bool InternOnCreateShader();
        virtual bool InternOnReleaseShader();
        virtual bool InternOnCreateMaterials();
        virtual bool InternOnReleaseMaterials();
        virtual bool InternOnCreateMeshes();
        virtual bool InternOnReleaseMeshes();
        virtual bool InternOnResize(int width, int height);
        virtual bool InternOnKeyEvent(unsigned int key, bool isDown, bool altDown);
        virtual bool InternOnUpdate();
        virtual bool InternOnFrame();
};