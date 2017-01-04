cbuffer MatrixBuffer
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
};

struct VS_IN
{
    float4 Pos : POSITION;
    float2 TexCoord : TEXCOORD;
};

struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float2 TexCoord : TEXCOORD;
};

VS_OUT main( VS_IN input )
{
    VS_OUT output;
    
    input.Pos.w = 1.0f;

    output.Pos = mul(input.Pos, world);
    output.Pos = mul(output.Pos, view);
    output.Pos = mul(output.Pos, projection);

    output.TexCoord = input.TexCoord;

	return output;
}