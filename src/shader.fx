// --- Textures ----------------------------------------------------------------

sampler   Sampler : register(s0);
Texture2D Texture : register(t0);

// --- Constant Buffers --------------------------------------------------------

cbuffer VSBuffer : register(b0) {
    float4x4 WorldMatrix;
};

cbuffer VSBuffer : register(b1) {
    float4x4 ViewProjectionMatrix;
}

// --- Data Types --------------------------------------------------------------

struct VSInput {
    float3 position  : POSITION;
    float3 normal    : NORMAL;
    float2 texCoords : TEXCOORD;
};

struct PSInput {
    float4 position  : SV_POSITION;
    float3 normal    : NORMAL;
    float2 texCoords : TEXCOORD0;
    float3 lightDir  : TEXCOORD1;
};

// --- Vertex Shader -----------------------------------------------------------

PSInput VShader(VSInput input) {
    float4 worldSpacePosition = mul(float4(input.position, 1.0f), WorldMatrix);
    float3 lightPos = float3(1.0f, 10.0f, -10.0f);

    PSInput result   = (PSInput) 0;
    result.position  = mul(worldSpacePosition, ViewProjectionMatrix);
    result.normal    = mul(input.normal, (float3x3)WorldMatrix);
    result.texCoords = input.texCoords;
    result.lightDir  = normalize(lightPos - (float3)worldSpacePosition);

    return result;
}

// --- Pixel Shader ------------------------------------------------------------

float4 PShader(PSInput input) : SV_Target {
    float3 lightDir = normalize(input.lightDir);
    float3 normal   = normalize(input.normal);

    float4 AmbientLight  = float4(0.1f, 0.1f, 0.1f, 1.0f);
	float4 DiffuseLight  = max(dot(normal, lightDir), 0.0f);
    // float4 SpecularLight = pow(max(dot(WSNormal, WSHalf), 0.0f), 2.0f);
	
    float4 Light = AmbientLight + DiffuseLight; // + SpecularLight;

    return Texture.Sample(Sampler, input.texCoords) * Light;
}