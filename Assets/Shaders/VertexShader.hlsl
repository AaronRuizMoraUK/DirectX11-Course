struct VertexIn
{
    float3 position : SV_Position;
    float2 uv : TEXCOORD0;
};

struct VertexOut
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

cbuffer ViewProjMatrixConstantBuffer : register(b0)
{
    float4x4 viewMatrix;
    float4x4 projMatrix;
};

cbuffer WorldMatrixConstantBuffer : register(b1)
{
    float4x4 worldMatrix;
};

VertexOut main(VertexIn vertexIn)
{
    VertexOut vertexOut;
    vertexOut.position = mul(worldMatrix, float4(vertexIn.position, 1.0));
    vertexOut.position = mul(viewMatrix, vertexOut.position);
    vertexOut.position = mul(projMatrix, vertexOut.position);
    vertexOut.color = float4(vertexIn.uv, (vertexIn.position.z > 0.0f) ? 0.0f : 1.0f, 1.0f);

    return vertexOut;
}
