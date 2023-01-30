#include "GameScene.h"
#include "Model.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include<sstream>
#include<imgui.h>

using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete spriteBG;
	delete baseSkydome;
	delete objGround;
	delete objFighter;
	delete modelSkydome;
	delete modelGround;
	delete modelFighter;
	delete camera;
	delete lightGroup;
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

	// カメラ生成
	camera = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight, input);

	// カメラ注視点をセット
	camera->SetTarget({ 0, 1, 0 });
	camera->SetDistance(5.0f);

	// 3Dオブジェクトにカメラをセット
	BaseModel::SetCamera(camera);
	Object3d::SetCamera(camera);
	LambertModel::SetCamera(camera);

	//ライト生成
	lightGroup = LightGroup::Create();
	//3Dオブジェクトにライトをセット
	Object3d::SetLight(lightGroup);

	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	// 3Dオブジェクト生成
	baseSkydome = BaseModel::Create();
	lambertSphere = LambertModel::Create();
	lambertGround = LambertModel::Create();
	objGround = Object3d::Create();
	objFighter = Object3d::Create();
	objSphere = Object3d::Create();

	// テクスチャ2番に読み込み
	Sprite::LoadTexture(2, L"Resources/texture.png");

	modelSkydome = Model::CreateFromOBJ("skydome");
	modelGround = Model::CreateFromOBJ("ground");
	modelFighter = Model::CreateFromOBJ("sphere",true);
	modelSphere = Model::CreateFromOBJ("sphere");

	baseSkydome->SetModel(modelSkydome);
	lambertSphere->SetModel(modelSphere);
	lambertGround->SetModel(modelGround);
	//lambertFighter->SetModel(modelFighter);

	objGround->SetModel(modelGround);
	objFighter->SetModel(modelFighter);

	lambertSphere->SetPosition({ -2,1,0 });

	objFighter->SetPosition(XMFLOAT3(fighterPos));
	objFighter->SetPosition({ 2,1,0 });
}

void GameScene::Update()
{
	if (input->TriggerKey(DIK_SPACE))
	{
		Reset();

		if (scene == 0)
		{
			scene = 1;
		}
		else if (scene == 1)
		{
			scene = 2;
		}
		else if (scene == 2)
		{
			scene = 3;
		}
		else if (scene == 3)
		{
			scene = 0;
		}
	}
	switch (scene)
	{
	case 0://Lambert

		camera->Update();
		baseSkydome->Update();
		lambertGround->Update();
		lambertSphere->Update();
		objFighter->Update();

		//オブジェクトの回転
		{
			XMFLOAT3 rot = lambertSphere->GetRotation();
			rot.y += 1.0f;
			lambertSphere->SetRotation(rot);
			objFighter->SetRotation(rot);
		}
		objFighter->SetPosition(XMFLOAT3(fighterPos));

		break;

	case 1://点光源

		camera->Update();
		baseSkydome->Update();

		lightGroup->Update();

		objGround->Update();
		objFighter->Update();
		lambertSphere->Update();

		//オブジェクトの回転
		{
			XMFLOAT3 rot = objFighter->GetRotation();
			rot.y += 1.0f;
			lambertSphere->SetRotation(rot);
			objFighter->SetRotation(rot);
		}

		{//imguiからのパラメータを反映

			//ポイントライト
			lightGroup->SetPointLightPos(0, XMFLOAT3(pointLightPos1));
			lightGroup->SetPointLightColor(0, XMFLOAT3(pointLightColor1));
			lightGroup->SetPointLightAtten(0, XMFLOAT3(pointLightAtten1));

			lightGroup->SetPointLightPos(1, XMFLOAT3(pointLightPos2));
			lightGroup->SetPointLightColor(1, XMFLOAT3(pointLightColor2));
			lightGroup->SetPointLightAtten(1, XMFLOAT3(pointLightAtten2));
		}
		objFighter->SetPosition(XMFLOAT3(fighterPos));

		break;

	case 2:

		camera->Update();
		baseSkydome->Update();

		lightGroup->Update();

		objGround->Update();
		objFighter->Update();
		lambertSphere->Update();

		//オブジェクトの回転
		{
			XMFLOAT3 rot = objFighter->GetRotation();
			rot.y += 1.0f;
			lambertSphere->SetRotation(rot);
			objFighter->SetRotation(rot);
		}

		{//imguiからのパラメータを反映

			//スポットライト
			lightGroup->SetSpotLightDir(0, XMVECTOR({ spotLightDir[0],spotLightDir[1] ,spotLightDir[2] }));
			lightGroup->SetSpotLightPos(0, XMFLOAT3(spotLightPos));
			lightGroup->SetSpotLightColor(0, XMFLOAT3(spotLightColor));
			lightGroup->SetSpotLightAtten(0, XMFLOAT3(spotLightAtten));
			lightGroup->SetSpotLightFactorAngle(0, XMFLOAT2(spotLightFactorAngle));
		}
		objFighter->SetPosition(XMFLOAT3(fighterPos));

		break;

	case 3:

		camera->Update();
		baseSkydome->Update();

		lightGroup->Update();

		objGround->Update();
		objFighter->Update();
		lambertSphere->Update();

		//オブジェクトの回転
		{
			XMFLOAT3 rot = objFighter->GetRotation();
			rot.y += 1.0f;
			lambertSphere->SetRotation(rot);
			objFighter->SetRotation(rot);
		}

		{//imguiからのパラメータを反映

			//丸影
			lightGroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0],circleShadowDir[1] ,circleShadowDir[2],0 }));
			lightGroup->SetCircleShadowCasterPos(0, XMFLOAT3({ fighterPos[0],fighterPos[1] ,fighterPos[2] }));
			lightGroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
			lightGroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));
		}
		objFighter->SetPosition(XMFLOAT3(fighterPos));

		break;
	}
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

	switch (scene)
	{
	case 0:

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

#pragma region Baseオブジェクト描画
		// BaseModel描画前処理
		BaseModel::PreDraw(cmdList);

		// BaseModelの描画
		baseSkydome->Draw();
		/// <summary>
		/// ここにBaseModelの描画処理を追加できる
		/// </summary>

		// BaseModel描画後処理
		BaseModel::PostDraw();
#pragma endregion

#pragma region 3Dオブジェクト描画
		// 3Dオブジェクト描画前処理
		Object3d::PreDraw(cmdList);

		// 3Dオブクジェクトの描画
		objFighter->Draw();
		/// <summary>
		/// ここに3Dオブジェクトの描画処理を追加できる
		/// </summary>

		// 3Dオブジェクト描画後処理
		Object3d::PostDraw();
#pragma endregion

#pragma region Lambertモデル描画
		// LambertModel描画前処理
		LambertModel::PreDraw(cmdList);

		// LambertModelの描画
		lambertGround->Draw();
		lambertSphere->Draw();

		/// <summary>
		/// ここにLambertModelの描画処理を追加できる
		/// </summary>

		// LambertModel描画後処理
		LambertModel::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
		// 前景スプライト描画前処理
		Sprite::PreDraw(cmdList);

		/// <summary>
		/// ここに前景スプライトの描画処理を追加できる
		/// </summary>

		//// 描画
		//sprite1->Draw();
		//sprite2->Draw();

		// デバッグテキストの描画
		debugText.DrawAll(cmdList);

		// スプライト描画後処理
		Sprite::PostDraw();
#pragma endregion

		break;

	case 1:

		ImGui::Begin("Light");
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::SetWindowSize(ImVec2(500, 200));
		/*ImGui::ColorEdit3("ambientColor", ambientColor0, ImGuiColorEditFlags_Float);
		ImGui::InputFloat3("lightDir0", lightDir0);
		ImGui::ColorEdit3("lightColor0", lightColor0, ImGuiColorEditFlags_Float);

		ImGui::InputFloat3("lightDir1", lightDir1);
		ImGui::ColorEdit3("lightColor1", lightColor1, ImGuiColorEditFlags_Float);

		ImGui::InputFloat3("lightDir2", lightDir2);
		ImGui::ColorEdit3("lightColor2", lightColor2, ImGuiColorEditFlags_Float);*/

		//点光源
		ImGui::ColorEdit3("pointLightColor1", pointLightColor1, ImGuiColorEditFlags_Float);
		ImGui::InputFloat3("pointLightPos1", pointLightPos1);
		ImGui::InputFloat3("pointLightAtten1", pointLightAtten1);

		ImGui::ColorEdit3("pointLightColor2", pointLightColor2, ImGuiColorEditFlags_Float);
		ImGui::InputFloat3("pointLightPos2", pointLightPos2);
		ImGui::InputFloat3("pointLightAtten2", pointLightAtten2);

		ImGui::End();

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

#pragma region Baseオブジェクト描画
		// BaseModel描画前処理
		BaseModel::PreDraw(cmdList);

		// BaseModelの描画
		baseSkydome->Draw();
		/// <summary>
		/// ここにBaseModelの描画処理を追加できる
		/// </summary>

		// BaseModel描画後処理
		BaseModel::PostDraw();
#pragma endregion

#pragma region 3Dオブジェクト描画
		// 3Dオブジェクト描画前処理
		Object3d::PreDraw(cmdList);

		// 3Dオブクジェクトの描画
		objGround->Draw();
		objFighter->Draw();
		
		/// <summary>
		/// ここに3Dオブジェクトの描画処理を追加できる
		/// </summary>

		// 3Dオブジェクト描画後処理
		Object3d::PostDraw();
#pragma endregion

#pragma region Lambertモデル描画
		// LambertModel描画前処理
		LambertModel::PreDraw(cmdList);

		// LambertModelの描画
		lambertSphere->Draw();

		/// <summary>
		/// ここにLambertModelの描画処理を追加できる
		/// </summary>

		// LambertModel描画後処理
		LambertModel::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
		// 前景スプライト描画前処理
		Sprite::PreDraw(cmdList);

		/// <summary>
		/// ここに前景スプライトの描画処理を追加できる
		/// </summary>

		//// 描画
		//sprite1->Draw();
		//sprite2->Draw();

		// デバッグテキストの描画
		debugText.DrawAll(cmdList);

		// スプライト描画後処理
		Sprite::PostDraw();
#pragma endregion

		break;

	case 2:

		ImGui::Begin("Light");
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::SetWindowSize(ImVec2(500, 200));

		//スポットライト
		ImGui::InputFloat3("spotLightDir", spotLightDir);
		ImGui::ColorEdit3("spotLightColor", spotLightColor, ImGuiColorEditFlags_Float);
		ImGui::InputFloat3("spotLightPos", spotLightPos);
		ImGui::InputFloat3("spotLightAtten", spotLightAtten);
		ImGui::InputFloat2("spotLightFactorAngle", spotLightFactorAngle);

		ImGui::End();

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

#pragma region Baseオブジェクト描画
		// BaseModel描画前処理
		BaseModel::PreDraw(cmdList);

		// BaseModelの描画
		baseSkydome->Draw();
		/// <summary>
		/// ここにBaseModelの描画処理を追加できる
		/// </summary>

		// BaseModel描画後処理
		BaseModel::PostDraw();
#pragma endregion

#pragma region 3Dオブジェクト描画
		// 3Dオブジェクト描画前処理
		Object3d::PreDraw(cmdList);

		// 3Dオブクジェクトの描画
		objGround->Draw();
		objFighter->Draw();

		/// <summary>
		/// ここに3Dオブジェクトの描画処理を追加できる
		/// </summary>

		// 3Dオブジェクト描画後処理
		Object3d::PostDraw();
#pragma endregion

#pragma region Lambertモデル描画
		// LambertModel描画前処理
		LambertModel::PreDraw(cmdList);

		// LambertModelの描画
		lambertSphere->Draw();

		/// <summary>
		/// ここにLambertModelの描画処理を追加できる
		/// </summary>

		// LambertModel描画後処理
		LambertModel::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
		// 前景スプライト描画前処理
		Sprite::PreDraw(cmdList);

		/// <summary>
		/// ここに前景スプライトの描画処理を追加できる
		/// </summary>

		//// 描画
		//sprite1->Draw();
		//sprite2->Draw();

		// デバッグテキストの描画
		debugText.DrawAll(cmdList);

		// スプライト描画後処理
		Sprite::PostDraw();
#pragma endregion

		break;

	case 3:

		ImGui::Begin("Light");
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::SetWindowSize(ImVec2(500, 200));

		//丸影
		ImGui::InputFloat3("circleShadowDir", circleShadowDir);
		ImGui::InputFloat("circleShadowAtten", circleShadowAtten, 8);
		ImGui::InputFloat2("circleShadowFactorAngle", circleShadowFactorAngle);
		ImGui::InputFloat3("fighterPos", fighterPos);

		ImGui::End();

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

#pragma region Baseオブジェクト描画
		// BaseModel描画前処理
		BaseModel::PreDraw(cmdList);

		// BaseModelの描画
		baseSkydome->Draw();
		/// <summary>
		/// ここにBaseModelの描画処理を追加できる
		/// </summary>

		// BaseModel描画後処理
		BaseModel::PostDraw();
#pragma endregion

#pragma region 3Dオブジェクト描画
		// 3Dオブジェクト描画前処理
		Object3d::PreDraw(cmdList);

		// 3Dオブクジェクトの描画
		objGround->Draw();
		objFighter->Draw();

		/// <summary>
		/// ここに3Dオブジェクトの描画処理を追加できる
		/// </summary>

		// 3Dオブジェクト描画後処理
		Object3d::PostDraw();
#pragma endregion

#pragma region Lambertモデル描画
		// LambertModel描画前処理
		LambertModel::PreDraw(cmdList);

		// LambertModelの描画
		lambertSphere->Draw();

		/// <summary>
		/// ここにLambertModelの描画処理を追加できる
		/// </summary>

		// LambertModel描画後処理
		LambertModel::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
		// 前景スプライト描画前処理
		Sprite::PreDraw(cmdList);

		/// <summary>
		/// ここに前景スプライトの描画処理を追加できる
		/// </summary>

		//// 描画
		//sprite1->Draw();
		//sprite2->Draw();

		// デバッグテキストの描画
		debugText.DrawAll(cmdList);

		// スプライト描画後処理
		Sprite::PostDraw();
#pragma endregion

		break;
	}
}

void GameScene::Reset()
{
	camera->SetEye({ 0,0,0 });
	camera->SetTarget({ 0, 1, 0 });
	camera->SetDistance(5.0f);

	ambientColor0[0] = 1;
	ambientColor0[1] = 1;
	ambientColor0[2] = 1;
	switch (scene)
	{
	case 0:

		lightGroup->SetPointLightActive(0, true);
		lightGroup->SetPointLightActive(1, true);

		//点光源
		//左
		pointLightPos1[0] = -3;
		pointLightPos1[1] = 1;
		pointLightPos1[2] = -2;
		pointLightColor1[0] = 1;
		pointLightColor1[1] = 1;
		pointLightColor1[2] = 1;
		pointLightAtten1[0] = 0.3f;
		pointLightAtten1[1] = 0.1f;
		pointLightAtten1[2] = 0.1f;

		//右
		pointLightPos2[0] = 3;
		pointLightPos2[1] = 1;
		pointLightPos2[2] = -2;
		pointLightColor2[0] = 1;
		pointLightColor2[1] = 1;
		pointLightColor2[2] = 1;
		pointLightAtten2[0] = 0.3f;
		pointLightAtten2[1] = 0.1f;
		pointLightAtten2[2] = 0.1f;

		break;

	case 1:

		lightGroup->SetPointLightActive(0, false);
		lightGroup->SetPointLightActive(1, false);

		lightGroup->SetSpotLightActive(0, true);
		/*lightGroup->SetPointLightActive(1, false);
		lightGroup->SetPointLightActive(2, false);*/

		//スポットライト
		spotLightDir[0] = 0;
		spotLightDir[1] = -1;
		spotLightDir[2] = 0;
		spotLightPos[0] = 0;
		spotLightPos[1] = 5;
		spotLightPos[2] = 0;
		spotLightColor[0] = 1;
		spotLightColor[1] = 1;
		spotLightColor[2] = 1;
		spotLightAtten[0] = 0.0f;
		spotLightAtten[1] = 0.0f;
		spotLightAtten[2] = 0.0f;
		spotLightFactorAngle[0] = 20.0f;
		spotLightFactorAngle[1] = 30.0f;

		break;

	case 2:

		fighterPos[0] = 2.0f;
		fighterPos[1] = 2.0f;
		fighterPos[2] = 0.0f;
		lambertSphere->SetPosition({ -2.0f,2.0f,0.0f });

		lightGroup->SetCircleShadowActive(0, true);

		//スポットライト
		spotLightPos[0] = 2;
		spotLightPos[1] = 5;
		spotLightPos[2] = 0;

		//丸影
		circleShadowDir[0] = 0;
		circleShadowDir[1] = -1;
		circleShadowDir[2] = 0;
		circleShadowAtten[0] = 0.5f;
		circleShadowAtten[1] = 0.6f;
		circleShadowAtten[2] = 0.0f;
		circleShadowFactorAngle[0] = 0.0f;
		circleShadowFactorAngle[1] = 0.5f;

		break;

	case 3:

		fighterPos[0] = 2.0f;
		fighterPos[1] = 1.0f;
		fighterPos[2] = 0.0f;
		lambertSphere->SetPosition({ -2.0f,1.0f,0.0f });

		lightGroup->SetSpotLightActive(0, false);
		lightGroup->SetCircleShadowActive(0, false);

		break;
	}
}
