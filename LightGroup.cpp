#include "LightGroup.h"
#include <d3dx12.h>

using namespace DirectX;

/// <summary>
/// 静的メンバ変数の実態
/// </summary>
ID3D12Device* LightGroup::device = nullptr;

void LightGroup::StaticInitialize(ID3D12Device* device)
{
	//再初期化チェック
	assert(!LightGroup::device);

	//nullptrチェック
	assert(device);

	LightGroup::device = device;
}

LightGroup* LightGroup::Create()
{
	//3Dオブジェクトのインスタンスを生成
	LightGroup* instance = new LightGroup();

	//初期化
	instance->Initialize();

	return instance;
}

void LightGroup::Initialize()
{
	// nullptrチェック
	assert(device);

	DefaultLightSetting();

	// ヒープ設定
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);

	HRESULT result;
	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	TransferConstBuffer();
}

void LightGroup::Update()
{
	//値の更新があった時だけ定数バッファに転送する
	if (dirty)
	{
		TransferConstBuffer();
		dirty = false;
	}
}

void LightGroup::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex)
{
	//定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuff->GetGPUVirtualAddress());
}

void LightGroup::TransferConstBuffer()
{
	HRESULT result;
	//定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {

		constMap->ambientColor = ambientColor;

		//平行光源
		for (int i = 0; i < DirLightNum; i++)
		{
			//ライトが有効の時のみ設定を転送
			if (dirLights[i].IsActive())
			{
				constMap->dirLights[i].active = true;
				constMap->dirLights[i].lightv = -dirLights[i].GetLightDir();
				constMap->dirLights[i].lightcolor = dirLights[i].GetLightColor();
			}
			//ライトが無効なら転送しない
			else
			{
				constMap->dirLights[i].active = false;
			}
		}

		//点光源
		for (int i = 0; i < PointLightNum; i++)
		{
			//ライトが有効なら設定を転送
			if (pointLights[i].IsActive())
			{
				constMap->pointLights[i].active = 1;
				constMap->pointLights[i].lightPos = pointLights[i].GetLightPos();
				constMap->pointLights[i].lightColor = pointLights[i].GetLightColor();
				constMap->pointLights[i].lightAtten = pointLights[i].GetLightAtten();
			}
			else
			{
				constMap->pointLights[i].active = 0;
			}
		}

		constBuff->Unmap(0, nullptr);
	}
}

void LightGroup::SetAmbientColor(const XMFLOAT3& color)
{
	ambientColor = color;
	dirty = true;
}

void LightGroup::SetDirLightActive(int index, bool active)
{
	assert(0 <= index && index < DirLightNum);
	dirLights[index].SetActive(active);
}

void LightGroup::SetDirLightDir(int index, const XMVECTOR& lightDer)
{
	assert(0 <= index && index < DirLightNum);
	dirLights[index].SetLightDir(lightDer);
	dirty = true;
}

void LightGroup::SetDirLightColor(int index, const XMFLOAT3& lightColor)
{
	assert(0 <= index && index < DirLightNum);
	dirLights[index].SetLightColor(lightColor);
	dirty = true;
}

void LightGroup::SetPointLightActive(int index, bool active)
{
	assert(0 <= index && index < PointLightNum);

	pointLights[index].SetActive(active);
}

void LightGroup::SetPointLightPos(int index, const XMFLOAT3& lightPos)
{
	assert(0 <= index && index < PointLightNum);

	pointLights[index].SetLightPos(lightPos);
	dirty = true;
}

void LightGroup::SetPointLightColor(int index, const XMFLOAT3& lightColor)
{
	assert(0 <= index && index < PointLightNum);

	pointLights[index].SetLightColor(lightColor);
	dirty = true;
}

void LightGroup::SetPointLightAtten(int index, const XMFLOAT3& lightAtten)
{
	assert(0 <= index && index < PointLightNum);

	pointLights[index].SetLightAtten(lightAtten);
	dirty = true;
}

void LightGroup::DefaultLightSetting()
{
	dirLights[0].SetActive(true);
	dirLights[0].SetLightColor({ 1.0f,1.0f,1.0f });
	dirLights[0].SetLightDir({ 0.0f,-1.0f,0.0f,0 });

	dirLights[1].SetActive(true);
	dirLights[1].SetLightColor({ 1.0f,1.0f,1.0f });
	dirLights[1].SetLightDir({ 0.5f,0.1f,0.2f,0 });

	dirLights[2].SetActive(true);
	dirLights[2].SetLightColor({ 1.0f,1.0f,1.0f });
	dirLights[2].SetLightDir({ -0.5f,0.1f,-0.2f,0 });
}
