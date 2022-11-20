#include "GameScene.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete spriteBG;
	delete billboard;
	delete object3d;
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);

	this->dxCommon = dxCommon;
	this->input = input;

	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png");
	// デバッグテキスト初期化
	debugText.Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/background.png");

	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	// 3Dオブジェクト生成
	object3d = Object3d::Create();
	object3d->Update();
	// ビルボード生成
	billboard = Billboard::Create();
	billboard->Update();
	// パーティクル生成
	emitter = std::make_unique<Emitter>();
	emitter->Initialize();

	billboardPosition.x = 20.0f;
	object3dPosition.x = -20.0f;
}

void GameScene::Update()
{
	switch (scene)
	{
	case Scene::Billboard:

		Reset();

		object3d->SetPosition(object3dPosition);

		// オブジェクト移動
		if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
		{
			// 移動後の座標を計算
			if (input->PushKey(DIK_UP)) { billboardPosition.y += 1.0f; }
			else if (input->PushKey(DIK_DOWN)) { billboardPosition.y -= 1.0f; }
			if (input->PushKey(DIK_RIGHT)) { billboardPosition.x += 1.0f; }
			else if (input->PushKey(DIK_LEFT)) { billboardPosition.x -= 1.0f; }
		}

		// 座標の変更を反映
		billboard->SetPosition(billboardPosition);

		// カメラ移動
		if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A))
		{
			if (input->PushKey(DIK_W)) { Object3d::CameraMoveEyeVector({ 0.0f,+1.0f,0.0f }); }
			else if (input->PushKey(DIK_S)) { Object3d::CameraMoveEyeVector({ 0.0f,-1.0f,0.0f }); }
			if (input->PushKey(DIK_D)) { Object3d::CameraMoveEyeVector({ +1.0f,0.0f,0.0f }); }
			else if (input->PushKey(DIK_A)) { Object3d::CameraMoveEyeVector({ -1.0f,0.0f,0.0f }); }

			if (input->PushKey(DIK_W)) { Billboard::CameraMoveEyeVector({ 0.0f,+1.0f,0.0f }); }
			else if (input->PushKey(DIK_S)) { Billboard::CameraMoveEyeVector({ 0.0f,-1.0f,0.0f }); }
			if (input->PushKey(DIK_D)) { Billboard::CameraMoveEyeVector({ +1.0f,0.0f,0.0f }); }
			else if (input->PushKey(DIK_A)) { Billboard::CameraMoveEyeVector({ -1.0f,0.0f,0.0f }); }
		}

		billboard->Update();
		object3d->Update();

		break;

	case Scene::Particle:

		Reset();

		// カメラ移動
		if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A))
		{
			if (input->PushKey(DIK_W)) { ParticleManager::CameraMoveEyeVector({ 0.0f,+1.0f,0.0f }); }
			else if (input->PushKey(DIK_S)) { ParticleManager::CameraMoveEyeVector({ 0.0f,-1.0f,0.0f }); }
			if (input->PushKey(DIK_D)) { ParticleManager::CameraMoveEyeVector({ +1.0f,0.0f,0.0f }); }
			else if (input->PushKey(DIK_A)) { ParticleManager::CameraMoveEyeVector({ -1.0f,0.0f,0.0f }); }
		}

		if (input->TriggerKey(DIK_SPACE))
		{
			if (particleFlag == true)
			{
				particleFlag = false;
			}
			else
			{
				particleFlag = true;
			}
		}

		if (particleFlag == true)
		{
			emitter->Create({-25,0,0});
			emitter->Create({ 25,0,0 });
			emitter->Create({ 0,-25,25 });
			emitter->Create({ 0,25,-25 });
		}

		emitter->Update();

		break;
	}
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	//spriteBG->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画

	switch ((Scene)scene)
	{
	case Scene::Billboard:

		// 3Dオブジェクト描画前処理
		Object3d::PreDraw(cmdList);
		// 3Dオブクジェクトの描画
		object3d->Draw();
		// 3Dオブジェクト描画後処理
		Object3d::PostDraw();

		// 3Dオブジェクト描画前処理
		Billboard::PreDraw(cmdList);
		// ビルボードの描画
		billboard->Draw();
		// 3Dオブジェクト描画後処理
		Billboard::PostDraw();

		break;

	case Scene::Particle:

		// 3Dオブジェクト描画前処理
		ParticleManager::PreDraw(cmdList);

		// パーティクルの描画
		emitter->Draw();

		// 3Dオブジェクト描画後処理
		ParticleManager::PostDraw();

		break;
	}


	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	/// 
	
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

void GameScene::Reset()
{
	if (input->TriggerKey(DIK_1))
	{
		BillboardReset();
		scene = Scene::Billboard;
	}
	else if (input->TriggerKey(DIK_2))
	{
		particleFlag = true;
		ParticleManager::SetEye({ 0,0,-50 });
		scene = Scene::Particle;
	}
}

void GameScene::BillboardReset()
{
	billboardPosition = { 20,0,0 };

	Object3d::SetEye({ 0,0,-50 });
	Billboard::SetEye({ 0,0,-50 });
}