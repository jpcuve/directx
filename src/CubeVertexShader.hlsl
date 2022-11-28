cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix mWorld;     // world matrix for object
	matrix View;       // view matrix
	matrix Projection; // projection matrix
};

struct VS_INPUT
{
	float3 vPos: POSITION;
	float3 vNorm: NORMAL;
	float4 vColor: COLOR0;
};

struct VS_OUTPUT
{
	float4 Position : SV_POSITION; // Vertex shaders must output SV_POSITION
	float4 Color    : COLOR0;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;

	// Transform the m_position from object space to homogeneous projection space
	float4 positionInViewSpace = mul(mul(float4(input.vPos, 1.0f), mWorld), View);
	float4 positionInProjectedSpace = mul(positionInViewSpace, Projection);
    float4 normalInWorldSpace = normalize(mul(float4(input.vNorm, 0), mWorld));
    float factor = (normalInWorldSpace.z + 1.0f) / 2.0f;
	// Just pass through the color data
	output.Position = positionInProjectedSpace;
	output.Color = float4(input.vColor.rgb * factor, input.vColor.a);

	return output;
}