//===================================
//
// ボスの向き直り状態ヘッダー[boss_turnstate.h]
// Author 小原立暉
//
//===================================
#ifndef _BOSS_TURN_STATE_H_
#define _BOSS_TURN_STATE_H_
//***********************************
// インクルードファイル
//***********************************
#include "boss_state.h"

// クラス定義(ボスの向き直り状態クラス)
class CBossTurnState : public CBossState
{
public:

	CBossTurnState();	// コンストラクタ
	~CBossTurnState();	// デストラクタ

	void Process(CBoss* pBoss) override;		// 状態処理
	void SetData(CBoss* pBoss) override;		// 情報の設定処理

private:

	// メンバ関数
	void Fraction(CBoss* pBoss);		// 破片発生処理

	// メンバ変数
	int m_nCount;		// 経過カウント
};

#endif