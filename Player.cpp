#include "Player.h"
#include<cassert>
#include"ImGuiManager.h"
#include"Input.h"
#include"MathUtility.h"

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;
}

void Player::Initialize(Model* model, uint32_t textureHandle, const Vector3& position) { 
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	// レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("target.png");

	// スプライト生成
	sprite2DReticle_ = Sprite::Create(textureReticle, spritePos, {1, 1, 1, 1}, {(0.5f), (0.5f)});

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	//3Dレティクルのワールドトランスフォーム初期化
	worldTransform3Dreticle_.Initialize();
}

void Player::Update(ViewProjection& viewProjection) { 
	/*Vector3 positionReticle = worldTransform3Dreticle_.translation_;

	//ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matviewProjectionViewport =
	    viewProjection.matView * viewProjection.matProjection * matViewport;

	//ワールド→スクリーン座標行列(ここで2Dから3Dになる)
	positionReticle = Transform(positionReticle, matviewProjectionViewport);

	//スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));*/

	// ビューポート行列
	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	POINT mousePosision;
	//マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosision);

	//クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosision);

	//マウス座標を2Dレティクルのスプライトに代入する
	sprite2DReticle_->SetPosition(Vector2(float(mousePosision.x),float(mousePosision.y)));
	
	//ビュープロジェクションビューポート合成行列
	Matrix4x4 matVPV = viewProjection.matView * viewProjection.matProjection * matViewport;

	//合成行列の逆行列を計算する
	Matrix4x4 matInverseVPV = Inverse(matVPV);

	//スクリーン座標
	Vector3 posNear = Vector3(float(mousePosision.x), float(mousePosision.y), 0);
	Vector3 posFar = Vector3(float(mousePosision.x), float(mousePosision.y), 1);

	//スクリーン座標系からワールド座標系へ
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	//マウスレイの方向
	Vector3 mouseDirection = {posFar.x - posNear.x, posFar.y - posNear.y, posFar.z - posNear.z};
	mouseDirection = Normalize(mouseDirection);

	//カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 80.0f;
	worldTransform3Dreticle_.translation_.x = (posNear.x + mouseDirection.x) * kDistanceTestObject;
	worldTransform3Dreticle_.translation_.y = (posNear.y + mouseDirection.y) * kDistanceTestObject;
	worldTransform3Dreticle_.translation_.z = (posNear.z + mouseDirection.z) * kDistanceTestObject;

	//worldTransform3DReticle_のワールド行列更新と転送
	worldTransform3Dreticle_.UpdateMatrix();

	//デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動の速さ
	const float kCharacterSpeed = 0.2f;
	
	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	}else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	}else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}
	
	//キャラクター旋回
	//押した方向で移動ベクトルを変更
    const float kRotSpeed = 0.02f;
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	}
	if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}

	//キャラクター攻撃処理
	Attack();

	//弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	//移動限界座標
	const float kMoveLimitX = 20.0f;
	const float kMoveLimitY = 20.0f;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	
	worldTransform_.UpdateMatrix();

	// 自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;
	// 自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset = {0, 0, 1.0f};
	//自機のワールド行列の回転を反映
	offset = TransFormNormal(offset, worldTransform_.matWorld_);
	//ベクトルの長さを整える
	float length = sqrtf((offset.x * offset.x) + (offset.y * offset.y) + (offset.z * offset.z));
	Vector3 dir(offset.x / length, offset.y / length, offset.z / length);
	offset = {
	    dir.x * kDistancePlayerTo3DReticle,
	    dir.y * kDistancePlayerTo3DReticle,
	    dir.z * kDistancePlayerTo3DReticle,
	};
	//3Dレティクルの座標を設定
	/*worldTransform3Dreticle_.translation_ = { worldTransform_.translation_.x + offset.x,
	                                          worldTransform_.translation_.y + offset.y, 
		                                      worldTransform_.translation_.z + offset.z};
	worldTransform3Dreticle_.UpdateMatrix();*/


#ifdef _DEBUG
	
	//座標の設定
	ImGui::SetNextWindowSize({300, 100});
	
	ImGui::Begin("player");
	float sliderValue[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("sliderFloat3", sliderValue, -20.0f, 20.0f);
	worldTransform_.translation_ = {sliderValue[0], sliderValue[1], sliderValue[2]};
	ImGui::End();
#endif // _DEBUG

	/*ImGui::Begin("Player");
	ImGui::Text("2DReticle:(%f,%f)",spritePos.x,spritePos.y);
	ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text("3DReticle(%+.2f,%+.2f,%+.2f)", worldTransform3Dreticle_.translation_.x,
	    worldTransform3Dreticle_.translation_.y, worldTransform3Dreticle_.translation_.z);
	ImGui::End();*/
}

void Player::Draw(ViewProjection& viewProjection) { 
    model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//model_->Draw(worldTransform3Dreticle_, viewProjection, textureHandle_);
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::DrawUI() { 
	sprite2DReticle_->Draw(); 
}

void Player::Attack() { 
	if (input_->TriggerKey(DIK_SPACE)) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		//velocity = TransFormNormal(velocity, worldTransform_.matWorld_);
		velocity = {
		    worldTransform3Dreticle_.translation_.x - worldTransform_.translation_.x,
		    worldTransform3Dreticle_.translation_.y - worldTransform_.translation_.y,
		    worldTransform3Dreticle_.translation_.z - worldTransform_.translation_.z,
		};
		Vector3 n = Normalize(velocity);
		velocity.x = n.x * kBulletSpeed;
		velocity.y = n.y * kBulletSpeed;
		velocity.z = n.z * kBulletSpeed;
		//弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);
		
		//弾を登録する
		bullets_.push_back(newBullet);
	}
}

void Player::OnCollision() {}

void Player::SetParent(const WorldTransform* parent) {
	//親子関係を結ぶ
	worldTransform_.parent_ = parent;
}

Vector3 Player::GetWorldPosition() { 
	//ワールド座標を入れる変数
	Vector3 worldPos{};
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos; 
}


