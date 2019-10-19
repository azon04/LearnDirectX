// AlphaMap_Pixel.hlsl

// Globals
Texture2D shaderTextures[3];
SamplerState SampleType;

// Typedefs
struct PixelInputType
{
    float4 position: SV_POSITION;
    float2 tex: TEXCOORD0;
};

// Pixel Shader
float4 AlphaMapPixelShader(PixelInputType input): SV_TARGET
{
    float4 color1;
    float4 color2;
    float4 alphaValue;
    float4 finalColor;

    // Get the pixel color from the first texture
    color1 = shaderTextures[0].Sample(SampleType, input.tex);

    // Get the pixel color from the second texture
    color2 = shaderTextures[1].Sample(SampleType, input.tex);

    // Get the alpha value from the alpha map texture
    alphaValue = shaderTextures[2].Sample(SampleType, input.tex);

    // Blend the two pixels based on the alpha value
    finalColor = (alphaValue * color1) + ((1.0-alphaValue) * color2);

    // Saturate the final color
    finalColor = saturate(finalColor);

    return finalColor;
}