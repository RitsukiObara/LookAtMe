//===================================
//
// ボスの当たり判定ヘッダー[boss_collision.h]
// Author 小原立暉
//
//===================================
#ifndef _BOSS_COLLISION_H_
#define _BOSS_COLLISION_H_

//***********************************
// インクルードファイル
//***********************************
#include "model.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CBoss;		// ボス

// クラス定義(ボスの出現状態クラス)
class CBossCollision
{
public:

	// 構造体定義(当たり判定)
	struct SCollision
	{
		D3DXVECTOR3 offset;			// オフセット座標
		float fRadius;				// 半径
		bool bWeakness;				// 弱点状況
	};

	CBossCollision();				// コンストラクタ
	~CBossCollision();				// デストラクタ

	void Uninit(void);				// 終了処理

	void SetData(const int nNum);	// 情報の設定処理

	// セット・ゲット関係
	void SetColl(const SCollision& coll, const int nIdx);				// 当たり判定の情報の設定処理
	SCollision GetColl(const int nIdx) const;							// 当たり判定の情報の取得処理
	void SetCollOffset(const D3DXVECTOR3& offset, const int nIdx);		// オフセット座標の設定処理
	D3DXVECTOR3 GetCollOffset(const int nIdx) const;					// オフセット座標の取得処理
	void SetRadius(const float fRadius, const int nIdx);				// 半径の設定処理
	float GetRadius(const int nIdx) const;								// 半径の取得処理
	void SetEnableWeakness(const bool bWeak, const int nIdx);			// 弱点状況の設定処理
	bool IsWeakness(const int nIdx) const;								// 弱点状況の取得処理
	void SetNumColl(const int nNum);				// 当たり判定の総数の設定処理
	int GetNumColl(void) const;						// 当たり判定の総数の取得処理

	// 静的メンバ関数
	static CBossCollision* Create(const int nNum);	// 生成処理

	// 定数定義
	static const int MAX_COLLISION = 10;	// 当たり判定の最大数

private:

	// メンバ関数
	SCollision m_coll[MAX_COLLISION];		// 当たり判定の情報
	int m_nNumColl;							// 当たり判定の総数
};

#endif