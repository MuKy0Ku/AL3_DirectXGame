#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Input.h"

class RailCamera {
public:
	void Initialize(Vector3 trans,Vector3 rot);

	void Update();

	ViewProjection& Getviewprojection() { return viewProjection_; }
	WorldTransform& GetWorldTransform() { return worldTransform_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	// キーボード入力
	Input* input_ = nullptr;
};
