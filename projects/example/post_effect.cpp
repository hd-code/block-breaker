
#include "yoshix.h"

#include <math.h>

using namespace gfx;

struct SVertexBuffer
{
    float m_ViewProjectionMatrix[16];       // Result of view matrix * projection matrix.
    float m_WorldMatrix[16];                // The world matrix to transform a mesh from local space to world space.
    float m_ScreenMatrix[16];               // A special view projection matrix to project a rectangular mesh onto a complete render target.
};

// -----------------------------------------------------------------------------

struct SPixelBuffer
{
    float m_NearFar[4];                     // Near and far distance of the view frustum, the other two components are wasted.
};

// -----------------------------------------------------------------------------

class CApplication : public IApplication
{
    public:

        CApplication();
        virtual ~CApplication();

    private:

        float   m_Near;                     // Near distance of the view frustum.
        float   m_Far;                      // Far distance of the view frustum-
        float   m_FieldOfViewY;             // Vertical view angle of the camera

        float   m_AngleY;                   // The rotation angle of the mesh around the y-axis.

        float   m_ViewMatrix[16];           // The view matrix to transform a mesh from world space into view space.
        float   m_ProjectionMatrix[16];     // The projection matrix to transform a mesh from view space into clip space.

        BHandle m_pDepthTarget;             // The depth render target of the GBuffer.
        BHandle m_pNormalTarget;            // The normal render target of the GBuffer.

        BHandle m_pColorTarget;             // An extra color render target to render the scene into.

        BHandle m_pVertexConstantBuffer;    // A vertex constant buffer to pass the matrices to the vertex shader.
        BHandle m_pPixelConstantBuffer;     // A pixel constant buffer to pass near and far view distance to the pixel shader.

        BHandle m_pGBufferVertexShader;     // A vertex shader to write depth and normal into the GBuffer.
        BHandle m_pGBufferPixelShader;      // A pixel shader to write depth and normal into the GBuffer.
        BHandle m_pGBufferMaterial;         // A material which uses the GBuffer vertex and pixel shader to fill the GBuffer.
        BHandle m_pGBufferMesh;             // The cube with GBuffer material.

        BHandle m_pTexture;                 // A texture to cover the cube.
        BHandle m_pVertexShader;            // A vertex shader to draw the textured cube.
        BHandle m_pPixelShader;             // A pixel shader to draw the textured cube.
        BHandle m_pMaterial;                // A material which uses the standard vertex and pixel shader to draw the cube.
        BHandle m_pMesh;                    // The cube with standard material.

        BHandle m_pPostVertexShader;        // A vertex shader to project a rectangular mesh onto the frame buffer.
        BHandle m_pPostPixelShader;         // A pixel shader to calculate the post effect.
        BHandle m_pPostMaterial;            // A material using the post effect shader.
        BHandle m_pPostMesh;                // A single quad.

    private:

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

// -----------------------------------------------------------------------------

CApplication::CApplication()
    : m_Near                 (0.1f)
    , m_Far                  (20.0f)
    , m_FieldOfViewY         (60.0f)
    , m_AngleY               (0.0f)
    , m_pDepthTarget         (nullptr)
    , m_pNormalTarget        (nullptr)
    , m_pColorTarget         (nullptr)
    , m_pVertexConstantBuffer(nullptr)
    , m_pPixelConstantBuffer (nullptr)
    , m_pGBufferVertexShader (nullptr)
    , m_pGBufferPixelShader  (nullptr)
    , m_pGBufferMaterial     (nullptr)
    , m_pGBufferMesh         (nullptr)
    , m_pPostVertexShader    (nullptr)
    , m_pPostPixelShader     (nullptr)
    , m_pPostMaterial        (nullptr)
    , m_pPostMesh            (nullptr)
    , m_pTexture             (nullptr)
    , m_pVertexShader        (nullptr)
    , m_pPixelShader         (nullptr)
    , m_pMaterial            (nullptr)
    , m_pMesh                (nullptr)
{
}

// -----------------------------------------------------------------------------

CApplication::~CApplication()
{
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateTextures()
{
    // -----------------------------------------------------------------------------
    // Create the render targets. We need two render targets for the GBuffer (depth
    // and normal) and one extra color render target. The last one is necessary
    // because we cannot pass the frame buffer as a texture to a pixel shader.
    // -----------------------------------------------------------------------------
    CreateDepthTarget(&m_pDepthTarget);
    CreateColorTarget(&m_pNormalTarget);
    CreateColorTarget(&m_pColorTarget);

    CreateTexture("..\\data\\images\\cube.dds", &m_pTexture);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnReleaseTextures()
{
    ReleaseTexture(m_pDepthTarget);
    ReleaseTexture(m_pNormalTarget);
    ReleaseTexture(m_pColorTarget);
    ReleaseTexture(m_pTexture);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateConstantBuffers()
{
    CreateConstantBuffer(sizeof(SVertexBuffer), &m_pVertexConstantBuffer);
    CreateConstantBuffer(sizeof(SPixelBuffer ), &m_pPixelConstantBuffer);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnReleaseConstantBuffers()
{
    ReleaseConstantBuffer(m_pVertexConstantBuffer);
    ReleaseConstantBuffer(m_pPixelConstantBuffer);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateShader()
{
    CreateVertexShader("..\\data\\shader\\post_effect.fx", "VSGBufferShader", &m_pGBufferVertexShader);
    CreatePixelShader ("..\\data\\shader\\post_effect.fx", "PSGBufferShader", &m_pGBufferPixelShader);
    CreateVertexShader("..\\data\\shader\\post_effect.fx", "VSShader"       , &m_pVertexShader);
    CreatePixelShader ("..\\data\\shader\\post_effect.fx", "PSShader"       , &m_pPixelShader);
    CreateVertexShader("..\\data\\shader\\post_effect.fx", "VSPostShader"   , &m_pPostVertexShader);
    CreatePixelShader ("..\\data\\shader\\post_effect.fx", "PSPostShader"   , &m_pPostPixelShader);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnReleaseShader()
{
    ReleaseVertexShader(m_pGBufferVertexShader);
    ReleasePixelShader (m_pGBufferPixelShader);
    ReleaseVertexShader(m_pVertexShader);
    ReleasePixelShader (m_pPixelShader);
    ReleaseVertexShader(m_pPostVertexShader);
    ReleasePixelShader (m_pPostPixelShader);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateMaterials()
{
    SMaterialInfo MaterialInfo;

    // -----------------------------------------------------------------------------
    // Material to render the cube into the GBuffer. The texture coordinates are not
    // used in the GBuffer shader but we do not want to model the cube twice with
    // different vertex layouts.
    // -----------------------------------------------------------------------------
    MaterialInfo.m_NumberOfTextures              = 0;
    MaterialInfo.m_NumberOfVertexConstantBuffers = 1;
    MaterialInfo.m_pVertexConstantBuffers[0]     = m_pVertexConstantBuffer;
    MaterialInfo.m_NumberOfPixelConstantBuffers  = 0;
    MaterialInfo.m_pVertexShader                 = m_pGBufferVertexShader;
    MaterialInfo.m_pPixelShader                  = m_pGBufferPixelShader;
    MaterialInfo.m_NumberOfInputElements         = 3;
    MaterialInfo.m_InputElements[0].m_pName      = "POSITION"; 
    MaterialInfo.m_InputElements[0].m_Type       = SInputElement::Float3;
    MaterialInfo.m_InputElements[1].m_pName      = "NORMAL";   
    MaterialInfo.m_InputElements[1].m_Type       = SInputElement::Float3;
    MaterialInfo.m_InputElements[2].m_pName      = "TEXCOORD"; 
    MaterialInfo.m_InputElements[2].m_Type       = SInputElement::Float2;

    CreateMaterial(MaterialInfo, &m_pGBufferMaterial);

    // -----------------------------------------------------------------------------
    // Standard material to render the cube into a color target.
    // -----------------------------------------------------------------------------
    MaterialInfo.m_NumberOfTextures              = 1;
    MaterialInfo.m_pTextures[0]                  = m_pTexture;
    MaterialInfo.m_NumberOfVertexConstantBuffers = 1;
    MaterialInfo.m_pVertexConstantBuffers[0]     = m_pVertexConstantBuffer;
    MaterialInfo.m_NumberOfPixelConstantBuffers  = 0;
    MaterialInfo.m_pVertexShader                 = m_pVertexShader;
    MaterialInfo.m_pPixelShader                  = m_pPixelShader;
    MaterialInfo.m_NumberOfInputElements         = 3;
    MaterialInfo.m_InputElements[0].m_pName      = "POSITION"; 
    MaterialInfo.m_InputElements[0].m_Type       = SInputElement::Float3;
    MaterialInfo.m_InputElements[1].m_pName      = "NORMAL";   
    MaterialInfo.m_InputElements[1].m_Type       = SInputElement::Float3;
    MaterialInfo.m_InputElements[2].m_pName      = "TEXCOORD"; 
    MaterialInfo.m_InputElements[2].m_Type       = SInputElement::Float2;

    CreateMaterial(MaterialInfo, &m_pMaterial);

    // -----------------------------------------------------------------------------
    // Material to render the post effect. Note that this material has the three
    // render targets as texture input. The pixel shader of the post effect is able
    // to use the depth and normals of the GBuffer and also the final color image of
    // the rendered scene.
    // -----------------------------------------------------------------------------
    MaterialInfo.m_NumberOfTextures              = 3;
    MaterialInfo.m_pTextures[0]                  = m_pDepthTarget;
    MaterialInfo.m_pTextures[1]                  = m_pColorTarget;
    MaterialInfo.m_pTextures[2]                  = m_pNormalTarget;
    MaterialInfo.m_NumberOfVertexConstantBuffers = 1;
    MaterialInfo.m_pVertexConstantBuffers[0]     = m_pVertexConstantBuffer;
    MaterialInfo.m_NumberOfPixelConstantBuffers  = 1;
    MaterialInfo.m_pPixelConstantBuffers[0]      = m_pPixelConstantBuffer;
    MaterialInfo.m_pVertexShader                 = m_pPostVertexShader;
    MaterialInfo.m_pPixelShader                  = m_pPostPixelShader;
    MaterialInfo.m_NumberOfInputElements         = 1;
    MaterialInfo.m_InputElements[0].m_pName      = "POSITION"; 
    MaterialInfo.m_InputElements[0].m_Type       = SInputElement::Float3;

    CreateMaterial(MaterialInfo, &m_pPostMaterial);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnReleaseMaterials()
{
    ReleaseMaterial(m_pGBufferMaterial);
    ReleaseMaterial(m_pMaterial);
    ReleaseMaterial(m_pPostMaterial);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateMeshes()
{
    // -----------------------------------------------------------------------------
    // The cube as interleaved array with position, normal, and texture coordinates.
    // -----------------------------------------------------------------------------
    const float HalfEdgeLength = 2.0f;

    float U[] =
    {
        0.0f / 4.0f,
        1.0f / 4.0f,
        2.0f / 4.0f,
        3.0f / 4.0f,
        4.0f / 4.0f,
    };

    float V[] =
    {
        3.0f / 3.0f,
        2.0f / 3.0f,
        1.0f / 3.0f,
        0.0f / 3.0f,
    };

    float CubeVertices[][8] =
    {
        { -HalfEdgeLength, -HalfEdgeLength, -HalfEdgeLength,  0.0f,  0.0f, -1.0f, U[1], V[1], },
        {  HalfEdgeLength, -HalfEdgeLength, -HalfEdgeLength,  0.0f,  0.0f, -1.0f, U[2], V[1], },
        {  HalfEdgeLength,  HalfEdgeLength, -HalfEdgeLength,  0.0f,  0.0f, -1.0f, U[2], V[2], },
        { -HalfEdgeLength,  HalfEdgeLength, -HalfEdgeLength,  0.0f,  0.0f, -1.0f, U[1], V[2], },
                                                                                                        
        {  HalfEdgeLength, -HalfEdgeLength, -HalfEdgeLength,  1.0f,  0.0f,  0.0f, U[2], V[1], },
        {  HalfEdgeLength, -HalfEdgeLength,  HalfEdgeLength,  1.0f,  0.0f,  0.0f, U[3], V[1], },
        {  HalfEdgeLength,  HalfEdgeLength,  HalfEdgeLength,  1.0f,  0.0f,  0.0f, U[3], V[2], },
        {  HalfEdgeLength,  HalfEdgeLength, -HalfEdgeLength,  1.0f,  0.0f,  0.0f, U[2], V[2], },
                                                                                                        
        {  HalfEdgeLength, -HalfEdgeLength,  HalfEdgeLength,  0.0f,  0.0f,  1.0f, U[3], V[1], },
        { -HalfEdgeLength, -HalfEdgeLength,  HalfEdgeLength,  0.0f,  0.0f,  1.0f, U[4], V[1], },
        { -HalfEdgeLength,  HalfEdgeLength,  HalfEdgeLength,  0.0f,  0.0f,  1.0f, U[4], V[2], },
        {  HalfEdgeLength,  HalfEdgeLength,  HalfEdgeLength,  0.0f,  0.0f,  1.0f, U[3], V[2], },
                                                                                                       
        { -HalfEdgeLength, -HalfEdgeLength,  HalfEdgeLength, -1.0f,  0.0f,  0.0f, U[0], V[1], },
        { -HalfEdgeLength, -HalfEdgeLength, -HalfEdgeLength, -1.0f,  0.0f,  0.0f, U[1], V[1], },
        { -HalfEdgeLength,  HalfEdgeLength, -HalfEdgeLength, -1.0f,  0.0f,  0.0f, U[1], V[2], },
        { -HalfEdgeLength,  HalfEdgeLength,  HalfEdgeLength, -1.0f,  0.0f,  0.0f, U[0], V[2], },
                                                                                                       
        { -HalfEdgeLength,  HalfEdgeLength, -HalfEdgeLength,  0.0f,  1.0f,  0.0f, U[1], V[2], },
        {  HalfEdgeLength,  HalfEdgeLength, -HalfEdgeLength,  0.0f,  1.0f,  0.0f, U[2], V[2], },
        {  HalfEdgeLength,  HalfEdgeLength,  HalfEdgeLength,  0.0f,  1.0f,  0.0f, U[2], V[3], },
        { -HalfEdgeLength,  HalfEdgeLength,  HalfEdgeLength,  0.0f,  1.0f,  0.0f, U[1], V[3], },
                                                                                                        
        { -HalfEdgeLength, -HalfEdgeLength,  HalfEdgeLength,  0.0f, -1.0f,  0.0f, U[1], V[0], },
        {  HalfEdgeLength, -HalfEdgeLength,  HalfEdgeLength,  0.0f, -1.0f,  0.0f, U[2], V[0], },
        {  HalfEdgeLength, -HalfEdgeLength, -HalfEdgeLength,  0.0f, -1.0f,  0.0f, U[2], V[1], },
        { -HalfEdgeLength, -HalfEdgeLength, -HalfEdgeLength,  0.0f, -1.0f,  0.0f, U[1], V[1], },
    };

    int CubeIndices[][3] =
    {
        {  0,  1,  2, },
        {  0,  2,  3, },

        {  4,  5,  6, },
        {  4,  6,  7, },

        {  8,  9, 10, },
        {  8, 10, 11, },

        { 12, 13, 14, },
        { 12, 14, 15, },

        { 16, 17, 18, },
        { 16, 18, 19, },

        { 20, 21, 22, },
        { 20, 22, 23, },
    };

    // -----------------------------------------------------------------------------
    // The rectangular mesh to render the post effect. Note that we can use the
    // passed positions as texture coordinates to sample the passed full screen
    // input textures.
    // -----------------------------------------------------------------------------
    float QuadVertices[][3] =
    {
        { 0.0f, 1.0f, 0.0f, },
        { 1.0f, 1.0f, 0.0f, },
        { 1.0f, 0.0f, 0.0f, },
        { 0.0f, 0.0f, 0.0f, },
    };

    int QuadIndices[][3] =
    {
        { 0, 1, 2, },
        { 0, 2, 3, },
    };

    // -----------------------------------------------------------------------------
    // The cube mesh with GBuffer and standard material.
    // -----------------------------------------------------------------------------
    SMeshInfo MeshInfo;

    MeshInfo.m_pVertices        = &CubeVertices[0][0];
    MeshInfo.m_NumberOfVertices = 24;
    MeshInfo.m_pIndices         = &CubeIndices[0][0];
    MeshInfo.m_NumberOfIndices  = 36;
    MeshInfo.m_pMaterial        = m_pGBufferMaterial;

    CreateMesh(MeshInfo, &m_pGBufferMesh);

    MeshInfo.m_pVertices        = &CubeVertices[0][0];
    MeshInfo.m_NumberOfVertices = 24;
    MeshInfo.m_pIndices         = &CubeIndices[0][0];
    MeshInfo.m_NumberOfIndices  = 36;
    MeshInfo.m_pMaterial        = m_pMaterial;

    CreateMesh(MeshInfo, &m_pMesh);

    // -----------------------------------------------------------------------------
    // The quad mesh with the post effect material.
    // -----------------------------------------------------------------------------
    MeshInfo.m_pVertices        = &QuadVertices[0][0];
    MeshInfo.m_NumberOfVertices = 4;
    MeshInfo.m_pIndices         = &QuadIndices[0][0];
    MeshInfo.m_NumberOfIndices  = 6;
    MeshInfo.m_pMaterial        = m_pPostMaterial;

    CreateMesh(MeshInfo, &m_pPostMesh);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnReleaseMeshes()
{
    ReleaseMesh(m_pGBufferMesh);
    ReleaseMesh(m_pMesh);
    ReleaseMesh(m_pPostMesh);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnResize(int _Width, int _Height)
{
    GetProjectionMatrix(m_FieldOfViewY, static_cast<float>(_Width) / static_cast<float>(_Height), m_Near, m_Far, m_ProjectionMatrix);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnUpdate()
{
    float Eye[3];
    float At [3];
    float Up [3];

    Eye[0] =  0.0f; At[0] = 0.0f; Up[0] = 0.0f;
    Eye[1] =  4.0f; At[1] = 0.0f; Up[1] = 1.0f;
    Eye[2] = -8.0f; At[2] = 0.0f; Up[2] = 0.0f;

    GetViewMatrix(Eye, At, Up, m_ViewMatrix);

    return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnFrame()
{
    float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f, };

    // -----------------------------------------------------------------------------
    // Upload the matrices to the vertex shader.
    // -----------------------------------------------------------------------------
    SVertexBuffer VertexBuffer;

    MulMatrix(m_ViewMatrix, m_ProjectionMatrix, VertexBuffer.m_ViewProjectionMatrix);

    GetRotationYMatrix(m_AngleY, VertexBuffer.m_WorldMatrix);

    GetScreenMatrix(VertexBuffer.m_ScreenMatrix);

    UploadConstantBuffer(&VertexBuffer, m_pVertexConstantBuffer);

    // -----------------------------------------------------------------------------
    // Upload the near and far distance to the pixel shader.
    // -----------------------------------------------------------------------------
    SPixelBuffer PixelBuffer;

    PixelBuffer.m_NearFar[0] = m_Near;
    PixelBuffer.m_NearFar[1] = m_Far;
    PixelBuffer.m_NearFar[2] = 0.0f;
    PixelBuffer.m_NearFar[3] = 0.0f;

    UploadConstantBuffer(&PixelBuffer, m_pPixelConstantBuffer);

    // -----------------------------------------------------------------------------
    // Activate the GBuffer and render depth and normals of the cube to it.
    // -----------------------------------------------------------------------------
    SetRenderTargets(&m_pNormalTarget, 1, m_pDepthTarget);

    ClearDepthTarget(m_pDepthTarget, 1.0f);
    ClearColorTarget(m_pNormalTarget, ClearColor);

    DrawMesh(m_pGBufferMesh);

    // -----------------------------------------------------------------------------
    // Draw the cube with standard materials into the color target. Note that we set
    // the depth buffer of the GBuffer as depth buffer, which is already filled with
    // the depth values of the GBuffer pass. So each pixel of the scene which is
    // more or equal distant than the pixel in the depth buffer is discarded. For
    // that reason we switch the depth test to equal, so at least the pixel with an
    // equal distance pass the test. These are exactly the same pixels which won in
    // the GBuffer pass.
    // -----------------------------------------------------------------------------
    SetRenderTargets(&m_pColorTarget, 1, m_pDepthTarget);
    
    ClearColorTarget(m_pColorTarget, ClearColor);

    SetDepthTest(SDepthTest::Equal);

    DrawMesh(m_pMesh);

    // -----------------------------------------------------------------------------
    // We now set the default frame and depth buffer, because the default frame
    // buffer is the only one, which is visible on the screen. Therefore we have to
    // render the final pass to this frame buffer to see anything on the screen.
    // Note that in this way the depth, normal, and color target are deactivated.
    // This is necessary, because we want to use these targets now as input textures
    // of the post effect. WHen rendering a post effect it is important to switch
    // off the depth test, because we want to calculate each pixel of the screen
    // under all circumstances.
    // -----------------------------------------------------------------------------
    ResetRenderTargets();

    SetDepthTest(SDepthTest::Off);

    DrawMesh(m_pPostMesh);

    // -----------------------------------------------------------------------------
    // We are done with the post effect so set the depth test to its default again.
    // -----------------------------------------------------------------------------
    SetDepthTest(SDepthTest::Lesser);

    m_AngleY = ::fmodf(m_AngleY + 0.003f, 360.0f);

    return true;
}

// -----------------------------------------------------------------------------

void main()
{
    CApplication Application;

    RunApplication(800, 600, "YoshiX Example", &Application);
}