// Texture_Pixel.hlsl

// Globals
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float specularPower;
    float4 specularColor;
};

// Typedef
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
};

// Pixel Shader
float4 LightPixelShader(PixelInputType input):SV_TARGET
{
    float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;
    float3 reflection;
    float4 specular;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location
    textureColor = shaderTexture.Sample(SampleType, input.tex);

    // set the default output color to the ambient light value for all pixels
    color = ambientColor;

    // Initialize specular color
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // Invert the light direction for calculations
    lightDir = - lightDirection;

    // Calculate the amount of light on this pixel
    lightIntensity = saturate(dot(input.normal, lightDir));
    
    // Determine the final diffuse color based on the diffuse color and the amount of light intensity
    color += diffuseColor * lightIntensity;

    // Calculate the reflection vector based on the light intensity, normal vector and light direction
    reflection = normalize(2 * input.normal - lightDir);

    // determine the amount of specular light based on the reflection vector, viewing direction and specular power
    specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);

    // Multipy the texture pixel and the final diffuse color to get the final pixel color result
    color = color * textureColor;
    
    // Add the specular component to output color
    color += specular * lightIntensity;

    // Stureate the final light color
    color = saturate(color);

    return color;
}