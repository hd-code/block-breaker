// --- Textures ----------------------------------------------------------------

sampler Sampler : register(s0);

Texture2D Tex0 : register(t0);
Texture2D Tex1 : register(t1);
Texture2D Tex2 : register(t2);
Texture2D Tex3 : register(t3);
Texture2D Tex4 : register(t4);
Texture2D Tex5 : register(t5);

// --- Constant Buffers --------------------------------------------------------

cbuffer VSBuffer : register(b0) {
    float4x4 ViewProjectionMatrix;
}

cbuffer VSBuffer : register(b1) {
    float4x4 WorldMatrix;
    float TextureIndex;
};

cbuffer PSBuffer : register(b0) {
    float3 CameraPos;
    float3 LightDir;
    float3 AmbientLight;
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

    float4 ambientLight  = float4(AmbientLight, 0.0f);
	float4 diffuseLight  = max(dot(normal, lightVec), 0.0f);
    float4 specularLight = pow(max(dot(normal, halfVec), 0.0f), 110.0f);
	
    float4 light = ambientLight + diffuseLight + specularLight;

    Texture2D tex = Tex0;
    switch (TextureIndex) {
        case 0: tex = Tex0;
        case 1: tex = Tex1;
        case 2: tex = Tex2;
        case 3: tex = Tex3;
        case 4: tex = Tex4;
        case 5: tex = Tex5;
        default: tex = Tex0;
    }

    return tex.Sample(Sampler, input.texCoords) * light;
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