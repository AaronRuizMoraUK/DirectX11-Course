struct PixelIn
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float3 viewDir : TEXCOORD2;
};

struct PixelOut
{
    float4 color : SV_Target;
};

Texture2D tex2D : register(t0);
SamplerState tex2DSamplerState : register(s0);

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
    
    float3 normal = normalize(pixelIn.normal);
    float3 halfDir = normalize(normalize(pixelIn.viewDir) - LightDir);
    float4 diffuleColor = tex2D.Sample(tex2DSamplerState, pixelIn.uv);
    
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
