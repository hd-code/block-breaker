#include <vector>

#include "yoshix.h"

#include "meshes.hpp"

// -----------------------------------------------------------------------------

struct SEntity {
    gfx::BHandle mesh;
    SVector3 position;
    float worldMatrix[16];
};

class CApplication : public gfx::IApplication {
    public:
        CApplication();
        virtual ~CApplication();

    private:
        float m_FieldOfViewY;             // Vertical view angle of the camera

        // Constant Buffers
        gfx::BHandle CB_VS_WorldMatrix;
        gfx::BHandle CB_VS_ViewProjectionMatrix;

        // Shaders
        gfx::BHandle VertexShader;
        gfx::BHandle PixelShader;

        // Materials
        gfx::BHandle Material;

        // Meshes
        gfx::BHandle TriangleMesh;
        gfx::BHandle CubeMesh;
        gfx::BHandle SphereMesh;

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