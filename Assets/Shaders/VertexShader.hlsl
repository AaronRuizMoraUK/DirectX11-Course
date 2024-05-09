struct VertexIn
{
    float3 position : SV_Position;
};

struct VertexOut
{
    float4 position : SV_Position;
};

VertexOut main(VertexIn vertexIn)
{
    VertexOut vertexOut;
    vertexOut.position = float4(vertexIn.position, 1.0);

    return vertexOut;
}
