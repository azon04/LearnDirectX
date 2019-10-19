// AlphaMap_Vertex.hlsl

// Globals
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

// typedefs
struct VertexInputType
{
	float4 position: POSITION;
	float2 tex: TEXCOORD0;
};

struct PixelInputType
{
	float4 position: SV_POSITION;
	float2 tex: TEXCOORD0;
};

// Vertex Shader
PixelInputType AlphaMapVertexShader(VertexInputType input)
{
	PixelInputType output;

	// Change the position vector tobe 4 uits for proper matrix calculations
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader
	output.tex = input.tex;
	
	return output;
}