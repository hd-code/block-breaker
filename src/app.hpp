#pragma once

#include "entities/entity.hpp"
#include "game/game.hpp"
#include "key.hpp"
#include "yoshix.h"

#include <vector>

using namespace gfx;

// -----------------------------------------------------------------------------

class CApplication : public IApplication {
    public:
        CApplication();
        virtual ~CApplication();

    private:
        // Textures
        const static int NUM_OF_TEXTURES = 6;
        BHandle textures[NUM_OF_TEXTURES];

        // Constant Buffers
        BHandle entityBuffer;
        BHandle generalVSBuffer;
        BHandle generalPSBuffer;

        // Shaders
        BHandle vertexShader;
        BHandle pixelShader;

        // Materials
        BHandle material;

        // Meshes
        BHandle ballMesh;
        BHandle blockMesh;
        BHandle paddleMesh;

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
        virtual bool InternOnResize(int _Width, int _Height);
        virtual bool InternOnKeyEvent(unsigned int key, bool isDown, bool altDown);
        virtual bool InternOnUpdate();
        virtual bool InternOnFrame();
};