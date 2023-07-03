#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Input.h"
#include"PlayerBullet.h"
#include<list>
#include"Sprite.h"



class Player {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();
	
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(ViewProjection& viewProjection);
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection&viewProjection);

	void DrawUI();

	void Attack();
    
	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	void SetParent(const WorldTransform* parent);
	
	// ワールド座標を取得
	Vector3 GetWorldPosition();

	const float GetRadius() { return radius_; }
	const float radius_ = 1.0f;
	
	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }

 private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//キーボード入力
	Input* input_ = nullptr;

	//弾
	std::list<PlayerBullet*> bullets_;
	//PlayerBullet* bullet_ = nullptr;

	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3Dreticle_;

	//2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;

	Vector2 spritePos = {640, 360};
};
