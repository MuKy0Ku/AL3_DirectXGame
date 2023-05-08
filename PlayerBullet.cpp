#include "PlayerBullet.h"
#include<cassert>

void PlayerBullet::Initialize(Model* model, const Vector3& position) 
{ 
	assert(model);
	model_ = model;

	textureHundle_ = TextureManager::Load("black.png");

	world_.Initialize();
	//引数で受け取った初期座標をリセット
	world_.translation_ = position;
}

void PlayerBullet::Update() 
{ 
	world_.UpdateMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(world_, viewProjection, textureHundle_);
}
