#pragma once

#include <vector>

#include "entity.hpp"
#include "meshes.hpp"
#include "yoshix.h"

using namespace gfx;

// -----------------------------------------------------------------------------

class CApplication : public IApplication {
    public:
        CApplication();
        virtual ~CApplication();

    private:
        float m_FieldOfViewY;             // Vertical view angle of the camera

        // Textures
        BHandle textures[6];

        // Constant Buffers
        BHandle CB_VS_WorldMatrix;
        BHandle CB_VS_ViewProjectionMatrix;

        // Shaders
        BHandle vertexShader;
        BHandle pixelShader;

        // Materials
        BHandle material;

        // Meshes
        BHandle ballMesh;
        BHandle blockMesh;
        BHandle paddleMesh;

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