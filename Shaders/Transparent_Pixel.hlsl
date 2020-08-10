// Texture_Pixel.hlsl

// Globals
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer TransparentBuffer
{
    float blendAmount;
};

// Typedef
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

// Pixel Shader
float4 TransparentPixelShader(PixelInputType input):SV_TARGET
{
    float4 textureColor;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location
    textureColor = shaderTexture.Sample(SampleType, input.tex);
    
    // Set the alpha value of this pixel to the blending amount to create the alpha blending effect
    textureColor.a = blendAmount;

    return textureColor;
}