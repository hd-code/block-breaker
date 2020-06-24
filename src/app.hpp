#pragma once

// -----------------------------------------------------------------------------

#include "entity.hpp"
#include "meshes.hpp"

#include "yoshix.h"

#include <vector>

using namespace gfx;

// -----------------------------------------------------------------------------

class CApplication : public IApplication {
    public:
        CApplication();
        virtual ~CApplication();

    private:
        float m_FieldOfViewY;             // Vertical view angle of the camera

        // Textures
        BHandle Texture;

        // Constant Buffers
        BHandle CB_VS_WorldMatrix;
        BHandle CB_VS_ViewProjectionMatrix;

        // Shaders
        BHandle VertexShader;
        BHandle PixelShader;

        // Materials
        BHandle Material;

        // Meshes
        BHandle TriangleMesh;
        BHandle CubeMesh;
        BHandle SphereMesh;

        // Entities
        std::vector<SEntity> staticEntities;
        std::vector<SEntity> dynamicEntities;

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
};