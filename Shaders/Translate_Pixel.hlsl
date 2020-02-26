// Translate_Pixel.hlsl

// Globals
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer TranslationBuffer
{
    float textureTranslation;
};

// Typedef
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

// Pixel Shader
float4 TranslatePixelShader(PixelInputType input):SV_TARGET
{
    float4 textureColor;

    input.tex.y += textureTranslation;
    input.tex.x += textureTranslation;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location
    textureColor = shaderTexture.Sample(SampleType, input.tex);
    
    return textureColor;
}