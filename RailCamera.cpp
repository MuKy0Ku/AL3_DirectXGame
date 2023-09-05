#include "RailCamera.h"
#include"ImGuiManager.h"
#include"Input.h"

void RailCamera::Initialize(Vector3 trans, Vector3 rot) { 
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	worldTransform_.translation_ = trans;
	worldTransform_.rotation_ = rot;
}

void RailCamera::Update() { 
	Vector3 move = {0, 0, 0.0f};
	Vector3 rot = {0, 0, 0};

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	worldTransform_.rotation_.x += rot.x;
	worldTransform_.rotation_.y += rot.y;
	worldTransform_.rotation_.z += rot.z;

	worldTransform_.UpdateMatrix();

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	viewProjection_.TransferMatrix();

	//// 押した方向で移動ベクトルを変更(左右)
	//if (input_->PushKey(DIK_A)) {
	//	move.x -= 0.2f;
	//} else if (input_->PushKey(DIK_D)) {
	//	move.x += 0.2f;
	//}

	//// 押した方向で移動ベクトルを変更(上下)
	//if (input_->PushKey(DIK_W)) {
	//	move.y += 0.2f;
	//} else if (input_->PushKey(DIK_S)) {
	//	move.y -= 0.2f;
	//}
	
#ifdef  _DEBUG
	//カメラの座標を画面表示する処理
	ImGui::SetNextWindowSize({300, 100});

	ImGui::Begin("Camera");
	float sliderValue[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	float sliderValueRot[3] = {
	    worldTransform_.rotation_.x, worldTransform_.rotation_.y,
	    worldTransform_.rotation_.z,
	};
	ImGui::SliderFloat3("transration", sliderValue, -100.0f, 100.0f);
	ImGui::SliderFloat3("rotation", sliderValueRot, -1.0f, 1.0f);
	worldTransform_.translation_ = {sliderValue[0], sliderValue[1], sliderValue[2]};
	worldTransform_.rotation_ = {sliderValueRot[0], sliderValueRot[1], sliderValueRot[2]};
	ImGui::End();
#endif //  _DEBUG
}
