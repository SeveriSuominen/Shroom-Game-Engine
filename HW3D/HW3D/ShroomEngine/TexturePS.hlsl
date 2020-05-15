Texture2D tex;

SamplerState splr;

cbuffer LightCBuf
{
    float3 lightPos;
    
    float  attConst = 1.0f; //Light constant aca enviromental light
    float  attLin = 0.045f; //Light linear scaling by range
    float  attQuad = 0.0075f; // Light exponential scaling by range

    float  diffuseIntensity = 1.0f;
    float4 diffuseColor = { 1.0f, 1.0f, 1.0f, 1.0f };
};

static const float3 materialColor = { 1.0f, 1.0f, 1.0f };
static const float3 ambient = { 0.10f, 0.10f, 0.10f };

/*static const float attConst = 1.0f; //Light constant aca enviromental light
static const float attLin = 0.045f; //Light linear scaling by range
static const float attQuad = 0.0075f; // Light exponential scaling by range*/
//static const float3 diffuseColor = { 1.0f, 1.0f, 1.0f };

float4 main(float3 worldPos : Position, float2 tc : TexCoord, float3 n : Normal) : SV_Target
{
    
	// fragment to light vector data
    const float3 vToL = lightPos - worldPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
	// diffuse attenuation
    const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
	// diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, n));
	// final color
    return float4(saturate(diffuse + ambient) * materialColor, 1.0f);
}