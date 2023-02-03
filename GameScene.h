#pragma once
#include "DebugCamera.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Object3d.h"
#include "Sprite.h"
#include <DirectXMath.h>

#include "Enum.h"

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

	/// <summary>
	/// 初期化まとめ
	/// </summary>
	void Reset();

	Scene GetNextScene() { return nextScene_; }

	int GetIsEnd() { return isEnd_; }

public:
	/// <summary>
	/// シーン切り替え
	/// </summary>
	void SceneChange();

  private: // メンバ変数
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	DebugText debugText;
	DebugCamera* camera = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	Sprite* spriteBG = nullptr;

	Model* modelSkydome = nullptr;
	Model* modelGround = nullptr;
	Model* modelFighter = nullptr;
	Model* modelBlock = nullptr;

	Object3d* objSkydome = nullptr;
	Object3d* objGround = nullptr;
	Object3d* objFighter = nullptr;
	Object3d* objBlock = nullptr;

	XMFLOAT3 posF, posB;
	int time = 200;
	int coolTime = 100;
	bool timerFlag = false;
	int isEnd_ = false;

	Scene nextScene_ = Scene::GAME;
};
