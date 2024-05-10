struct PixelIn
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

struct PixelOut
{
    float4 color : SV_Target;
};

PixelOut main(PixelIn pixelIn)
{
    PixelOut pixelOut;
    pixelOut.color = pixelIn.color;
    return pixelOut;
}
