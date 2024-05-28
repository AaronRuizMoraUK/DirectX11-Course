struct PixelIn
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float3 tangent : TEXCOORD2;
    float3 binormal : TEXCOORD3;
    float3 viewDir : TEXCOORD4;
};

struct PixelOut
{
    float4 color : SV_Target;
};

cbuffer WorldMatrixConstantBuffer : register(b1)
{
    float4x4 worldMatrix;
    float4x4 inverseTransposeWorldMatrix;
};

Texture2D diffuseTexture : register(t0);
Texture2D normalTexture : register(t1);
SamplerState diffuseSampler : register(s0);
SamplerState normalSampler : register(s1);

static const float3 LightDir = normalize(float3(0, -1, 1));
static const float3 LightColor = float3(0.8, 1.0, 1.0);

static const float3 AmbientColor = float3(0.0f, 0.0f, 0.0f);
static const float3 BaseDiffuseAmount = float3(0.05f, 0.05f, 0.05f);
static const float3 SpecularColor = float3(0.9, 1.0, 0.9);
static const float SpecularPower = 70.0f;

static const float Gamma = 2.2; // Assume the monitor is calibrated to the sRGB color space

PixelOut main(PixelIn pixelIn)
{
    PixelOut pixelOut;
    
    float3 halfDir = normalize(normalize(pixelIn.viewDir) - LightDir);
    float4 diffuleColor = diffuseTexture.Sample(diffuseSampler, pixelIn.uv);
    float4 normalColor = normalTexture.Sample(normalSampler, pixelIn.uv);
    
    // Normal map
    float3x3 tangentToLocal = transpose(float3x3(
        normalize(pixelIn.tangent), 
        normalize(pixelIn.binormal),
        normalize(pixelIn.normal)));
    float3 normalTangentSpace = normalize(normalColor.xyz * 2.0f - 1.0f);
    float3 normal = mul(tangentToLocal, normalTangentSpace);
    normal = normalize(mul((float3x3) inverseTransposeWorldMatrix, normal));
    
    // Diffuse Color
    float3 diffuleColorLinear = pow(diffuleColor.rgb, Gamma);
    float diffuseAmount = saturate(dot(-LightDir, normal));
    float3 diffuse = diffuleColorLinear * max(diffuseAmount, BaseDiffuseAmount);
    
    // Specular Color
    float specularAmount = pow(saturate(dot(halfDir, normal)), SpecularPower);
    specularAmount = saturate(4*diffuseAmount) * saturate(specularAmount);
    float3 specular = SpecularColor * specularAmount;
    
    // Final Color and Gamma
    float3 colorLinear = LightColor * (specular + diffuse) + AmbientColor;
    float3 colorGammaCorrected = saturate(pow(colorLinear, 1.0 / Gamma));
    
    pixelOut.color = float4(colorGammaCorrected, diffuleColor.a);
    return pixelOut;
}
