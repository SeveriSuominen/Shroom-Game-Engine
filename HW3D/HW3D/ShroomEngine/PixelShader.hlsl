
cbuffer CBuf
{
	float4 face_colors[6];
}

float4 main(uint tid : SV_PrimitiveID/*Primitive ID, triangle for example*/) : SV_Target
{
	return face_colors[tid % 6];
}