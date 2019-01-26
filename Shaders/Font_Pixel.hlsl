// Font_Pixel.hlsl

// Globals
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer PixelBuffer
{
    float4 pixelColor;
};

// Typedef
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

// Pixel Shader
float4 FontPixelShader(PixelInputType input):SV_TARGET
{
    float4 color;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location
    color = shaderTexture.Sample(SampleType, input.tex);

    // if the color is black on the texture then treat this pixel as transparent
    if(color.r == 0.0f)
    {
        color.a = 0.0f;
    }
    // if the color is other than black on the texture then this is a pixel in the font so draw it using the font pixel color
    else
    {
        color.a = 1.0f;
        color = color * pixelColor;
    }

    return color;
}