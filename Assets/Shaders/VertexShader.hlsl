struct VertexIn
{
    float3 position : SV_Position;
    float4 color : COLOR;
};

struct VertexOut
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

VertexOut main(VertexIn vertexIn)
{
    VertexOut vertexOut;
    vertexOut.position = float4(vertexIn.position, 1.0);
    vertexOut.color = vertexIn.color;

    return vertexOut;
}
