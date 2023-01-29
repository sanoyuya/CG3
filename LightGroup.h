#pragma once
#include<DirectXMath.h>
#include"DirectionalLight.h"
#include"PointLight.h"

class LightGroup
{
private://エイリアス
	//Microsoft::WRL::を省略
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public://定数
	static const int DirLightNum = 3;
	static const int PointLightNum = 3;

public://サブクラス
	//定数バッファ用構造体
	struct ConstBufferData
	{
		//環境光の色
		XMFLOAT3 ambientColor;
		float pad1;
		//平行光源用
		DirectionalLight::ConstBufferData dirLights[DirLightNum];
		//点光源用
		PointLight::ConstBufferData pointLights[PointLightNum];
	};

private://静的メンバ変数
	//デバイス
	static ID3D12Device* device;

public://静的メンバ関数
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// インスタンス生成
	/// </summary>
	/// <returns>インスタンス</returns>
	static LightGroup* Create();

private://メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource>constBuff;
	//環境光の色
	XMFLOAT3 ambientColor = { 1,1,1 };
	//平行光源の配列
	DirectionalLight dirLights[DirLightNum];
	//点光源の配列
	PointLight pointLights[PointLightNum];
	//ダーティフラグ
	bool dirty = false;

public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList,UINT rootParameterIndex);

	/// <summary>
	/// 定数バッファ転送
	/// </summary>
	void TransferConstBuffer();

	/// <summary>
	/// 環境光のライトをセット
	/// </summary>
	/// <param name="color">ライトの色</param>
	void SetAmbientColor(const XMFLOAT3& color);

	/// <summary>
	/// 標準のライト設定
	/// </summary>
	void DefaultLightSetting();

#pragma region DirLight

	/// <summary>
	/// 平行光源の有効フラグをセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="active">有効フラグ</param>
	void SetDirLightActive(int index, bool active);

	/// <summary>
	/// 平行光源のライト方向をセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightDer">ライト方向</param>
	void SetDirLightDir(int index, const XMVECTOR& lightDir);

	/// <summary>
	/// 平行光源のライトの色をセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightColor">ライトの色</param>
	void SetDirLightColor(int index, const XMFLOAT3& lightColor);

#pragma endregion
	
#pragma region PointLight

	/// <summary>
	/// 点光源の有効フラグをセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="active">有効フラグ</param>
	void SetPointLightActive(int index, bool active);

	/// <summary>
	/// 点光源の座標をセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightPos">座標</param>
	void SetPointLightPos(int index, const XMFLOAT3& lightPos);

	/// <summary>
	/// 点光源のライトの色をセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightColor">ライトの色</param>
	void SetPointLightColor(int index, const XMFLOAT3& lightColor);

	/// <summary>
	/// 点光源の距離減衰係数をセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightAtten">距離減衰係数</param>
	void SetPointLightAtten(int index, const XMFLOAT3& lightAtten);

#pragma endregion
};