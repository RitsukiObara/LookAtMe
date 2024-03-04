//===================================
//
// ボスの登場状態ヘッダー[boss_appearstate.h]
// Author 小原立暉
//
//===================================
#ifndef _BOSS_APPEAR_STATE_H_
#define _BOSS_APPEAR_STATE_H_
//***********************************
// インクルードファイル
//***********************************
#include "boss_state.h"

// クラス定義(ボスの出現状態クラス)
class CBossAppearState : public CBossState
{
public:

	CBossAppearState();		// コンストラクタ
	~CBossAppearState();	// デストラクタ

	void Process(CBoss* pBoss) override;		// 状態処理
	void SetData(CBoss* pBoss) override;		// 情報の設定処理

private:

	// メンバ関数
	void Length(void);			// 距離関係処理
	void PosRot(CBoss* pBoss);	// 位置向き設定処理
	void Ripple(CBoss* pBoss);	// 波紋の設置処理
	void LightCol(void);		// 光の色の設定処理

	// メンバ変数
	int m_nCount;			// 経過カウント
	float m_fLangeRot;		// 離れる向き
	float m_fLength;		// 距離
};

#endif