struct VS_Out
{
    float3 colour : Colour;
    float4 pos : SV_Position;
};

VS_Out main(float2 pos : Position, float3 colour : Colour)
{
    VS_Out vso;
    vso.pos = float4(pos.x, pos.y, 0.0f, 1.0f);
    vso.colour = colour;
    return vso;
}
