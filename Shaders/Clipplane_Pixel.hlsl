// Texture_Pixel.hlsl

// Globals
Texture2D shaderTexture;
SamplerState SampleType;

// Typedef
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float clip : SV_ClipDistance0;
};

// Pixel Shader
float4 ClipplanePixelShader(PixelInputType input):SV_TARGET
{
    float4 textureColor;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location
    textureColor = shaderTexture.Sample(SampleType, input.tex);
    
    return textureColor;
}