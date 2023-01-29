#include "LambertShader.hlsli"

Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    //�e�N�X�`���}�b�s���O
    float4 texColor = tex.Sample(smp, input.uv);
    //�V�F�[�f�B���O�ɂ��F�ŕ`��
    return input.color * texColor;
}