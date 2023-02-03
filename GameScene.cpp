#include "GameScene.h"
#include "Model.h"
#include <cassert>
#include <sstream>
#include <iomanip>

using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete spriteBG;
	delete objSkydome;
	delete objGround;
	delete objFighter;
	delete modelSkydome;
	delete modelGround;
	delete modelFighter;
	delete camera;
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
	/*Sprite::LoadTexture(1, L"Resources/background.png");*/

    // カメラ生成
	camera = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight,input);

	// カメラ注視点をセット
	camera->SetTarget({0, 1, 0});
	camera->SetDistance(3.0f);

    // 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera);

	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	// 3Dオブジェクト生成
	objSkydome = Object3d::Create();
	objGround = Object3d::Create();
	objFighter = Object3d::Create();
	objBlock = Object3d::Create();

	// テクスチャ2番に読み込み
	/*Sprite::LoadTexture(2, L"Resources/texture.png");*/

	modelSkydome = Model::CreateFromOBJ("skydome");
	modelGround = Model::CreateFromOBJ("ground");
	modelFighter = Model::CreateFromOBJ("chr_sword");
	modelBlock = Model::CreateFromOBJ("Box");

	objSkydome->SetModel(modelSkydome);
	objGround->SetModel(modelGround);
	objFighter->SetModel(modelFighter);
	objBlock->SetModel(modelBlock);


	objBlock->SetPosition({ 0,+1,+20 });
}

void GameScene::Update()
{
	camera->Update();
	Object3d::SetCamera(camera);

	objSkydome->Update();
	objGround->Update();
	objFighter->Update();
	objBlock->Update();

	XMFLOAT3 position = objFighter->GetPosition();
	if (input->TriggerKey(DIK_A)) {
		timerFlag = true;
		position.x -= 2.5;
		if (position.x < -2.6)
		{
			position.x = -2.5;
		}
	}
	if (coolTime <= 0 && timerFlag == true)
	{
		debugText.Print("bbbb", 50, 130, 1.0f);
		timerFlag = false;
		coolTime = 150;
		position.x = 0;
	}

	if (timerFlag == true)
	{
		coolTime--;
	}
	objFighter->SetPosition(position);
	XMFLOAT3 move = objBlock->GetPosition();
	time--;
	if (time <= 0)
	{
		move.z -= 2;
	}

	objBlock->SetPosition(move);

#pragma region 当たり判定

	posF = objFighter->GetPosition();
	posB = objBlock->GetPosition();
	{
		float a = std::pow(posB.x - posF.x, 2.0f) + std::pow(posB.y - posF.y, 2.0f) +
			std::pow(posB.z - posF.z, 2.0f);
		float lenR = std::pow((objBlock->r + objFighter->r), 1.0);

		if (a <= lenR) {
			// 自キャラ
			objFighter->OnColision();
			// 敵
			objBlock->OnColision();
			debugText.Print("aaaaaaaa", 50, 110, 1.0f);
	
		}
	}
#pragma endregion

	SceneChange();
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
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	objSkydome->Draw();
	objGround->Draw();
	objFighter->Draw();
	objBlock->Draw();
	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
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
}

void GameScene::Reset()
{
	objBlock->SetPosition({ 0,+1,+20 });
	objFighter->SetIsDead(false);
	objFighter->SetPosition({ 0,0,0 });
	isEnd_ = false;
	timerFlag = false;
	time = 200;
	coolTime = 100;
}

void GameScene::SceneChange()
{
	isEnd_ = false;
 	if (objFighter->IsDead()) {

		nextScene_ = Scene::END;
		isEnd_ = true;
	}
	if (objBlock->GetPosition().z <= -10) {
		nextScene_ = Scene::CLREA;
		isEnd_ = true;
	}
}

