#include "LambertShader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    //右、下、奥の方向を向いたライト
    float3 lightDir = float3(1, -1, 1);
    lightDir = normalize(lightDir);
    //ライトの色(白)
    float3 lightColor = float3(1, 1, 1);
    //ピクセルシェーダーに渡す値
    VSOutput output;
    output.svpos = mul(mat, pos);
    //Lambert反射の計算
    output.color.rgb = dot(-lightDir, normal) * m_diffuse * lightColor;
    output.color.a = m_alpha;
    output.uv = uv;
    
    return output;
}