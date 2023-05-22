﻿#pragma once
#include"Model.h"
#include"Vector3.h"
#include"ViewProjection.h"
#include"WorldTransform.h"
#include"EnemyBullet.h"
#include"Input.h"
#include<list>

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
	static const int kFireInterval = 60;

	//接近フェーズ初期化
	void ApproachInitialize();

private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHundle_;
	
	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};
	//フェーズ
	Phase phase_ = Phase::Approach;

	//弾
	std::list<EnemyBullet*> bullets_;
	//EnemyBullet* bullet_ = nullptr;

	// キーボード入力
	Input* input_ = nullptr;

	//発射タイマー
	int32_t fireTimer = 0;
};
