#pragma once

#include "entities/entity.hpp"
#include "game.hpp"
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
        BHandle Textures[6];

        // Constant Buffers
        BHandle EntityBuffer;
        BHandle GeneralVSBuffer;
        BHandle GeneralPSBuffer;

        // Shaders
        BHandle VertexShader;
        BHandle PixelShader;

        // Materials
        BHandle Material;

        // Meshes
        BHandle BallMesh;
        BHandle BlockMesh;
        BHandle PaddleMesh;

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

        void drawEntity(SEntity &entity);
};