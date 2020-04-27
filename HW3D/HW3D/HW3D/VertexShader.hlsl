
//STRUCT MUST BE ORDERED RIGHT 
struct VSOUT
{ 
	float4 color  : Color;
	float4 vexpos : Position;
	float4 pos    : SV_Position;  //<- position of vertex
};

//Constant buffer
cbuffer CBuf
{
	// row_major == read matrix row by row instead of 4 and 4, slighty slower but easier
	row_major matrix transform;
};

VSOUT main(float3 pos : Position, float4 color : Color)
{
	VSOUT vso;
	vso.vexpos = float4(pos.x, pos.y, pos.z, 1.0f);
	
	//Multiply vertex with Matrix4x4
	vso.pos = mul
	(		
		float4(pos.x, pos.y, pos.z, 1.0f),
		transform
	);

	vso.color  = color;

	return vso;
}