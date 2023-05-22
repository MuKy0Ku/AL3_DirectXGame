#include "Enemy.h"
#include<cassert>
#include"ImGuiManager.h"
#include"Input.h"
#include"MathUtility.h"
#include"EnemyBullet.h"

Enemy::~Enemy() {
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Enemy::Initialize(Model* model, const Vector3& position) { 
	assert(model);

	model_ = model;
	textureHundle_ = TextureManager::Load("sample.png");

	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	
	aaaa();
}

void Enemy::Update() { 
	// デスフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	switch (phase_) {
	case Enemy::Phase::Approach:
	default:
		ApproachUpdate();
		/*if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}*/
		break;
	case Enemy::Phase::Leave:
		LeaveUpdate();
		break;
	}
	// 弾更新
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHundle_);
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::ApproachUpdate() { 
	worldTransform_.translation_.z -= 0.1f; 
	//発射タイマーをデクリメント
	fireTimer--;
	//指定時間に達した
	if (fireTimer <= 0) {
		//弾を発射
		Fire();
		//発射タイマーを初期化
		fireTimer = kFireInterval;
	}
}

void Enemy::LeaveUpdate() { 
	worldTransform_.translation_.x -= 0.2f;
	worldTransform_.translation_.y += 0.2f; 
}

void Enemy::Fire() {
	// 弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);
	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_,velocity);

	// 弾を登録する
	bullets_.push_back(newBullet);
}

void Enemy::aaaa() {
	//発射タイマーを初期化
	fireTimer = kFireInterval;
}
