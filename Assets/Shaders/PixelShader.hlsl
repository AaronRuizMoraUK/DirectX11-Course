struct PixelIn
{
    float4 position : SV_Position;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
};

struct PixelOut
{
    float4 color : SV_Target;
};

Texture2D tex2D : register(t0);

SamplerState tex2DSamplerState : register(s0);

PixelOut mainColor(PixelIn pixelIn)
{
    PixelOut pixelOut;
    pixelOut.color = pixelIn.color;
    return pixelOut;
}

PixelOut main(PixelIn pixelIn)
{
    PixelOut pixelOut;
    pixelOut.color = tex2D.Sample(tex2DSamplerState, pixelIn.uv) * pixelIn.color.b;
    return pixelOut;
}
