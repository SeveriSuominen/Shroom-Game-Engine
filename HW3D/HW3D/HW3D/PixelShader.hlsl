
float4 main(float4 color : Color, float4 pos : Position) : SV_Target
{
	return float4(color.r + pos.z, color.g + pos.z, color.b + pos.z, color.a);
}