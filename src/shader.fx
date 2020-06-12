// -----------------------------------------------------------------------------
// Constant Buffers
// -----------------------------------------------------------------------------
cbuffer VSBuffer : register(b0)
{
    float4x4 WorldMatrix;
};

cbuffer VSBuffer : register(b1)
{
    float4x4 ViewProjectionMatrix;
}

// -----------------------------------------------------------------------------
// Data Types of Shaders
// -----------------------------------------------------------------------------
struct VSInput
{
    float3 position : POSITION;
};

struct PSInput
{
    float4 position : SV_POSITION;
};

// -----------------------------------------------------------------------------
// Vertex Shader
// -----------------------------------------------------------------------------
PSInput VShader(VSInput input)
{
    PSInput result    = (PSInput) 0;

    float4 worldSpacePosition = mul(float4(input.position, 1.0f), WorldMatrix);
    result.position = mul(worldSpacePosition, ViewProjectionMatrix);

    return result;
}

// -----------------------------------------------------------------------------
// Pixel Shader
// -----------------------------------------------------------------------------
float4 PShader(PSInput _Input) : SV_Target
{
    return 1.0f;
}


