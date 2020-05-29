
// -----------------------------------------------------------------------------
// Define the constant buffers.
// -----------------------------------------------------------------------------
cbuffer VSBuffer : register(b0)                 // Register the vertex constant buffer on slot 0
{
    float4x4 g_ViewProjectionMatrix;
    float4x4 g_WorldMatrix;
    float4x4 g_ScreenMatrix;
};

cbuffer PSBuffer : register(b0)                 // Register the pixel constant buffer on slot 0
{
    float4 g_NearFar;                           // x: Near distance of view frustum  y: Far distance of view frustum
};

// -----------------------------------------------------------------------------
// Texture variables.
// -----------------------------------------------------------------------------
Texture2D g_ColorMap     : register(t0);        // Register the color map on texture slot 0
Texture2D g_DepthTarget  : register(t0);        // Register the GBuffer depth target as input texture on slot 0
Texture2D g_ColorTarget  : register(t1);        // Register the color target as input texture on slot 1.
Texture2D g_NormalTarget : register(t2);        // Register the GBuffer normal target as input texture on slot 2

// -----------------------------------------------------------------------------
// Sampler variables.
// -----------------------------------------------------------------------------
sampler g_Sampler : register(s0);               // Register the sampler on sampler slot 0

// -----------------------------------------------------------------------------
// Define input and output data of the vertex shader.
// -----------------------------------------------------------------------------
struct VSGBufferInput
{
    float3 m_Position  : POSITION;
    float3 m_Normal    : NORMAL;
    float2 m_TexCoords : TEXCOORD;
};

struct PSGBufferInput
{
    float4 m_Position : SV_POSITION;
    float3 m_Normal   : NORMAL;
};

// -----------------------------------------------------------------------------
// Vertex Shader
// -----------------------------------------------------------------------------
PSGBufferInput VSGBufferShader(VSGBufferInput _Input)
{
    float4 WSPosition;

    PSGBufferInput Output = (PSGBufferInput) 0;

	// -------------------------------------------------------------------------------
	// Get the world space position.
	// -------------------------------------------------------------------------------
    WSPosition             = mul(float4(_Input.m_Position, 1.0f), g_WorldMatrix);

	// -------------------------------------------------------------------------------
	// Get the clip space position.
	// -------------------------------------------------------------------------------
    Output.m_Position      = mul(WSPosition, g_ViewProjectionMatrix);
    Output.m_Normal        = mul(_Input.m_Normal, (float3x3) g_WorldMatrix);

    return Output;
}

// -----------------------------------------------------------------------------
// Pixel Shader
// -----------------------------------------------------------------------------
float4 PSGBufferShader(PSGBufferInput _Input) : SV_Target
{
    // -----------------------------------------------------------------------------
    // Pack the normal from values -1..1 to 0..1 . That is because the normal target
    // is a standard RGBA buffer with 8 bits per channel. On modern GPU hardware we
    // could use a floating point buffer as target, but this would increase the
    // complexitiy of YoshiX.
    // -----------------------------------------------------------------------------
    return float4(normalize(_Input.m_Normal) * 0.5f + 0.5f, 1.0f);
}

// -----------------------------------------------------------------------------
// Define input and output data of the vertex shader.
// -----------------------------------------------------------------------------
struct VSInput
{
    float3 m_Position  : POSITION;
    float3 m_Normal    : NORMAL;
    float2 m_TexCoords : TEXCOORD;
};

struct PSInput
{
    float4 m_Position  : SV_POSITION;
    float3 m_Normal    : NORMAL;
    float2 m_TexCoords : TEXCOORD0;
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
    Output.m_Position  = mul(WSPosition, g_ViewProjectionMatrix);
    Output.m_Normal    = mul(_Input.m_Normal, (float3x3) g_WorldMatrix);
    Output.m_TexCoords = _Input.m_TexCoords;

    return Output;
}

// -----------------------------------------------------------------------------
// Pixel Shader
// -----------------------------------------------------------------------------
float4 PSShader(PSInput _Input) : SV_Target
{
    return g_ColorMap.Sample(g_Sampler, _Input.m_TexCoords);
}

// -----------------------------------------------------------------------------
// Define input and output data of the vertex shader.
// -----------------------------------------------------------------------------
struct VSPostInput
{
    float3 m_Position     : POSITION;
};

struct PSPostInput
{
    float4 m_Position     : SV_POSITION;
    float2 m_ScreenCoord0 : TEXCOORD0;
    float4 m_ScreenCoord1 : TEXCOORD1;
    float4 m_ScreenCoord2 : TEXCOORD2;
    float4 m_ScreenCoord3 : TEXCOORD3;
    float4 m_ScreenCoord4 : TEXCOORD4;
};

// -----------------------------------------------------------------------------
// Vertex Shader
// -----------------------------------------------------------------------------
PSPostInput VSPostShader(VSPostInput _Input)
{
    float4 WSPosition;
    float2 Offset;

    PSPostInput Output       = (PSPostInput) 0;
                             
    // -----------------------------------------------------------------------------
    // Get the texture coordinate offset to access the neighbor pixels in the
    // textures. Note that the input textures cover the whole screen, so we use the
    // screen resolution here to calculate the offset. Note the best way to hard
    // code the resolution here. A better way would be to pass it via the vertex
    // constant buffer. Note that texture coordinates start in the left top corner.
    // -----------------------------------------------------------------------------
    Offset                   = float2(1.0f, 1.0f) / float2(800.0f, 600.0f);
                             
    Output.m_Position        = mul(float4(_Input.m_Position, 1.0f), g_ScreenMatrix);
    Output.m_ScreenCoord0.xy = _Input.m_Position.xy;                                        // Center
    Output.m_ScreenCoord1.xy = Output.m_ScreenCoord0.xy + float2(-Offset.x, -Offset.y);     // Left Top
    Output.m_ScreenCoord1.zw = Output.m_ScreenCoord0.xy + float2( Offset.x,  Offset.y);     // Right Bottom
    Output.m_ScreenCoord2.xy = Output.m_ScreenCoord0.xy + float2( Offset.x, -Offset.y);     // Right Top
    Output.m_ScreenCoord2.zw = Output.m_ScreenCoord0.xy + float2(-Offset.x,  Offset.y);     // Left Bottom
    Output.m_ScreenCoord3.xy = Output.m_ScreenCoord0.xy + float2(-Offset.x,      0.0f);     // Left
    Output.m_ScreenCoord3.zw = Output.m_ScreenCoord0.xy + float2( Offset.x,      0.0f);     // Right
    Output.m_ScreenCoord4.xy = Output.m_ScreenCoord0.xy + float2(     0.0f, -Offset.y);     // Top
    Output.m_ScreenCoord4.zw = Output.m_ScreenCoord0.xy + float2(     0.0f,  Offset.y);     // Bottom

    return Output;
}

// -----------------------------------------------------------------------------
// Pixel Shader
// -----------------------------------------------------------------------------
float GetLinearDepth(float2 _TexCoords)
{
    float Depth;

    // -----------------------------------------------------------------------------
    // Depth buffer stores a logarithmic depth so convert it to linear depth.
    // -----------------------------------------------------------------------------
    Depth = g_DepthTarget.Sample(g_Sampler, _TexCoords).r;

    return ((g_NearFar.x * g_NearFar.y) / (g_NearFar.y - (Depth * (g_NearFar.y - g_NearFar.x)))) / g_NearFar.y;
}

float3 GetNormal(float2 _TexCoords)
{
    float3 Normal;

    Normal = g_NormalTarget.Sample(g_Sampler, _TexCoords).rgb * 2.0f - 1.0f;

    return normalize(Normal);
}

float4 PSPostShader(PSPostInput _Input) : SV_Target
{
	float4 Color;
	float4 ColorLeftTop;
	float4 ColorRightBottom;
	float4 ColorRightTop;
	float4 ColorLeftBottom;

    float  DepthCenter;
    float  DepthLeftTop;
    float  DepthRightBottom;
    float  DepthRightTop;
    float  DepthLeftBottom;
    float  DepthLeft;
    float  DepthRight;
    float  DepthTop;
    float  DepthBottom;
    
    float3 NormalCenter;
    float3 NormalLeftTop;
    float3 NormalRightBottom;
    float3 NormalRightTop;
    float3 NormalLeftBottom;
    
    float4 NormalDot;
	float  NormalEpsilon;
	
	float4 DepthDifference;
	float  DepthEpsilon;
	
	float  Weight;
	
	float2 Offset;

    // -----------------------------------------------------------------------------
    // Antialiasing shader based on an edge detection as used in the game 'Stalker'.
    // The GPU Gems 2, page 159, for more information.
    // -----------------------------------------------------------------------------
    const float2 g_Barrier = float2(0.80f, 0.10f);
    const float2 g_Weights = float2(0.25f, 0.25f);

	DepthCenter        = GetLinearDepth(_Input.m_ScreenCoord0.xy);
	DepthLeftTop       = GetLinearDepth(_Input.m_ScreenCoord1.xy);
	DepthRightBottom   = GetLinearDepth(_Input.m_ScreenCoord1.zw);
	DepthRightTop      = GetLinearDepth(_Input.m_ScreenCoord2.xy);
	DepthLeftBottom    = GetLinearDepth(_Input.m_ScreenCoord2.zw);
	DepthLeft          = GetLinearDepth(_Input.m_ScreenCoord3.xy);
	DepthRight         = GetLinearDepth(_Input.m_ScreenCoord3.zw);
	DepthTop           = GetLinearDepth(_Input.m_ScreenCoord4.xy);
	DepthBottom        = GetLinearDepth(_Input.m_ScreenCoord4.zw);
	
	NormalCenter       = GetNormal(_Input.m_ScreenCoord0.xy);
	NormalLeftTop      = GetNormal(_Input.m_ScreenCoord1.xy);
	NormalRightBottom  = GetNormal(_Input.m_ScreenCoord1.zw);
	NormalRightTop     = GetNormal(_Input.m_ScreenCoord2.xy);
	NormalLeftBottom   = GetNormal(_Input.m_ScreenCoord2.zw);
	
	NormalDot.x        = dot(NormalCenter, NormalLeftTop    );
	NormalDot.y        = dot(NormalCenter, NormalRightBottom);
	NormalDot.z        = dot(NormalCenter, NormalRightTop   );
	NormalDot.w        = dot(NormalCenter, NormalLeftBottom );
	
	NormalDot         -= g_Barrier.x;
	NormalDot          = step(0.0f, NormalDot);
	
	NormalEpsilon      = saturate(dot(NormalDot, g_Weights.x));
	
	DepthDifference.x  = DepthLeftTop  + DepthRightBottom;
	DepthDifference.y  = DepthRightTop + DepthLeftBottom;
	DepthDifference.z  = DepthLeft     + DepthRight;
	DepthDifference.w  = DepthTop      + DepthBottom;
	
	DepthDifference    = abs(2.0f * DepthCenter - DepthDifference) - g_Barrier.y;
	DepthDifference    = step(DepthDifference, 0.0f);
	
	DepthEpsilon       = saturate(dot(DepthDifference, g_Weights.y));
	
	Weight             = (1.0f - NormalEpsilon * DepthEpsilon) * 2.5f;

    Offset             = _Input.m_ScreenCoord0.xy * (1.0f - Weight);
	
	ColorLeftTop       = g_ColorTarget.Sample(g_Sampler, Offset + _Input.m_ScreenCoord1.xy * Weight);
	ColorRightBottom   = g_ColorTarget.Sample(g_Sampler, Offset + _Input.m_ScreenCoord1.zw * Weight);
	ColorRightTop      = g_ColorTarget.Sample(g_Sampler, Offset + _Input.m_ScreenCoord2.xy * Weight);
	ColorLeftBottom    = g_ColorTarget.Sample(g_Sampler, Offset + _Input.m_ScreenCoord2.zw * Weight);
	
	Color              = (ColorLeftTop + ColorRightBottom + ColorRightTop + ColorLeftBottom) / 4.0f;

//	Color = Weight; // Uncomment this line to the result of the edge detection on the screen.
	
	return Color;
}
