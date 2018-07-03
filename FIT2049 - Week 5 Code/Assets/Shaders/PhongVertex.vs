cbuffer MatrixBuffer
{
	matrix world;
	matrix view;
	matrix projection;
};

cbuffer CameraBuffer
{
	float3 camPos;
	float padding;
};

struct VertexInput
{
	float4 position : POSITION;
	float4 colour : COLOR;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float4 colour : COLOR;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 viewDirection: TEXCOORD1;
};

PixelInput main( VertexInput input )
{
	PixelInput output;
	input.position.w = 1.0f;
	
	float4 worldPosition = mul(input.position, world);

	float4 position = mul(worldPosition, view);
	position = mul(position, projection);
	output.position = position;

	output.colour = input.colour;

	output.normal = mul(input.normal, (float3x3)world);

	output.uv = input.uv;

	output.viewDirection = camPos.xyz - worldPosition.xyz;
	output.viewDirection = normalize(output.viewDirection);

	return output;
}