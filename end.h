#pragma once
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Sprite.h"
#include "DebugCamera.h"

#include "Enum.h"
#include "GameScene.h"
class EndScene
{
private: // 静的メンバ変数
	static const int debugTextTexNumber = 0;

public://メンバ関数

	/// <summary>
	/// コンストクラタ
	/// </summary>
	EndScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EndScene();

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

	Scene GetNextScene() { return nextScene_; }

	int GetChangeFlag() { return changeFlag_; }
private:
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	DebugText debugText;
	DebugCamera* camera = nullptr;

	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	Scene nextScene_ = Scene::END;

	int changeFlag_ = false;
	//スプライト
	Sprite* spriteBG = nullptr;
};

