#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"ImGuiManager.h"
#include"AxisIndicator.h"
#include"MathUtility.h"

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_; 
	delete player_;
	delete debugCamera_;
	delete enemy_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	playerHandle_ = TextureManager::Load("sample.png");
	model_ = Model::Create();
	viewProjection_.Initialize();
	player_ = new Player();
	player_->Initialize(model_, playerHandle_);
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	enemy_ = new Enemy();
	enemy_->Initialize(model_, enemyPos_);
}

void GameScene::Update() { 
	player_->Update();
	debugCamera_->Update();
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) 
	{
		isDebugCameraActive_ = true;
	}
#endif // _DEBUG
	//カメラの処理
	if (isDebugCameraActive_) 
	{
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		//ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		//ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}
	enemy_->Update();
	if (enemy_) {
		
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//model_->Draw(worldTransform_, viewProjection_, playeyrHandle_);
	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);
	if (enemy_) {
		
	}
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
