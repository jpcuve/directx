struct PS_INPUT
{
	float4 Position : SV_POSITION; // interpolated vertex m_position (system value)
	float4 Color    : COLOR0;      // interpolated diffuse color
};

struct PS_OUTPUT
{
	float4 RGBColor : SV_TARGET; // pixel color (your PS computes this system value)
};

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;
	output.RGBColor = input.Color;
	return output;
}