#pragma once
#include"Model.h"
#include"Vector3.h"
#include"ViewProjection.h"
#include"WorldTransform.h"
#include"EnemyBullet.h"
#include"Input.h"
#include<list>

//自機クラスの前方宣言
class Player;

//GameSceneの前方宣言
class  GameScene;

class Enemy {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();

	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	//接近
	void ApproachUpdate();

	//離脱
	void LeaveUpdate();

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	//発射間隔
	static const int kFireInterval = 90;

	//接近フェーズ初期化
	void ApproachInitialize();

	void SetPlayer(Player* player) { player_ = player; }

	void SetGameScene(GameScene* gameScene) { gamescene_ = gameScene; }

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();
	bool IsDead() const { return isDead_; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	const float GetRadius() { return radius_; }
	const float radius_ = 2.0f;

	// 弾リストを取得
	//const std::list<EnemyBullet*>& GetBullets() { return bullets_; }

private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHundle_;
	
	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};
	//フェーズ
	Phase phase_ = Phase::Leave;

	//弾
	//std::list<EnemyBullet*> bullets_;
	//EnemyBullet* bullet_ = nullptr;

	// キーボード入力
	Input* input_ = nullptr;

	//発射タイマー
	int32_t fireTimer = 0;
	int32_t moveTimer = 120;

	//自キャラ
	Player* player_ = nullptr;

	//ゲームシーン
	GameScene* gamescene_ = nullptr;

	// 寿命<frm>
	//static const int32_t klifeTime = 60 * 5;
	// デスタイマー
	int32_t deathTimer_ = 300;
	// デスフラグ
	bool isDead_ = false;
};
