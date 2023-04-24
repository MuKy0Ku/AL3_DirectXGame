#pragma once
#include"Vector3.h"

/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {
	float m[4][4];
};

//スケーリング行列
Matrix4x4 MakeScale(const Vector3& scale);

//回転行列
Matrix4x4 MakeRotationX(float theta);

Matrix4x4 MakeRotationY(float theta);

Matrix4x4 MakeRotationZ(float theta);

//平行移動行列
Matrix4x4 MakeTranslation(Vector3 trans);

//アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& trans);

//代入演算子オーバーロード
Matrix4x4& operator*=(Matrix4x4& m1, const Matrix4x4& m2);

//2項演算子オーバーロード
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);
