#pragma once

#include <vector>

#include "entities/entity.hpp"
#include "yoshix.h"

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
        BHandle GeneralBuffer;

        // Shaders
        BHandle VertexShader;
        BHandle PixelShader;

        // Materials
        BHandle Material;

        // Meshes
        BHandle BallMesh;
        BHandle BlockMesh;
        BHandle PaddleMesh;

        // Entities
        std::vector<SEntity> StaticEntities;
        std::vector<SEntity> DynamicEntities;

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
        virtual bool InternOnUpdate();
        virtual bool InternOnFrame();

        void drawEntity(SEntity &entity);
};