cbuffer CBuf
{
	matrix transform;
    matrix transformViewProjection;
};

struct VSOut
{
    float3 worldPos : Position;
	float2 tex      : TexCoord;
    float3 normal   : Normal;
    
	float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float2 tex : TexCoord, float3 normal : Normal)
{
	VSOut vso;
    vso.pos = mul(float4(pos, 1.0f), transformViewProjection);
   
    vso.worldPos = (float3)mul(float4(pos, 1.0f), transform);
    vso.normal = mul(normal, (float3x3)transform);
	vso.tex = tex;
	
    return vso;
}