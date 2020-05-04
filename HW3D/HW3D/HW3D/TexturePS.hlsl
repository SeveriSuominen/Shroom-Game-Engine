//U can type "register(t0)" to bind to specific input Slot these. 
//If not, default is allways t0 binded tex
Texture2D tex;

SamplerState splr;

float4 main(float2 tc : TexCoord) : SV_Target
{
	return tex.Sample(splr, tc);
}