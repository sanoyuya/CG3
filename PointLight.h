#pragma once

#include<DirectXMath.h>

/// <summary>
/// �_����
/// </summary>
class PointLight
{
private://�G�C���A�X
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public://�T�u�N���X

	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMFLOAT3 lightPos;
		float pad1;
		XMFLOAT3 lightColor;
		float pad2;
		XMFLOAT3 lightAtten;
		unsigned int active;
	};

private://�����o�ϐ�
	//���C�g���W(���[���h���W�n)
	XMFLOAT3 lightPos = { 0,0,0 };
	//���C�g�̐F
	XMFLOAT3 lightColor = { 1,1,1 };
	//���C�g���������W��
	XMFLOAT3 lightAtten = { 1.0f,1.0f ,1.0f };
	//�L���t���O
	bool active = false;

public://�����o�֐�

	inline void SetLightPos(const XMFLOAT3& lightPos) { this->lightPos = lightPos; }
	inline const XMFLOAT3& GetLightPos() { return lightPos; }
	inline void SetLightColor(const XMFLOAT3& lightColor) { this->lightColor = lightColor; }
	inline const XMFLOAT3& GetLightColor() { return lightColor; }
	inline void SetLightAtten(const XMFLOAT3& lightAtten) { this->lightAtten = lightAtten; }
	inline const XMFLOAT3& GetLightAtten() { return lightAtten; }
	inline void SetActive(bool active) { this->active = active; }
	inline bool IsActive() { return active; }
};