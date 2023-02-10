#include "GameScene.h"
#include "Collision.h"
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
	//3Dオブジェクト解放
	delete object3d_1;
	delete object3d_2;
	delete object3d_3;
	//3Dモデル解放
	delete model;
	delete model_1;

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

	//OBJからモデルデータを読み込む
	model = Model::LoadFromOBJ("ground");
	model_1 = Model::LoadFromOBJ("player");
	// 3Dオブジェクト生成
	object3d_1 = Object3d::Create();
	object3d_2 = Object3d::Create();
	//object3d_3 = Object3d::Create();
	//3Dオブジェクトと3Dモデルを紐づけ
	object3d_1->SetModel(model);
	object3d_2->SetModel(model_1);
	/*object3d_3->SetModel(model_1);*/
	//3Dオブジェクトの位置を指定
	//object3d_1->SetPosition({ 0,-10,0 });
	//object3d_2->SetPosition({ 0,15,-5 });
	object3d_2->SetScale({ 1.0f, 1.0f, 1.0f });
	//object3d_3->SetPosition({ +5,0,+5 });

	//球の初期値を設定
	sphere.center = XMVectorSet(0, 2, 0, 1);//中心座標
	sphere.radius = 1.0f;//半径
	//平面の初期化を設定
	plane.normal = XMVectorSet(0, 1, 0, 0);//法線ベクトル
	plane.distance = 0.0f;//原点(0,0,0)からの距離
	//三角形の初期値を設定
	triangle.p0 = XMVectorSet(-1.0f, 0, -1.0f, 1);//左手前
	triangle.p1 = XMVectorSet(-1.0f, 0, +1.0f, 1);//左奥
	triangle.p2 = XMVectorSet(+1.0f, 0, -1.0f, 1);//右手前
	triangle.normal = XMVectorSet(0.0f, 1.0f, 0.0f, 1);//上向き
	//レイの初期値を設定
	ray.start = XMVectorSet(0, 1, 0, 1);//原点やや下
	ray.dir = XMVectorSet(0, -1, 0, 0);//下向き
}

void GameScene::Update()
{
	XMFLOAT4 color = object3d_2->GetColor();
	//// オブジェクト移動
	//if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	//{
	//	// 球移動
	//	XMVECTOR moveY = XMVectorSet(0, 0.01f, 0, 0);
	//	if (input->PushKey(DIK_UP)) { sphere.center += moveY; }
	//	else if (input->PushKey(DIK_DOWN)) { sphere.center -= moveY; }
	//	XMVECTOR moveX = XMVectorSet(0.01f, 0, 0, 0);
	//	if (input->PushKey(DIK_RIGHT)) { sphere.center += moveX; }
	//	else if (input->PushKey(DIK_LEFT)) { sphere.center -= moveX; }
	//}

	enum Dir
	{
		up,
		down
	};

	int dir = Dir::down;
	{
		XMVECTOR moveY = XMVectorSet(0, 0.1f, 0, 0);
		
		switch (dir)
		{
		case down:
			if (sphere.center.m128_f32[1] > 0.0f && dir == Dir::down) {
				sphere.center -= moveY;
				if (sphere.center.m128_f32[1] < 0.0f && dir == Dir::down) {
					dir = Dir::up;
				}
			}
			break;
		case up:
			if (sphere.center.m128_f32[1] < 1.0f && dir == Dir::up) {
				sphere.center += moveY;
				if (sphere.center.m128_f32[1] > 1.0f && dir == Dir::up) {
					dir = Dir::down;
				}
			}
			break;
		}
	}

	//stringstreamで変数の値を埋め込んで整形する
	std::ostringstream spherestr;
	spherestr << "Sphere:("
		<< std::fixed << std::setprecision(2)//小数点以下2桁まで
		<< sphere.center.m128_f32[0] << ","//x
		<< sphere.center.m128_f32[1] << ","//y
		<< sphere.center.m128_f32[2] << ")";//z

	debugText.Print(spherestr.str(), 50, 180, 1.0f);

	// 球と平面の当たり判定
	XMVECTOR inter;
	bool hit = Collision::CheckSphere2Plane(sphere, plane, &inter);
	if (hit) {
		color = { 0.0f,1.0f,0.0f,1.0f };
		object3d_2->SetColor(color);
	}
	else
	{
		color = { 1.0f,1.0f,1.0f,1.0f };
		object3d_2->SetColor(color);
	}

	//球と三角形の当たり判定
	//XMVECTOR inter;
	//bool hit = Collision::CheckSphere2Triangle(sphere, triangle, &inter);
	//if (hit) {
	//	debugText.Print("HIT", 50, 200, 1.0f);
	//	//stringstreamをリセットし、交点座標を埋め込む
	//	spherestr.str("");
	//	spherestr.clear();
	//	spherestr << "("
	//		<< std::fixed << std::setprecision(2)//小数点以下2桁まで
	//		<< inter.m128_f32[0] << ","//x
	//		<< inter.m128_f32[1] << ","//y
	//		<< inter.m128_f32[2] << ",)";//z
	// 
	//	debugText.Print(spherestr.str(), 50, 220, 1.0f);
	//}

	//レイ操作
	//{
	//	XMVECTOR moveZ = XMVectorSet(0, 0, 0.01f, 0);
	//	if (input->PushKey(DIK_UP)) { ray.start += moveZ; }
	//	else if (input->PushKey(DIK_DOWN)) { ray.start -= moveZ; }

	//	XMVECTOR moveX = XMVectorSet(0.01f, 0, 0, 0);
	//	if(input->PushKey(DIK_RIGHT)){ ray.start += moveX; }
	//	else if (input->PushKey(DIK_LEFT)) { ray.start -= moveX; }
	//}
	//
	//std::ostringstream raystr;
	//raystr<<"lay.start("
	//	<< std::fixed << std::setprecision(2)//小数点以下2桁まで
	//	<< ray.start.m128_f32[0] << ","//x
	//	<< ray.start.m128_f32[1] << ","//y
	//	<< ray.start.m128_f32[2] << ",)";//z
	// 
	//debugText.Print(spherestr.str(), 50, 180, 1.0f);

	////レイと平面の当たり判定
	//XMVECTOR inter;
	//float distance;
	//bool hit = Collision::CheckRay2Plane(ray, plane,
	//	&distance, &inter);
	//if (hit) {
	//	debugText.Print("HIT", 50, 260, 1.0f);
	//	//stringsteamをリセットし、交点座標を埋め込む
	//	raystr.str("");
	//	raystr.clear();
	//	raystr << "("
	//		<< std::fixed << std::setprecision(2)
	//		<< inter.m128_f32[0] << ","
	//		<< inter.m128_f32[1] << ","
	//		<< inter.m128_f32[2] << ")";
	// 
	//	debugText.Print(raystr.str(), 50, 280, 1.0f);
	//}

	//レイと三角形の当たり判定
	//float distance;
	//XMVECTOR inter;
	//bool hit = Collision::CheckRay2Triangle(ray, triangle, &distance, &inter);
	//if (hit) {
	//	debugText.Print("HIT", 50, 220, 1.0f);
	//	//stringsteamをリセットし、交点座標を埋め込む
	//	raystr.str("");
	//	raystr.clear();
	//	raystr << "inter:(" << std::fixed << std::setprecision(2)
	//		<< inter.m128_f32[0] << "," << inter.m128_f32[1] << "," << inter.m128_f32[2] << ")";
	//	debugText.Print(raystr.str(), 50, 240, 1.0f);
	//	raystr.str("");
	//	raystr.clear();
	//	raystr << "distance:(" << std::fixed << std::setprecision(2) << distance << ")";
	//	debugText.Print(raystr.str(), 50, 260, 1.0f);
	//}

	//レイと球の当たり判定
	//float distance;
	//XMVECTOR inter;
	//bool hit = Collision::CheakRay2Shere(ray, sphere, &distance, &inter);
	//if (hit) {
	//	debugText.Print("HIT", 50, 260, 1.0f);
	//	//stringstreamをリセットし、交点座標を埋め込む
	//	raystr.str("");
	//	raystr.clear();
	//	raystr << "inter:(" << std::fixed << std::setprecision(2)
	//		<< inter.m128_f32[0] << "," << inter.m128_f32[1] << "," << inter.m128_f32[2] << ")";
	//	debugText.Print(raystr.str(), 50, 280, 1.0f);
	//	raystr.str("");
	//	raystr.clear();
	//	raystr << "distance:(" << std::fixed << std::setprecision(2) << distance << ")";
	//	debugText.Print(raystr.str(), 50, 300, 1.0f);
	//}

	object3d_2->SetPosition({ sphere.center.m128_f32[0],
		sphere.center.m128_f32[1], sphere.center.m128_f32[2] });
	// カメラ移動
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A))
	{
		if (input->PushKey(DIK_W)) { Object3d::CameraMoveVector({ 0.0f,+1.0f,0.0f }); }
		else if (input->PushKey(DIK_S)) { Object3d::CameraMoveVector({ 0.0f,-1.0f,0.0f }); }
		if (input->PushKey(DIK_D)) { Object3d::CameraMoveVector({ +1.0f,0.0f,0.0f }); }
		else if (input->PushKey(DIK_A)) { Object3d::CameraMoveVector({ -1.0f,0.0f,0.0f }); }
	}

	//3Dオブジェクト更新
	object3d_1->Update();
	object3d_2->Update();
	/*object3d_3->Update();*/
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	/*spriteBG->Draw();*/

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
	object3d_1->Draw();
	object3d_2->Draw();
	//object3d_3->Draw();
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
	// デバッグテキストの描画
	debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}