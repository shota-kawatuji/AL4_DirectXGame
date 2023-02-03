#include "DirectXCommon.h"
#include "WinApp.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "Clear.h"
#include "End.h" 

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
{
	// 汎用機能
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;	
	GameScene* gameScene = nullptr;
	TitleScene* titleScene = nullptr;
	Clear* clearScene = nullptr;
	EndScene* endScene = nullptr;

	Scene scene = Scene::TITLE;
	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow();
		
	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// 入力の初期化
	input = new Input();
	input->Initialize(win->GetHInstance(), win->GetHwnd());

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);
	
	// 3Dオブジェクト静的初期化
	Object3d::StaticInitialize(dxCommon->GetDevice());
#pragma endregion
	//タイトルの初期化
	titleScene = new TitleScene();
	titleScene->Initialize(dxCommon, input);
	// ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initialize(dxCommon, input);
	//
	clearScene = new Clear();
	clearScene->Initialize(dxCommon, input);
	
	endScene= new EndScene();
	endScene->Initialize(dxCommon, input);
	// メインループ
	while (true)
	{
		// メッセージ処理
		if (win->ProcessMessage()) { break; }

		// 入力関連の毎フレーム処理
		input->Update();
		//---追加---
		switch (scene)
		{
		case Scene::TITLE:
			titleScene->Update();
			if (titleScene->GetChangeFlag()) {
				scene = titleScene->GetNextScene();
			}
			break;
		case Scene::GAME:
			// ゲームシーンの毎フレーム処理
			gameScene->Update();
			if (gameScene->GetIsEnd()) {
				gameScene->Reset();
				scene = gameScene->GetNextScene();
			}
			break;
		case Scene::CLREA:
			// ゲームシーンの毎フレーム処理
			clearScene->Update();
			if (clearScene->GetChangeFlag()) {
				scene = clearScene->GetNextScene();
			}
			break;
		case Scene::END:
			endScene->Update();
			if (endScene->GetChangeFlag()) {
				scene = endScene->GetNextScene();
			}
		}
		// 描画開始
		dxCommon->PreDraw();
		//---追加---
		switch (scene)
		{
		case Scene::TITLE:
			titleScene->Draw();
			break;
		case Scene::GAME:
			gameScene->Draw();
			break;
		case Scene::CLREA:
			clearScene->Draw();
			break;
		case Scene::END:
			endScene->Draw();
			break;

		}
		// 描画終了
		dxCommon->PostDraw();
	}
	// 各種解放
	delete titleScene;
	delete gameScene;
	delete input;

	// DirectX終了処理
	dxCommon->Finalize();
	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}