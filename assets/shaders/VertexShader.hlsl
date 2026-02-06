struct VS_Out
{
    float3 colour : Colour;
    float4 pos : SV_Position;
};

cbuffer CBuf
{
    row_major matrix transform;
};

VS_Out main(float2 pos : Position, float3 colour : Colour)
{
    VS_Out vso;
    vso.pos = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transform);
    vso.colour = colour;
    return vso;
}
