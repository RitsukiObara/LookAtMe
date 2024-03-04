//===================================
//
// ボスの炎攻撃状態ヘッダー[boss_firestate.h]
// Author 小原立暉
//
//===================================
#ifndef _BOSS_FIRE_STATE_H_
#define _BOSS_FIRE_STATE_H_

//***********************************
// インクルードファイル
//***********************************
#include "boss_state.h"

//-----------------------------------
// クラス定義(ボスの炎攻撃状態クラス)
//-----------------------------------
class CBossFireState : public CBossState
{
public:

	CBossFireState();		// コンストラクタ
	~CBossFireState();		// デストラクタ

	void Process(CBoss* pBoss) override;		// 状態処理
	void SetData(CBoss* pBoss) override;		// 情報の設定処理

private:

	// メンバ変数
	int m_nCount;				// 経過カウント
};

#endif