
// -----------------------------------------------------------------------------
// Define the constant buffer.
// -----------------------------------------------------------------------------
cbuffer VSBuffer : register(b0)                 // Register the constant buffer on slot 0
{
    float4x4 g_ViewProjectionMatrix;
    float4x4 g_WorldMatrix;
};

// -----------------------------------------------------------------------------
// Texture variables.
// -----------------------------------------------------------------------------
Texture2D g_ColorMap : register(t0);            // Register the color map on texture slot 0

// -----------------------------------------------------------------------------
// Sampler variables.
// -----------------------------------------------------------------------------
sampler g_ColorMapSampler : register(s0);       // Register the sampler on sampler slot 0

// -----------------------------------------------------------------------------
// Define input and output data of the vertex shader.
// -----------------------------------------------------------------------------
struct VSInput
{
    float3 m_Position : POSITION;
    float2 m_TexCoord : TEXCOORD;
};

struct PSInput
{
    float4 m_Position : SV_POSITION;
    float2 m_TexCoord : TEXCOORD0;
};

// -----------------------------------------------------------------------------
// Vertex Shader
// -----------------------------------------------------------------------------
PSInput VSShader(VSInput _Input)
{
    float4 WSPosition;

    PSInput Output    = (PSInput) 0;

	// -------------------------------------------------------------------------------
	// Get the world space position.
	// -------------------------------------------------------------------------------
    WSPosition        = mul(float4(_Input.m_Position, 1.0f), g_WorldMatrix);

	// -------------------------------------------------------------------------------
	// Get the clip space position.
	// -------------------------------------------------------------------------------
    Output.m_Position = mul(WSPosition, g_ViewProjectionMatrix);
    Output.m_TexCoord = _Input.m_TexCoord;

    return Output;
}

// -----------------------------------------------------------------------------
// Pixel Shader
// -----------------------------------------------------------------------------
float4 PSShader(PSInput _Input) : SV_Target
{
    return g_ColorMap.Sample(g_ColorMapSampler, _Input.m_TexCoord); 
}


