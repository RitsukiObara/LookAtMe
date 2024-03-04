//============================================
//
// 便利ヘッダー[useful.h]
// Author：小原立暉
//
//============================================
#ifndef _USEFUL_H_				//このマクロ定義がされていなかったら
#define _USEFUL_H_				//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "main.h"

//--------------------------------------------
// 名前空間
//--------------------------------------------
namespace useful
{
	void RotNormalize(float *pRot);													// 向きの正規化
	void Gravity(float *fGravity, float* pPos, const float fAdd);					// 重力処理
	void RotCorrect(const float fDest, float *pRot, const float fCorrect);			// 向きの補正処理(ホーミング処理)
	void Correct(const float fDest, float *pValue, const float fCorrect);			// 値の補正処理
	void ColReverse(float *pData);													// 1.0fと0.0fを反転する処理
	bool CircleCollisionXY(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2, const float fRadius1, const float fRadius2);		// 円の当たり判定(XY平面)
	bool CircleCollisionYZ(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2, const float fRadius1, const float fRadius2);		// 円の当たり判定(YZ平面)
	bool CircleCollisionXZ(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2, const float fRadius1, const float fRadius2);		// 円の当たり判定(XZ平面)
	float LineOuterProductXZ(const D3DXVECTOR3& posLeft, const D3DXVECTOR3& posRight, const D3DXVECTOR3& pos);										// 外積の左右判断処理(XZ平面)
	float LineOuterProductXY(const D3DXVECTOR3& posUp, const D3DXVECTOR3& posDown, const D3DXVECTOR3& pos);											// 外積の左右判断処理(XY平面)
	void NormalizeVector(D3DXVECTOR3* nor, const D3DXVECTOR3& posRight, const D3DXVECTOR3& posLeft, const D3DXVECTOR3& pos);	// 法線の計算(正規化)
	float InnerProduct(const D3DXVECTOR3& VecMove, const D3DXVECTOR3& VecNor);		// 内積の判定処理
	bool CollisionOuterProductXZ(D3DXVECTOR3* pTargetpos, const D3DXVECTOR3& TargetposOld, const D3DXVECTOR3 posRight, const D3DXVECTOR3 posLeft);		// 外積の当たり判定処理(XZ軸版)
	bool CollisionOuterProductXY(D3DXVECTOR3* pTargetpos, const D3DXVECTOR3& TargetposOld, const D3DXVECTOR3 posRight, const D3DXVECTOR3 posLeft);		// 外積の当たり判定処理(XY軸版)
	bool RectangleCollisionXY(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& VtxMax1, const D3DXVECTOR3& VtxMax2, const D3DXVECTOR3& VtxMin1, const D3DXVECTOR3& VtxMin2);		// XY座標の矩形の当たり判定
	bool RectangleCollisionXZ(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& VtxMax1, const D3DXVECTOR3& VtxMax2, const D3DXVECTOR3& VtxMin1, const D3DXVECTOR3& VtxMin2);		// XZ座標の矩形の当たり判定
	bool RectangleCollisionYZ(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& VtxMax1, const D3DXVECTOR3& VtxMax2, const D3DXVECTOR3& VtxMin1, const D3DXVECTOR3& VtxMin2);		// YZ座標の矩形の当たり判定
	bool CylinderCollision(D3DXVECTOR3* pTargetPos, const D3DXVECTOR3& ObjectPos, const float fObjectRadius);					// 円柱の当たり判定
	void DecimalCalculation(const int nDigit, const int nValue, int* aAnswer);		// 10進数への変換処理
	bool FrameCorrect(const float fDest, float* fTarget, const float fCorrect);		// 均等な数値の補正(float版)
	bool FrameCorrect(const int nDest, int* pTarget, const int nCorrect);			// 均等な数値の補正(int版)
	float QuadraticCurve(const float fPos, const float fStart, const float fEnd, const float fHeight);		// 二次曲線処理
	void Parabola(const float fVelo, const float fAdd, const int nCount, float* pGravity, float* pHeight);	// 放物線処理
	D3DXVECTOR3 VtxMinConv(const D3DXVECTOR3& size);		// キャラクターの頂点の最小値変換処理
}

#endif