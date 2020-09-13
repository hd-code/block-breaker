// --- Textures ----------------------------------------------------------------

sampler Sampler : register(s0);

Texture2D Tex0 : register(t0);
Texture2D Tex1 : register(t1);
Texture2D Tex2 : register(t2);
Texture2D Tex3 : register(t3);
Texture2D Tex4 : register(t4);
Texture2D Tex5 : register(t5);
Texture2D Tex6 : register(t6);
Texture2D Tex7 : register(t7);
Texture2D Tex8 : register(t8);
Texture2D Tex9 : register(t9);

// --- Constant Buffers --------------------------------------------------------

cbuffer VSBuffer : register(b0) {
    float4x4 ViewProjectionMatrix;
}

cbuffer VSBuffer : register(b1) {
    float4x4 WorldMatrix;
};

cbuffer PSBuffer : register(b0) {
    float3 CameraPos;
    float3 LightDir;
    float3 AmbientLight;
}

cbuffer PSBuffer : register(b1) {
    float TextureIndex;
    float SpecularExponent;
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
};

// --- Vertex Shader -----------------------------------------------------------

PSInput VShader(VSInput input) {
    float4 worldSpacePosition = mul(float4(input.position, 1.0f), WorldMatrix);

    PSInput result   = (PSInput) 0;
    result.position  = mul(worldSpacePosition, ViewProjectionMatrix);
    result.normal    = mul(input.normal, (float3x3)WorldMatrix);
    result.texCoords = input.texCoords;

    return result;
}

// --- Pixel Shader ------------------------------------------------------------

float4 PShader(PSInput input) : SV_Target {
    float3 lightVec = normalize(-LightDir);
    float3 viewVec  = normalize(CameraPos - input.position.xyz);
    float3 halfVec  = normalize(lightVec + viewVec);

    float3 normal = normalize(input.normal);

    float4 ambientLight  = float4(AmbientLight, 1.0f);
	float4 diffuseLight  = max(dot(normal, lightVec), 0.0f);
    float4 specularLight = pow(max(dot(normal, halfVec), 0.0f), SpecularExponent);
	
    float4 light = ambientLight + diffuseLight + specularLight;

    switch (TextureIndex) {
        case 0: return Tex0.Sample(Sampler, input.texCoords) * light;
        case 1: return Tex1.Sample(Sampler, input.texCoords) * light;
        case 2: return Tex2.Sample(Sampler, input.texCoords) * light;
        case 3: return Tex3.Sample(Sampler, input.texCoords) * light;
        case 4: return Tex4.Sample(Sampler, input.texCoords) * light;
        case 5: return Tex5.Sample(Sampler, input.texCoords) * light;
        case 6: return Tex6.Sample(Sampler, input.texCoords) * light;
        case 7: return Tex7.Sample(Sampler, input.texCoords) * light;
        case 8: return Tex8.Sample(Sampler, input.texCoords) * light;
        case 9: return Tex9.Sample(Sampler, input.texCoords) * light;
        default: return Tex0.Sample(Sampler, input.texCoords) * light;
    }
}

// Texture2D getTexture() {
//     switch (TextureIndex) {
//         case 0: return Tex0;
//         case 1: return Tex1;
//         case 2: return Tex2;
//         case 3: return Tex3;
//         case 4: return Tex4;
//         case 5: return Tex5;
//         default: return Tex0;
//     }
// }