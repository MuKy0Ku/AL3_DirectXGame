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
	delete skydome_;
	delete modelSkydome_;
	delete railCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	playerHandle_ = TextureManager::Load("sample.png");
	model_ = Model::Create();
	viewProjection_.Initialize();
	Vector3 playerPosition(0, 0, 40);
	player_ = new Player();
	player_->Initialize(model_, playerHandle_, playerPosition);
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	enemy_ = new Enemy();
	enemy_->Initialize(model_, enemyPos_);
	
	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);

	railCamera_ = new RailCamera();
	railCamera_->Initialize({0, 0, 0}, {0, 0, 0});
	//自キャラとレールカメラの親子関係を結ぶ
	
	player_->SetParent(&railCamera_->GetWorldTransform());
}

void GameScene::Update() { 
	player_->Update();
	debugCamera_->Update();
	skydome_->Update();
	railCamera_->Update();
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
	} else if (!isDebugCameraActive_){
		viewProjection_.matView = railCamera_->Getviewprojection().matView;
		viewProjection_.matProjection = railCamera_->Getviewprojection().matProjection;
		//ビュープロジェクション行列の更新と転送
		viewProjection_.TransferMatrix();
	}
	
	if (enemy_) {
		enemy_->Update();
	}
	CheckAllCollisions();
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
	
	if (enemy_) {
		enemy_->Draw(viewProjection_);
	}

	skydome_->Draw(viewProjection_);
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

void GameScene::CheckAllCollisions() { 
	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	//敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	//自キャラと敵弾の当たり判定
    #pragma region
	#pragma endregion
	posA = player_->GetWorldPosition();
	
	for (EnemyBullet*bullet : enemyBullets) {
		posB = bullet->GetWorldPosition();
		float c = ((posA.x - posB.x) * (posA.x - posB.x) + (posA.y - posB.y) * (posA.y - posB.y) +
		     (posA.z - posB.z) * (posA.z - posB.z));
		if (c <= enemy_->GetRadius() + player_->GetRadius()) {
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

	// 自弾と敵キャラの当たり判定
    #pragma region
    #pragma endregion
	posA = enemy_->GetWorldPosition();

	for (PlayerBullet* bullet : playerBullets) {
		posB = bullet->GetWorldPosition();
		float c =
		    ((posA.x - posB.x) * (posA.x - posB.x) + (posA.y - posB.y) * (posA.y - posB.y) +
		     (posA.z - posB.z) * (posA.z - posB.z));
		if (c <= enemy_->GetRadius() + player_->GetRadius()) {
			// 敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
			// 自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

	// 自弾と敵弾の当たり判定
    #pragma region
    #pragma endregion
	for (PlayerBullet* bulletA : playerBullets) {
		for (EnemyBullet* bulletB : enemyBullets) {
		posA = bulletA->GetWorldPosition();
		posB = bulletB->GetWorldPosition();
		float c =
		    ((posA.x - posB.x) * (posA.x - posB.x) + (posA.y - posB.y) * (posA.y - posB.y) +
		     (posA.z - posB.z) * (posA.z - posB.z));
		if (c <= enemy_->GetRadius() + player_->GetRadius()) {
			// 自弾の衝突時コールバックを呼び出す
			bulletA->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bulletB->OnCollision();
		}
		}
	}
}
