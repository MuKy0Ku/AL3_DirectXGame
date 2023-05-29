﻿#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Input.h"
#include"PlayerBullet.h"
#include<list>



class Player {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();
	
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model*model,uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection&viewProjection);

	void Attack();
    
	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();
	
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

	
	
};
