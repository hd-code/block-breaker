#include "yoshix.h"

#include "meshes.cpp"

// -----------------------------------------------------------------------------

class CApplication : public gfx::IApplication
{
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
	: m_FieldOfViewY(60.0f)        // Set the vertical view angle of the camera to 60 degrees.
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
	// Path is "..\\data\\images\\texture.dds"
	// -----------------------------------------------------------------------------
	return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnReleaseTextures()
{
	return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateConstantBuffers()
{
	float dummy[16];

	gfx::CreateConstantBuffer(sizeof(dummy), &this->CB_VS_WorldMatrix);
	gfx::CreateConstantBuffer(sizeof(dummy), &this->CB_VS_ViewProjectionMatrix);

	return true; 
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnReleaseConstantBuffers()
{
	gfx::ReleaseConstantBuffer(this->CB_VS_WorldMatrix);
	gfx::ReleaseConstantBuffer(this->CB_VS_ViewProjectionMatrix);

	return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateShader()
{
	gfx::CreateVertexShader("..\\src\\shader.fx", "VShader", &this->VertexShader);
	gfx::CreatePixelShader("..\\src\\shader.fx", "PShader", &this->PixelShader);

	return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnReleaseShader()
{
	gfx::ReleaseVertexShader(this->VertexShader);
	gfx::ReleasePixelShader(this->PixelShader);

	return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateMaterials()
{
	gfx::SMaterialInfo materialInfo;

	materialInfo.m_NumberOfTextures = 0;

	materialInfo.m_NumberOfVertexConstantBuffers = 2;
    materialInfo.m_pVertexConstantBuffers[0] = this->CB_VS_WorldMatrix;
    materialInfo.m_pVertexConstantBuffers[1] = this->CB_VS_ViewProjectionMatrix;

    materialInfo.m_NumberOfPixelConstantBuffers = 0;

    materialInfo.m_pVertexShader = this->VertexShader;
    materialInfo.m_pPixelShader  = this->PixelShader;

    materialInfo.m_NumberOfInputElements = 1;
    materialInfo.m_InputElements[0].m_pName = "POSITION";
    materialInfo.m_InputElements[0].m_Type  = gfx::SInputElement::Float3;

	gfx::CreateMaterial(materialInfo, &this->Material);

	return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnReleaseMaterials()
{
	gfx::ReleaseMaterial(this->Material);

	return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnCreateMeshes()
{
	gfx::CreateMesh(GetTriangleMesh(this->Material), &this->TriangleMesh);
	// CreateTriangleMesh(this->Material, &this->TriangleMesh);

	return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnReleaseMeshes()
{
	gfx::ReleaseMesh(this->TriangleMesh);

	return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnResize(int _Width, int _Height)
{
	float viewMatrix[16];

	float cameraPosition[3] = { 0.0f, 0.0f, -2.0f };
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

bool CApplication::InternOnUpdate()
{
	return true;
}

// -----------------------------------------------------------------------------

bool CApplication::InternOnFrame()
{
	float WorldMatrix[16];

	gfx::GetIdentityMatrix(WorldMatrix);

	gfx::UploadConstantBuffer(WorldMatrix, this->CB_VS_WorldMatrix);

	gfx::DrawMesh(this->TriangleMesh);

	return true;
}