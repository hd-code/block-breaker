// -----------------------------------------------------------------------------
// Define the constant buffers.
// -----------------------------------------------------------------------------
cbuffer VSBuffer : register(b0)         // Register the constant buffer on slot 0
{
    float4x4 WorldMatrix;
};

cbuffer VSBuffer : register(b1)
{
    float4x4 ViewProjectionMatrix;
}

// -----------------------------------------------------------------------------
// Define input and output data of the vertex shader.
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
PSInput VSShader(VSInput input)
{
    float4 WSPosition;

    PSInput result    = (PSInput) 0;

    worldSpacePosition = mul(float4(input.position, 1.0f), WorldMatrix);
    result.position = mul(worldSpacePosition, ViewProjectionMatrix);

    return result;
}

// -----------------------------------------------------------------------------
// Pixel Shader
// -----------------------------------------------------------------------------
float4 PSShader(PSInput _Input) : SV_Target
{
    return 1.0f;
}


