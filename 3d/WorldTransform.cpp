#include "WorldTransform.h"

void WorldTransform::UpdateMatrix() 
{
	//行列更新
	matWorld_ = MakeAffineMatrix(
	    scale_, rotation_, translation_);

	//行列転送
	TransferMatrix();
}