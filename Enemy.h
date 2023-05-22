#pragma once
#include"Model.h"
#include"Vector3.h"
#include"ViewProjection.h"
#include"WorldTransform.h"

class Enemy {
public:
	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	//接近
	void ApproachUpdate();

	//離脱
	void LeaveUpdate();

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
};
