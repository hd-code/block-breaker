
// -----------------------------------------------------------------------------
// Define the constant buffers.
// -----------------------------------------------------------------------------
cbuffer VSBuffer : register(b0)					// Register the constant buffer in the vertex constant buffer state on slot 0
{
    float4x4 g_ViewProjectionMatrix;
    float4x4 g_WorldMatrix;
	float3   g_WSEyePosition;                   // Position of the viewer in world space
	float3   g_WSLightPosition;                 // Position of the light in world space (no light direction, so it is a point light)
};

cbuffer PSBuffer : register(b0)					// Register the constant buffer in the pixel constant buffer state on slot 0
{
	float4 g_AmbientLightColor;
	float4 g_DiffuseLightColor;
	float4 g_SpecularLightColor;
	float  g_SpecularExponent;
};

// -----------------------------------------------------------------------------
// Texture variables.
// -----------------------------------------------------------------------------
Texture2D g_ColorMap		: register(t0);		// Register the color map on texture slot 0
Texture2D g_NormalMap		: register(t1);		// Register the normal map on texture slot 1

// -----------------------------------------------------------------------------
// Sampler variables.
// -----------------------------------------------------------------------------
sampler g_Sampler           : register(s0);     // Register the sampler on sampler slot 0

// -----------------------------------------------------------------------------
// Define input and output data of the vertex shader.
// -----------------------------------------------------------------------------
struct VSInput
{
	float3 m_OSPosition		: POSITION;         // Object Space Position
	float3 m_OSTangent		: TANGENT;          // Object Space Tangent
	float3 m_OSBinormal		: BINORMAL;         // Object Space Binormal
	float3 m_OSNormal		: NORMAL;           // Object Space Normal
	float2 m_TexCoord		: TEXCOORD;         // Texture coordinates to access color and normal map
};

struct PSInput
{
	float4 m_CSPosition		: SV_POSITION;	    // Clip Space Position
	float3 m_WSTangent		: TEXCOORD0;	    // World Space Tangent
	float3 m_WSBinormal		: TEXCOORD1;	    // World Space Binormal
	float3 m_WSNormal		: NORMAL;		    // World Space Normal
    float3 m_WSView         : TEXCOORD2;        // 
    float3 m_WSLight        : TEXCOORD3;
	float2 m_TexCoord		: TEXCOORD4;
};

// -----------------------------------------------------------------------------
// Vertex Shader
// -----------------------------------------------------------------------------
PSInput VSShader(VSInput _Input)
{
	PSInput Output;
	float4  WSPosition;

	Output = (PSInput) 0;
	
	WSPosition          = mul(float4(_Input.m_OSPosition, 1.0f), g_WorldMatrix);
	
	Output.m_CSPosition = mul(WSPosition, g_ViewProjectionMatrix);
	Output.m_WSTangent  = normalize(mul(_Input.m_OSTangent , (float3x3) g_WorldMatrix));
	Output.m_WSBinormal = normalize(mul(_Input.m_OSBinormal, (float3x3) g_WorldMatrix));
	Output.m_WSNormal   = normalize(mul(_Input.m_OSNormal  , (float3x3) g_WorldMatrix));
	Output.m_WSView     = g_WSEyePosition   - WSPosition.xyz;
	Output.m_WSLight    = g_WSLightPosition - WSPosition.xyz;
	Output.m_TexCoord   = _Input.m_TexCoord;
	
	return Output;
}

// -----------------------------------------------------------------------------
// Pixel Shader
// -----------------------------------------------------------------------------
float4 PSShader(PSInput _Input) : SV_Target
{
    float3   WSTangent;
    float3   WSBinormal;
    float3   WSNormal;
    float3   TSNormal;
    float3x3 TS2WSMatrix;
    float3   WSView;
    float3   WSLight;
    float3   WSHalf;
    float4   Light;
    float4   AmbientLight;
    float4   DiffuseLight;
    float4   SpecularLight;

    WSTangent     = normalize(_Input.m_WSTangent);
    WSBinormal    = normalize(_Input.m_WSBinormal);
    WSNormal      = normalize(_Input.m_WSNormal);
    WSView        = normalize(_Input.m_WSView);
    WSLight       = normalize(_Input.m_WSLight);
	WSHalf        = (WSView + WSLight) * 0.5f;

    TS2WSMatrix   = float3x3(WSTangent, WSBinormal, WSNormal);

    TSNormal      = g_NormalMap.Sample(g_Sampler, _Input.m_TexCoord).rgb * 2.0f - 1.0f;
    WSNormal      = mul(TSNormal, TS2WSMatrix);
    WSNormal      = normalize(WSNormal);

    AmbientLight  = g_AmbientLightColor;
	DiffuseLight  = g_DiffuseLightColor * max(dot(WSNormal, WSLight), 0.0f);
    SpecularLight = g_SpecularLightColor * pow(max(dot(WSNormal, WSHalf), 0.0f), g_SpecularExponent);
	
    Light         = AmbientLight + DiffuseLight + SpecularLight;
    


/// Uncomment these lines separately to see the influence of the different light components
//     return AmbientLight;
//     return DiffuseLight;
//     return SpecularLight;


    
    return g_ColorMap.Sample(g_Sampler, _Input.m_TexCoord) * Light;
}