#include "Enemy.h"
#include<cassert>

void Enemy::Initialize(Model* model, const Vector3& position) { 
	assert(model);

	model_ = model;
	textureHundle_ = TextureManager::Load("sample.png");

	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
}

void Enemy::Update() { 
	worldTransform_.translation_.z -= 0.3f;

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHundle_);
}