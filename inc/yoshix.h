
#pragma once

namespace gfx
{
    typedef void* BHandle;
} // namespace gfx

namespace gfx
{
    struct SDepthTest
    {
        enum ETest
        {
            Lesser,                                             ///< The pixel is drawn if its depth value is lesser than the depth value of the pixel already in the depth buffer.
            Equal,                                              ///< The pixel is drawn if its depth value is equal to the depth value of the pixel already in the depth buffer.
            Off,                                                ///< The pixel is always drawn, i.e. the depth test is deactivated.
        };
    };

    struct SInputElement
    {
        enum EType
        {
            SInt1,                                              ///< The type of the argument is a 1D signed integer.
            SInt2,                                              ///< The type of the argument is a 2D signed integer.
            SInt3,                                              ///< The type of the argument is a 3D signed integer.
            SInt4,                                              ///< The type of the argument is a 4D signed integer.
            UInt1,                                              ///< The type of the argument is a 1D unsigned integer.
            UInt2,                                              ///< The type of the argument is a 2D unsigned integer.
            UInt3,                                              ///< The type of the argument is a 3D unsigned integer.
            UInt4,                                              ///< The type of the argument is a 4D unsigned integer.
            Float1,                                             ///< The type of the argument is a 1D floating point.
            Float2,                                             ///< The type of the argument is a 2D floating point.
            Float3,                                             ///< The type of the argument is a 3D floating point.
            Float4,                                             ///< The type of the argument is a 4D floating point.
        };

        const char* m_pName;                                    ///< The semantic name of the vertex shader input argument such as POSITION, NORMAL, or TEXCOORD. Can be chosen freely as long as it matches the semantics of the vertex shader input.
        EType       m_Type;                                     ///< The type of the argument, which can be an 1D, 2D, 3D, or 4D signed integral, unsigned integral, or floating point value.
    };

    struct SMaterialInfo
    {
        int           m_NumberOfTextures;                       ///< The number of textures used by the material.
        BHandle       m_pTextures[16];                          ///< An array of texture handles matching the passed texture number.
        int           m_NumberOfVertexConstantBuffers;          ///< The number of vertex constant buffers used by the material.
        BHandle       m_pVertexConstantBuffers[16];             ///< An array of vertex constant buffer handles matching the passed vertex constant buffer number.
        int           m_NumberOfPixelConstantBuffers;           ///< The number of pixel constant buffers used by the material.
        BHandle       m_pPixelConstantBuffers[16];              ///< An array of pixel constant buffer handles matching the passed pixel constant buffer number.
        BHandle       m_pVertexShader;                          ///< A handle to a vertex shader.
        BHandle       m_pPixelShader;                           ///< A handle to a pixel shader.
        int           m_NumberOfInputElements;                  ///< The number of input arguments of the vertex shader (the number of members of the input struct). The maximum number is 16.
        SInputElement m_InputElements[16];                      ///< Defines semantics and format of each vertex shader input argument.
    };

    struct SMeshInfo
    {
        float*        m_pVertices;                              ///< An array with vertices, whereas each vertex is represented by 3 float values.
        int           m_NumberOfVertices;                       ///< The number of vertices in the array. If defined the number of normals, colors, and texture coordinates has to match this number.
        int*          m_pIndices;                               ///< An array with indices addressing the vertices. Each sequent triple of indices represents one triangle.
        int           m_NumberOfIndices;                        ///< The number of indices in the index array.
        BHandle       m_pMaterial;                              ///< A handle to a former created texture if the mesh should be textured. Texture coordinates have to be defined in this case.
    };
} // namespace gfx

namespace gfx
{
    class IApplication
    {
        public:

            virtual ~IApplication();

        public:

            bool OnStartup();
            bool OnShutdown();
            bool OnCreateTextures();
            bool OnReleaseTextures();
            bool OnCreateConstantBuffers();
            bool OnReleaseConstantBuffers();
            bool OnCreateShader();
            bool OnReleaseShader();
            bool OnCreateMaterials();
            bool OnReleaseMaterials();
            bool OnCreateMeshes();
            bool OnReleaseMeshes();
            bool OnResize(int _Width, int _Height);
            bool OnKeyEvent(unsigned int _Key, bool _IsKeyDown, bool _IsAltDown);
            bool OnMouseEvent(int _X, int _Y, int _Button, bool _IsButtonDown, bool _IsDoubleClick, int _WheelDelta);
            bool OnUpdate();
            bool OnFrame();

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
            virtual bool InternOnKeyEvent(unsigned int _Key, bool _IsKeyDown, bool _IsAltDown);
            virtual bool InternOnMouseEvent(int _X, int _Y, int _Button, bool _IsButtonDown, bool _IsDoubleClick, int _WheelDelta);
            virtual bool InternOnUpdate();
            virtual bool InternOnFrame();
    };
} // namespace gfx

namespace gfx
{
    void RunApplication(int _Width, int _Height, const char* _pTitle, IApplication* _pApplication);
    void StopApplication();
} // namespace gfx

namespace gfx
{
    void SetClearColor(const float* _pColor);
    void SetDepthTest(SDepthTest::ETest _Test);
    void SetWireFrame(bool _Flag);
    void SetAlphaBlending(bool _Flag);
} // namespace gfx

namespace gfx
{
    void CreateTexture(const char* _pPath, BHandle* _ppTexture);
    void CreateColorTarget(BHandle* _ppTexture);
    void CreateDepthTarget(BHandle* _ppTexture);
    void ReleaseTexture(BHandle _pTexture);
} // namespace gfx

namespace gfx
{
    void CreateConstantBuffer(int _NumberOfBytes, BHandle* _ppConstantBuffer);
    void ReleaseConstantBuffer(BHandle _pConstantBuffer);
    
    void UploadConstantBuffer(void* _pData, BHandle _pConstantBuffer);
} // namespace gfx

namespace gfx
{
    void CreateVertexShader(const char* _pPath, const char* _pShaderName, BHandle* _ppShader);
    void ReleaseVertexShader(BHandle _pShader);

    void CreatePixelShader(const char* _pPath, const char* _pShaderName, BHandle* _ppShader);
    void ReleasePixelShader(BHandle _pShader);
} // namespace gfx

namespace gfx
{
    void CreateMaterial(const SMaterialInfo& _rMaterialInfo, BHandle* _ppMaterial);
    void ReleaseMaterial(BHandle _pMaterial);
} // namespace gfx

namespace gfx
{
    void CreateMesh(const SMeshInfo& _rMeshInfo, BHandle* _ppMesh);
    void ReleaseMesh(BHandle _pMesh);
} // namespace gfx

namespace gfx
{
    void ResetRenderTargets();
    void SetRenderTargets(BHandle* _ppColorTargets, int _NumberOfColorTargets, BHandle _pDepthTarget);

    void ClearColorTarget(BHandle _pTexture, const float* _pColor);
    void ClearDepthTarget(BHandle _pTexture, float _Depth);

    void DrawMesh(BHandle _pMesh);
} // namespace gfx

namespace gfx
{
    float GetDotProduct2D(const float* _pVector1, const float* _pVector2);
    float GetDotProduct3D(const float* _pVector1, const float* _pVector2);
    float GetDotProduct4D(const float* _pVector1, const float* _pVector2);

    float* GetCrossProduct(const float* _pVector1, const float* _pVector2, float* _pResultVector);

    float* GetNormalizedVector(const float* _pVector, float* _pResultVector);
    float* TransformVector(const float* _pVector, const float* _pMatrix, float* _pResultVector);

    float* MulMatrix(const float* _pLeftMatrix, const float* _pRightMatrix, float* _pResultMatrix);

    float* GetIdentityMatrix(float* _pResultMatrix);
    float* GetTranslationMatrix(float _X, float _Y, float _Z, float* _pResultMatrix);
    float* GetScaleMatrix(float _Scalar, float* _pResultMatrix);
    float* GetScaleMatrix(float _ScalarX, float _ScalarY, float _ScalarZ, float* _pResultMatrix);
    float* GetRotationXMatrix(float _Degrees, float* _pResultMatrix);
    float* GetRotationYMatrix(float _Degrees, float* _pResultMatrix);
    float* GetRotationZMatrix(float _Degrees, float* _pResultMatrix);

    float* GetViewMatrix(float* _pEye, float* _pAt, float* _pUp, float* _pResultMatrix);
    float* GetProjectionMatrix(float _FieldOfViewY, float _AspectRatio, float _Near, float _Far, float* _pResultMatrix);
    float* GetScreenMatrix(float* _pResultMatrix);
} // namespace gfx

