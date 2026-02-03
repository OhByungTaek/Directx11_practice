////////////////////////////////////////////////////////////////////////////////
// Filename: texture.ps
////////////////////////////////////////////////////////////////////////////////


SamplerState SampleType;
Texture2D shaderTexture1;
Texture2D shaderTexture2;


cbuffer LightBuffer
{
    float4 diffuseColor;
    float3 lightDirection;
    
    float4 diffuseColor2;
    float3 lightDirection2;
};


struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 TexturePixelShader(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    
    float4 color;
    
    float3 lightDir;
    float3 lightDir2;
    
    float lightIntensity;
    float lightIntensity2;

    
    textureColor = shaderTexture1.Sample(SampleType, input.tex);

    lightDir = -lightDirection;
    lightIntensity = saturate(dot(input.normal, lightDir));
    
    lightDir2 = -lightDirection2;
    lightDir2 = normalize(lightDir2);


    float3 HalfV;
    float3 viewingV;
    viewingV = normalize(-input.normal.xyz);
    
    HalfV = normalize(lightDir2 + viewingV);
    float3 reflectV = reflect(lightDir2, input.normal);
    lightIntensity2 = pow(saturate(dot(viewingV, reflectV)), 16);
    
    color = saturate((diffuseColor * lightIntensity) + (diffuseColor2 * lightIntensity2));
    
    color = color * textureColor;

    return color;
}
