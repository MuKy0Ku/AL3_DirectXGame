#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include"Player.h"
#include"DebugCamera.h"
#include"Enemy.h"
#include"Skydome.h"
#include"RailCamera.h"
#include"list"
#include<sstream>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

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
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


	void AddEnemy(Vector3 pos);

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet"></param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);


    /// <summary>
    /// 衝突判定と応答
    /// </summary>
	void CheckAllCollisions();

    /// <summary>
    /// 敵発生データ読み込み
    /// </summary>
	void LoadEnemypopData();

    /// <summary>
    ///敵発生コマンドの更新 
    /// </summary>
	void UpdateEnemyPopCommands();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	
	/// <summary>
	/// ゲームシーン用
	/// </summary>
	uint32_t playerHandle_ = 0;
	Model* model_ = nullptr;
	ViewProjection viewProjection_;
	Player* player_ = nullptr;
	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	//敵
	std::list<Enemy*> enemies_;
	WorldTransform worldTransform_;

	//skydome
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;

	//レールカメラ
	RailCamera* railCamera_ = nullptr;

	std::list<EnemyBullet*> bullets_;

	//敵発生コマンド
	std::stringstream enemyPopCommands;

	bool waitFlag = false;
	int waitTimer;
};
