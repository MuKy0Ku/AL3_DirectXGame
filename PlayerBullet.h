#pragma once
#include"Model.h"
#include"Vector3.h"
#include"ViewProjection.h"
#include"WorldTransform.h"

class PlayerBullet {
public:
	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(const ViewProjection& viewProjection);

private:
	WorldTransform world_;
	Model* model_;
	uint32_t textureHundle_;
};
