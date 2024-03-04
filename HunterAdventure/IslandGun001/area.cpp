//===========================================
//
// エリアのメイン処理[area.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "area.h"

//===================
// 区分の設定処理
//===================
int area::SetFieldIdx(const D3DXVECTOR3& pos)
{
	for (int nCnt = 0; nCnt < area::NUM_AREA; nCnt++)
	{
		if (pos.x >= area::AREA_RANGE[nCnt][0].x &&
			pos.x <= area::AREA_RANGE[nCnt][1].x &&
			pos.z >= area::AREA_RANGE[nCnt][0].z &&
			pos.z <= area::AREA_RANGE[nCnt][1].z)
		{ // 範囲内にあった場合

			// 区分の番号を返す
			return nCnt;
		}
	}

	// 0を返す(緊急用)
	return 0;
}

//===================
// 番号の判定処理
//===================
bool area::IndexCheck(const int nIdx)
{
	if (nIdx >= 0 &&
		nIdx < NUM_AREA)
	{ // 番号が範囲内だった場合

		// true を返す
		return true;
	}
	else
	{ // 上記以外

		// falseを返す
		return false;
	}
}