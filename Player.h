#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Input.h"
#include"PlayerBullet.h"



class Player {
public:
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
	PlayerBullet* bullet_ = nullptr;
};
