#include "LambertShader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    //�E�A���A���̕��������������C�g
    float3 lightDir = float3(1, -1, 1);
    lightDir = normalize(lightDir);
    //���C�g�̐F(��)
    float3 lightColor = float3(1, 1, 1);
    //�s�N�Z���V�F�[�_�[�ɓn���l
    VSOutput output;
    output.svpos = mul(mat, pos);
    //Lambert���˂̌v�Z
    output.color.rgb = dot(-lightDir, normal) * m_diffuse * lightColor;
    output.color.a = m_alpha;
    output.uv = uv;
    
    return output;
}