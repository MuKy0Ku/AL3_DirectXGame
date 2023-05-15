#include "PlayerBullet.h"
#include<cassert>

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) { 
	assert(model);
	model_ = model;

	textureHundle_ = TextureManager::Load("black.png");

	world_.Initialize();
	//引数で受け取った初期座標をリセット
	world_.translation_ = position;
	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
}

void PlayerBullet::Update() 
{ 
	//座標を移動させる(1フレーム分の移動量を足しこむ)
	world_.translation_.x += velocity_.x;
	world_.translation_.y += velocity_.y;
	world_.translation_.z += velocity_.z;

	world_.UpdateMatrix();

	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(world_, viewProjection, textureHundle_);
}
