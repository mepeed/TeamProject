#include "VertexShaderInputType.hlsli"
#include "SSBConstantBuffer.hlsli"
#include "VertexShaderOutputType.hlsli"

VertexOutput_PC VS(Vertex_PC input)
{
	float4 pos = input.Position;

	pos = mul(input.Position, WorldTransformMatrix);
	pos = mul(pos, ViewMatrix);
	pos = mul(pos, ProjectionMatrix);

	VertexOutput_PC output = (VertexOutput_PC)0;
	output.p = pos;
	output.c = input.Color;

	return output;
}
