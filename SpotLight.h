#pragma once

#include<DirectXMath.h>

/// <summary>
/// スポットライト
/// </summary>
class SpotLight
{
private://エイリアス
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public://サブクラス

	//定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMVECTOR lightv;
		XMFLOAT3 lightPos;
		float pad1;
		XMFLOAT3 lightColor;
		float pad2;
		XMFLOAT3 lightAtten;
		float pad3;
		XMFLOAT2 lightActorAngleCos;
		bool active;
		float pad4;
	};

private://メンバ変数

	//ライト方向(単位ベクトル)
	XMVECTOR lightDir = { 1,0,0,0 };
	//ライト座標(ワールド座標系)
	XMFLOAT3 lightPos = { 0,0,0 };
	//ライトの色
	XMFLOAT3 lightColor = { 1,1,1 };
	//ライト距離減衰係数
	XMFLOAT3 lightAtten = { 1.0f,1.0f ,1.0f };
	//ライト減衰角度(開始角度、終了角度)
	XMFLOAT2 lightFactorAngleCos = { 0.5f,0.2f };
	//有効フラグ
	bool active = false;

public://メンバ関数

	inline void SetLightDir(const XMVECTOR& lightDir) { this->lightDir = DirectX::XMVector3Normalize(lightDir); }
	inline const XMVECTOR& GetLightDir() { return lightDir; }
	inline void SetLightPos(const XMFLOAT3& lightPos) { this->lightPos = lightPos; }
	inline const XMFLOAT3& GetLightPos() { return lightPos; }
	inline void SetLightColor(const XMFLOAT3& lightColor) { this->lightColor = lightColor; }
	inline const XMFLOAT3& GetLightColor() { return lightColor; }
	inline void SetLightAtten(const XMFLOAT3& lightAtten) { this->lightAtten = lightAtten; }
	inline const XMFLOAT3& GetLightAtten() { return lightAtten; }
	inline void SetLightFactorAngle(const XMFLOAT2& lightFactorAngle)
	{
		this->lightFactorAngleCos.x = cosf(DirectX::XMConvertToRadians(lightFactorAngle.x));
		this->lightFactorAngleCos.y = cosf(DirectX::XMConvertToRadians(lightFactorAngle.y));
	}
	inline const XMFLOAT2& GetLightFactorAngleCos() { return lightFactorAngleCos; }
	inline void SetActive(bool active) { this->active = active; }
	inline bool IsActive() { return active; }
};