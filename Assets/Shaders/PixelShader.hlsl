struct PixelIn
{
    float4 position : SV_Position;
};

float4 main(PixelIn pixelIn) : SV_Target
{
    return float4(1.0, 0.0, 0.0, 1.0f);
}
