// LightMap_Pixel.hlsl

// Globals
Texture2D shaderTextures[2];
SamplerState SampleType;

// Typedefs
struct PixelInputType
{
    float4 position: SV_POSITION;
    float2 tex: TEXCOORD0;
};

// Pixel Shader
float4 LightMapPixelShader(PixelInputType input): SV_TARGET
{
    float4 color;
    float4 lightColor;
    float4 finalColor;

    // Get the pixel color from the color texture
    color = shaderTextures[0].Sample(SampleType, input.tex);

    // Get the pixel color from the light map
    lightColor = shaderTextures[1].Sample(SampleType, input.tex);

    // Blend the two pixels together and multiply by the gamma value
    finalColor = color * lightColor;

    return finalColor;
}