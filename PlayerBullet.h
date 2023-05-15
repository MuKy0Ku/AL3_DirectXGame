#pragma once
#include"Model.h"
#include"Vector3.h"
#include"ViewProjection.h"
#include"WorldTransform.h"

class PlayerBullet {
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

private:
	WorldTransform world_;
	Model* model_;
	uint32_t textureHundle_;

	//速度
	Vector3 velocity_;
	//寿命<frm>
	static const int32_t klifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer_ = klifeTime;
	//デスフラグ
	bool isDead_ = false;
};
