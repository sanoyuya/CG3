﻿#pragma once

#include "DebugCamera.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Object3d.h"
#include "Sprite.h"
#include <DirectXMath.h>
#include"LightGroup.h"
#include "LambertModel.h"
#include"BaseModel.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
private: // エイリアス
	// Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // 静的メンバ変数
	static const int debugTextTexNumber = 0;

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxCommon, Input* input);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	DebugText debugText;
	DebugCamera* camera = nullptr;
	LightGroup* lightGroup = nullptr;

	uint8_t scene = 0;

	//光線方向初期値
	//static XMVECTOR lightDir;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	Sprite* spriteBG = nullptr;

	Model* modelSkydome = nullptr;
	Model* modelGround = nullptr;
	Model* modelFighter = nullptr;
	Model* modelSphere = nullptr;

	BaseModel* baseSkydome = nullptr;
	LambertModel* lambertSphere = nullptr;
	LambertModel* lambertGround = nullptr;
	/*LambertModel* lambertFighter = nullptr;*/

	Object3d* objGround = nullptr;
	Object3d* objFighter = nullptr;
	Object3d* objSphere = nullptr;

	float ambientColor0[3] = { 1,1,1 };
	//光線方向初期値
	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

	//点光源
	float pointLightPos1[3] = { -3,1,-2 };
	float pointLightColor1[3] = { 1,1,1 };
	float pointLightAtten1[3] = { 0.3f,0.1f,0.1f };

	float pointLightPos2[3] = { 3,1,-2 };
	float pointLightColor2[3] = { 1,1,1 };
	float pointLightAtten2[3] = { 0.3f,0.1f,0.1f };

	//スポットライト
	float spotLightDir[3] = { 0,-1,0 };
	float spotLightPos[3] = { 0,5,0 };
	float spotLightColor[3] = { 1,1,1 };
	float spotLightAtten[3] = { 0.0f,0.0f ,0.0f };
	float spotLightFactorAngle[2] = { 20.0f,30.0f };

	//丸影
	float circleShadowDir[3] = { 0,-1,0 };
	float circleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle[2] = { 0.0f,0.5f };

	float fighterPos[3] = { 2,1.0f,0 };

private:

	void Reset();
};