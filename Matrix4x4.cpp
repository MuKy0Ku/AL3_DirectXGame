#include"Matrix4x4.h"
#include<cmath>

Matrix4x4 MakeScale(const Vector3& scale)
{ 
	Matrix4x4 result;

	result = {
		scale.x, 0.0f,    0.0f,    0.0f,
		0.0f,    scale.y, 0.0f,    0.0f,
	    0.0f,    0.0f,    scale.z, 0.0f,
	    0.0f,    0.0f,    0.0f,    1.0f
	};
	return result;
}

Matrix4x4 MakeRotationX(float theta) 
{ 
	float sin = std::sinf(theta);
	float cos = std::cosf(theta);
	
	Matrix4x4 result;

	result = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cos,  sin,  0.0f,
	    0.0f, -sin, cos,  0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return result;
}

Matrix4x4 MakeRotationY(float theta) {
	float sin = std::sinf(theta);
	float cos = std::cosf(theta);

	Matrix4x4 result;

	result = {
		cos,  0.0f, -sin, 0.0f, 
		0.0f, 1.0f, 0.0f, 0.0f, 
		sin,  0.0f, cos,  0.0f, 
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return result;
}

Matrix4x4 MakeRotationZ(float theta) {
	float sin = std::sinf(theta);
	float cos = std::cosf(theta);

	Matrix4x4 result;

	result = {
		cos,  sin,  0.0f, 0.0f, 
		-sin, cos,  0.0f, 0.0f, 
		0.0f, 0.0f, 1.0f, 0.0f, 
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return result;
}

Matrix4x4 MakeTranslation(Vector3 trans) 
{
	Matrix4x4 result = {
	    1.0f,    0.0f,    0.0f,    0.0f,
		0.0f,    1.0f,    0.0f,    0.0f,
	    0.0f,    0.0f,    1.0f,    0.0f, 
		trans.x, trans.y, trans.z, 1.0f,
	};
	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& trans) 
{
	//スケーリング行列
	Matrix4x4 matScale = MakeScale(scale);

	//回転行列
	Matrix4x4 matRotX = MakeRotationX(rot.x);
	Matrix4x4 matRotY = MakeRotationY(rot.y);
	Matrix4x4 matRotZ = MakeRotationZ(rot.z);
	//合成(Z * X * Y)
	Matrix4x4 matRot = matRotZ * matRotX * matRotY;

	//平行移動行列
	Matrix4x4 matTrans = MakeTranslation(trans);

	//合成
	Matrix4x4 result = matScale * matRot * matTrans;

	return result;
}

Matrix4x4& operator*=(Matrix4x4& m1, const Matrix4x4& m2) 
{ 
	Matrix4x4 result = {};

	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			for (size_t k = 0; k < 4; k++) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}

	m1 = result;
	return m1; 
}

Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) 
{
	Matrix4x4 result = m1;

	return result *= m2; 
}
