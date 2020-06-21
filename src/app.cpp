#include "app.hpp"

#include "meshes.hpp"

// -----------------------------------------------------------------------------

CApplication::CApplication()
    : m_FieldOfViewY(60.0f)        // Set the vertical view angle of the camera to 60 degrees.
    , CB_VS_WorldMatrix(nullptr)
    , CB_VS_ViewProjectionMatrix(nullptr)
    , VertexShader(nullptr)
    , PixelShader(nullptr)
    , Material(nullptr)
    , TriangleMesh(nullptr)
    , CubeMesh(nullptr)
    , SphereMesh(nullptr)
{}

// -----------------------------------------------------------------------------

CApplication::~CApplication() {}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateTextures() {
    // -----------------------------------------------------------------------------
    // Path is "..\\data\\images\\texture.dds"
    // -----------------------------------------------------------------------------
    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnReleaseTextures() {
    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateConstantBuffers() {
    float dummy[16];

    gfx::CreateConstantBuffer(sizeof(dummy), &this->CB_VS_WorldMatrix);
    gfx::CreateConstantBuffer(sizeof(dummy), &this->CB_VS_ViewProjectionMatrix);

    return true; 
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnReleaseConstantBuffers() {
    gfx::ReleaseConstantBuffer(this->CB_VS_WorldMatrix);
    gfx::ReleaseConstantBuffer(this->CB_VS_ViewProjectionMatrix);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateShader() {
    gfx::CreateVertexShader("..\\src\\shader.fx", "VShader", &this->VertexShader);
    gfx::CreatePixelShader("..\\src\\shader.fx", "PShader", &this->PixelShader);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnReleaseShader() {
    gfx::ReleaseVertexShader(this->VertexShader);
    gfx::ReleasePixelShader(this->PixelShader);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateMaterials() {
    gfx::SMaterialInfo materialInfo;

    materialInfo.m_NumberOfTextures = 0;

    materialInfo.m_NumberOfVertexConstantBuffers = 2;
    materialInfo.m_pVertexConstantBuffers[0] = this->CB_VS_WorldMatrix;
    materialInfo.m_pVertexConstantBuffers[1] = this->CB_VS_ViewProjectionMatrix;

    materialInfo.m_NumberOfPixelConstantBuffers = 0;

    materialInfo.m_pVertexShader = this->VertexShader;
    materialInfo.m_pPixelShader  = this->PixelShader;

    materialInfo.m_NumberOfInputElements = 3;
    materialInfo.m_InputElements[0].m_pName = "POSITION";
    materialInfo.m_InputElements[0].m_Type  = gfx::SInputElement::Float3;
    materialInfo.m_InputElements[1].m_pName = "TEXCOORD";
    materialInfo.m_InputElements[1].m_Type  = gfx::SInputElement::Float2;
    materialInfo.m_InputElements[2].m_pName = "NORMAL";
    materialInfo.m_InputElements[2].m_Type  = gfx::SInputElement::Float3;

    gfx::CreateMaterial(materialInfo, &this->Material);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnReleaseMaterials() {
    gfx::ReleaseMaterial(this->Material);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateMeshes() {
    CreateTriangleMesh(this->Material, this->TriangleMesh);
    CreateCubeMesh(this->Material, this->CubeMesh);
    CreateSphereMesh(this->Material, this->SphereMesh);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnReleaseMeshes() {
    gfx::ReleaseMesh(this->TriangleMesh);
    gfx::ReleaseMesh(this->CubeMesh);
    gfx::ReleaseMesh(this->SphereMesh);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnResize(int _Width, int _Height) {
    float viewMatrix[16];

    float cameraPosition[3] = { 0.0f, 0.0f, -8.0f };
    float cameraTarget[3] = { 0.0f, 0.0f, 0.0f };
    float cameraUp[3] = { 0.0f, 1.0f, 0.0f };

    gfx::GetViewMatrix(cameraPosition, cameraTarget, cameraUp, viewMatrix);

    float projectionMatrix[16];
    gfx::GetProjectionMatrix(
        this->m_FieldOfViewY,
        (float) _Width / (float) _Height,
        0.1f, 100.0f, projectionMatrix
    );

    float viewProjectionMatrix[16];
    gfx::MulMatrix(viewMatrix, projectionMatrix, viewProjectionMatrix);
    gfx::UploadConstantBuffer(viewProjectionMatrix, this->CB_VS_ViewProjectionMatrix);

    return true;
}

// -----------------------------------------------------------------------------

float angle = 0.0f;
float angleStep = 1.0f;
float maxAngle = 360.0f;

bool CApplication::InternOnUpdate() {
    angle += angleStep;
    if (angle > maxAngle) angle = 0;

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnFrame() {
    float WorldMatrix[16];
    gfx::GetRotationXMatrix(angle, WorldMatrix);
    gfx::UploadConstantBuffer(WorldMatrix, this->CB_VS_WorldMatrix);

    gfx::DrawMesh(this->TriangleMesh);

    return true;
}