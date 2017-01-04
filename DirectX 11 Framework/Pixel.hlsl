struct PS_IN
{
    float4 Pos : SV_POSITION;
    float2 TexCoord : TEXCOORD;
};

Texture2D Texture;
SamplerState TextureSampler;

float4 main(PS_IN input) : SV_TARGET
{
    return Texture.Sample(TextureSampler, input.TexCoord);
}