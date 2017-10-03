struct PS_IN
{
    float4 Pos : SV_POSITION;
    float2 TexCoord : TEXCOORD;
    float3 Norm : NORMAL;
};

Texture2D Texture;
SamplerState TextureSampler;

float4 main(PS_IN input) : SV_TARGET
{
    //Light not implemented yet
    //return Texture.Sample(TextureSampler, input.TexCoord);
	return float4(.5f, .5f, .5f, 1.0f);
}