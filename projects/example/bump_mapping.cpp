
#include "yoshix.h"

#include <math.h>

using namespace gfx;

// -----------------------------------------------------------------------------
// Defines a constant buffer on the CPU. Note that this struct matches exactly
// the 'VSBuffer' in the 'textured.fx' file. For easy communication between CPU
// and GPU it is always a good idea to rebuild the GPU struct on the CPU side.
// Node that the size of a constant buffer on bytes has to be a multiple of 16.
// For example the following is not possible:
// 
//     struct SVertexBuffer
//     {
//         float m_ViewProjectionMatrix[16];
//         float m_WorldMatrix[16];
//         float m_Scalar;
//     };
//
// The problem is the final member 'm_Scalar'. The two matrices at the begin
// require 2 * 16 * 4 = 128 bytes, which is dividable by 16. Adding the four
// bytes of 'm_Scalar' results in 132 bytes, which cannot be divided by 16.
// Instead you have to use a 4D vector, even if that implies a waste of memory.
// 
//     struct SVertexBuffer
//     {
//         float m_ViewProjectionMatrix[16];
//         float m_WorldMatrix[16];
//         float m_Vector[4];           => store 'm_Scalar' in the first component of the 4D vector and waste the other three ones.
//     };
//     
// -----------------------------------------------------------------------------
struct SVertexBuffer
{
	float m_ViewProjectionMatrix[16];       // Result of view matrix * projection matrix.
	float m_WorldMatrix[16];                // The world matrix to transform a mesh from local space to world space.
    float m_WSEyePosition[3];
    float m_EyeDummy;
    float m_WSLightPosition[3];
    float m_LightDummy;
};

struct SPixelBuffer
{
	float m_AmbientLightColor[4];
	float m_DiffuseLightColor[4];
	float m_SpecularColor[4];
	float m_SpecularExponent;
    float m_SpecularDummy[3];
};

// -----------------------------------------------------------------------------

class CApplication : public IApplication
{
public:

	CApplication();
	virtual ~CApplication();

private:

	float   m_FieldOfViewY;             // Vertical view angle of the camera
	float   m_ViewMatrix[16];           // The view matrix to transform a mesh from world space into view space.
	float   m_ProjectionMatrix[16];     // The projection matrix to transform a mesh from view space into clip space.
	
	BHandle m_pVertexConstantBuffer;    // A pointer to a YoshiX constant buffer, which defines global data for a vertex shader.
	BHandle m_pPixelConstantBuffer;

	BHandle m_pColorTexture;            // A pointer to a YoshiX texture, which is part of the material covering the mesh.
	BHandle m_pNormalTexture;
	
	BHandle m_pVertexShader;            // A pointer to a YoshiX vertex shader, which processes each single vertex of the mesh.
	BHandle m_pPixelShader;             // A pointer to a YoshiX pixel shader, which computes the color of each pixel visible of the mesh on the screen.
	
	BHandle m_pMaterial;                // A pointer to a YoshiX material, spawning the surface of the mesh.
	BHandle m_pMesh;                    // A pointer to a YoshiX mesh, which represents a single triangle.

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
	: m_FieldOfViewY         (60.0f)        // Set the vertical view angle of the camera to 60 degrees.
	, m_pVertexConstantBuffer(nullptr)
	, m_pPixelConstantBuffer (nullptr)
	, m_pColorTexture        (nullptr)
	, m_pNormalTexture		 (nullptr)
	, m_pVertexShader        (nullptr)
	, m_pPixelShader         (nullptr)
	, m_pMaterial            (nullptr)
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
	// Load an image from the given path and create a YoshiX texture representing
	// the image.
	// -----------------------------------------------------------------------------
	CreateTexture("..\\data\\images\\wall_color_map.dds" , &m_pColorTexture);
	CreateTexture("..\\data\\images\\wall_normal_map.dds", &m_pNormalTexture);

	return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnReleaseTextures()
{
	// -----------------------------------------------------------------------------
	// Important to release the texture again when the application is shut down.
	// -----------------------------------------------------------------------------
	ReleaseTexture(m_pColorTexture);
	ReleaseTexture(m_pNormalTexture);

	return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateConstantBuffers()
{
	// -----------------------------------------------------------------------------
	// Create a constant buffer with global data for the vertex shader. We use this
	// buffer to upload the data defined in the 'SVertexBuffer' struct. Note that it
	// is not possible to use the data of a constant buffer in vertex and pixel
	// shader. Constant buffers are specific to a certain shader stage. If a 
	// constant buffer is a vertex or a pixel buffer is defined in the material info
	// when creating the material.
	// -----------------------------------------------------------------------------
	CreateConstantBuffer(sizeof(SVertexBuffer), &m_pVertexConstantBuffer);
	CreateConstantBuffer(sizeof(SPixelBuffer) , &m_pPixelConstantBuffer);

	return true; 
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnReleaseConstantBuffers()
{
	// -----------------------------------------------------------------------------
	// Important to release the buffer again when the application is shut down.
	// -----------------------------------------------------------------------------
	ReleaseConstantBuffer(m_pVertexConstantBuffer);
	ReleaseConstantBuffer(m_pPixelConstantBuffer);

	return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateShader()
{
	// -----------------------------------------------------------------------------
	// Load and compile the shader programs.
	// -----------------------------------------------------------------------------
	CreateVertexShader("..\\data\\shader\\bump_mapping.fx", "VSShader", &m_pVertexShader);
	CreatePixelShader ("..\\data\\shader\\bump_mapping.fx", "PSShader", &m_pPixelShader);

	return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnReleaseShader()
{
	// -----------------------------------------------------------------------------
	// Important to release the shader again when the application is shut down.
	// -----------------------------------------------------------------------------
	ReleaseVertexShader(m_pVertexShader);
	ReleasePixelShader (m_pPixelShader);

	return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateMaterials()
{
	// -----------------------------------------------------------------------------
	// Create a material spawning the mesh. Note that you can use the same material
	// for multiple meshes as long as the input layout of the vertex shader matches
	// the vertex layout of the mesh.
	// -----------------------------------------------------------------------------
	SMaterialInfo MaterialInfo;

	MaterialInfo.m_NumberOfTextures              = 2;                       // We sample one texture in the pixel shader.
	MaterialInfo.m_pTextures[0]                  = m_pColorTexture;              // The handle to the texture.
	MaterialInfo.m_pTextures[1] 				 = m_pNormalTexture;

	MaterialInfo.m_NumberOfVertexConstantBuffers = 1;                       // We need one vertex constant buffer to pass world matrix and view projection matrix to the vertex shader.
	MaterialInfo.m_pVertexConstantBuffers[0]     = m_pVertexConstantBuffer; // Pass the handle to the created vertex constant buffer.
	
	MaterialInfo.m_NumberOfPixelConstantBuffers  = 1;                       // We do not need any global data in the pixel shader.
	MaterialInfo.m_pPixelConstantBuffers[0]		 = m_pPixelConstantBuffer;

	MaterialInfo.m_pVertexShader                 = m_pVertexShader;         // The handle to the vertex shader.
	MaterialInfo.m_pPixelShader                  = m_pPixelShader;          // The handle to the pixel shader.

	MaterialInfo.m_NumberOfInputElements         = 5;                       // The vertex shader requests the position and the texture coordinates as arguments.
	MaterialInfo.m_InputElements[0].m_pName      = "POSITION";              // The semantic name of the first argument, which matches exactly the first identifier in the 'VSInput' struct.
	MaterialInfo.m_InputElements[0].m_Type       = SInputElement::Float3;   // The position is a 3D vector with floating points.
	MaterialInfo.m_InputElements[1].m_pName      = "TANGENT";
	MaterialInfo.m_InputElements[1].m_Type       = SInputElement::Float3;
	MaterialInfo.m_InputElements[2].m_pName      = "BINORMAL";
	MaterialInfo.m_InputElements[2].m_Type       = SInputElement::Float3;
	MaterialInfo.m_InputElements[3].m_pName      = "NORMAL";           
	MaterialInfo.m_InputElements[3].m_Type       = SInputElement::Float3;
	MaterialInfo.m_InputElements[4].m_pName      = "TEXCOORD";
	MaterialInfo.m_InputElements[4].m_Type       = SInputElement::Float2;   // The texture coordinates are a 2D vector with floating points.

	CreateMaterial(MaterialInfo, &m_pMaterial);

	return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnReleaseMaterials()
{
	// -----------------------------------------------------------------------------
	// Important to release the material again when the application is shut down.
	// -----------------------------------------------------------------------------
	ReleaseMaterial(m_pMaterial);

	return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateMeshes()
{
	// -----------------------------------------------------------------------------
	// Define the vertices of the mesh. Note that we define the complete data of one
	// vertex in one piece, so no separate arrays for position, normals, ... This
	// technique is called 'interleaved storage'. The vertex layout here consists of
	// two elements, namely the position (3D) and the texture coordinates (2D). This
	// matches the definition of the input elements in the material info and also
	// the members of the 'VSInput' struct in the 'textured.fx' file.
	// -----------------------------------------------------------------------------
	float QuadVertices[][14] =
	{
		{ -2.0f, -2.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f },
		{  2.0f, -2.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f },
		{  2.0f,  2.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f },
		{ -2.0f,  2.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f },
	};

	// -----------------------------------------------------------------------------
	// Define the topology of the mesh via indices. An index addresses a vertex from
	// the array above. Three indices represent one triangle. When defining the 
	// triangles of a mesh imagine that you are standing in front of the triangle 
	// and looking to the center of the triangle. If the mesh represents a closed
	// body such as a cube, your view position has to be outside of the body. Now
	// define the indices of the addressed vertices of the triangle in counter-
	// clockwise order.
	// -----------------------------------------------------------------------------
	int QuadIndices[][3] =
	{
		{ 0, 2, 3, },
		{ 0, 1, 2, },
	};

	// -----------------------------------------------------------------------------
	// Define the mesh and its material. The material defines the look of the 
	// surface covering the mesh. Note that you pass the number of indices and not
	// the number of triangles.
	// -----------------------------------------------------------------------------
	SMeshInfo MeshInfo;

	MeshInfo.m_pVertices        = &QuadVertices[0][0];      // Pointer to the first float of the first vertex.
	MeshInfo.m_NumberOfVertices = 4;                        // The number of vertices.
	MeshInfo.m_pIndices         = &QuadIndices[0][0];       // Pointer to the first index.
	MeshInfo.m_NumberOfIndices  = 6;                        // The number of indices (has to be dividable by 3).
	MeshInfo.m_pMaterial        = m_pMaterial;              // A handle to the material covering the mesh.

	CreateMesh(MeshInfo, &m_pMesh);

	return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnReleaseMeshes()
{
	// -----------------------------------------------------------------------------
	// Important to release the mesh again when the application is shut down.
	// -----------------------------------------------------------------------------
	ReleaseMesh(m_pMesh);

	return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnResize(int _Width, int _Height)
{
	// -----------------------------------------------------------------------------
	// The projection matrix defines the size of the camera frustum. The YoshiX
	// camera has the shape of a pyramid with the eye position at the top of the
	// pyramid. The horizontal view angle is defined by the vertical view angle
	// and the ratio between window width and window height. Note that we do not
	// set the projection matrix to YoshiX. Instead we store the projection matrix
	// as a member and upload it in the 'InternOnFrame' method in a constant buffer.
	// -----------------------------------------------------------------------------
	GetProjectionMatrix(m_FieldOfViewY, static_cast<float>(_Width) / static_cast<float>(_Height), 0.1f, 100.0f, m_ProjectionMatrix);

	return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnUpdate()
{
	float Eye[3];
	float At [3];
	float Up [3];

	// -----------------------------------------------------------------------------
	// Define position and orientation of the camera in the world. The result is
	// stored in the 'm_ViewMatrix' matrix and uploaded in the 'InternOnFrame'
	// method.
	// -----------------------------------------------------------------------------

	Eye[0] =  0.0f; At[0] = 0.0f; Up[0] = 0.0f;
	Eye[1] =  0.0f; At[1] = 0.0f; Up[1] = 1.0f;
	Eye[2] = -8.0f; At[2] = 0.0f; Up[2] = 0.0f;

	GetViewMatrix(Eye, At, Up, m_ViewMatrix);

	return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnFrame()
{
	// -----------------------------------------------------------------------------
	// Upload the world matrix and the view projection matrix to the GPU. This has
	// to be done before drawing the mesh, though not necessarily in this method.
	// -----------------------------------------------------------------------------
	SVertexBuffer VertexBuffer;

	GetIdentityMatrix(VertexBuffer.m_WorldMatrix);

	MulMatrix(m_ViewMatrix, m_ProjectionMatrix, VertexBuffer.m_ViewProjectionMatrix);

    VertexBuffer.m_WSEyePosition[0]   =  0.0f;
    VertexBuffer.m_WSEyePosition[1]	  =  0.0f;
    VertexBuffer.m_WSEyePosition[2]	  = -8.0f;

    VertexBuffer.m_WSLightPosition[0] =   5.0f;
    VertexBuffer.m_WSLightPosition[1] =   5.0f;
    VertexBuffer.m_WSLightPosition[2] = -20.0f;

	UploadConstantBuffer(&VertexBuffer, m_pVertexConstantBuffer);

	SPixelBuffer PixelBuffer;

	PixelBuffer.m_AmbientLightColor[0] = 0.2f;
	PixelBuffer.m_AmbientLightColor[1] = 0.2f;
	PixelBuffer.m_AmbientLightColor[2] = 0.2f;
	PixelBuffer.m_AmbientLightColor[3] = 1.0f;

	PixelBuffer.m_DiffuseLightColor[0] = 0.7f;
	PixelBuffer.m_DiffuseLightColor[1] = 0.7f;
	PixelBuffer.m_DiffuseLightColor[2] = 0.7f;
	PixelBuffer.m_DiffuseLightColor[3] = 1.0f;

	PixelBuffer.m_SpecularColor[0]     = 1.0f;
	PixelBuffer.m_SpecularColor[1]     = 1.0f;
	PixelBuffer.m_SpecularColor[2]     = 1.0f;
	PixelBuffer.m_SpecularColor[3]     = 1.0f;

	PixelBuffer.m_SpecularExponent     = 100.0f;

	UploadConstantBuffer(&PixelBuffer, m_pPixelConstantBuffer);

	// -----------------------------------------------------------------------------
	// Draw the mesh. This will activate the shader, constant buffers, and textures
	// of the material on the GPU and render the mesh to the current render targets.
	// -----------------------------------------------------------------------------
	DrawMesh(m_pMesh);

	return true;
}

// -----------------------------------------------------------------------------

void main()
{
	CApplication Application;

	RunApplication(800, 600, "YoshiX Example", &Application);
}