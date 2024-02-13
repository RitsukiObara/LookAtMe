//===================================
//
// ボスの飛行状態ヘッダー[boss_flystate.h]
// Author 小原立暉
//
//===================================
#ifndef _BOSS_FLY_STATE_H_
#define _BOSS_FLY_STATE_H_
//***********************************
// インクルードファイル
//***********************************
#include "boss_state.h"

// クラス定義(ボスの飛行状態クラス)
class CBossFlyState : public CBossState
{
public:

	CBossFlyState();	// コンストラクタ
	~CBossFlyState();	// デストラクタ

	void Process(CBoss* pBoss) override;		// 状態処理
	void SetData(CBoss* pBoss) override;		// 情報の設定処理

private:

	// メンバ関数
	void RotMove(CBoss* pBoss);		// 向きの移動処理
	void Standby(CBoss* pBoss);		// スタンバイ処理 
	void Fly(CBoss* pBoss);			// 飛行処理
	void Ripple(CBoss* pBoss);		// 波紋の生成処理

	// メンバ変数
	D3DXVECTOR3 m_moveDest;			// 目的の移動量
	D3DXVECTOR3 m_move;				// 移動量
	int m_nCount;					// 経過カウント
};

#endif