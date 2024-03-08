//===================================
//
// ボスの通常状態ヘッダー[boss_nonestate.h]
// Author 小原立暉
//
//===================================
#ifndef _BOSS_NONE_STATE_H_
#define _BOSS_NONE_STATE_H_
//***********************************
// インクルードファイル
//***********************************
#include "boss_state.h"

// クラス定義(ボスの通常状態クラス)
class CBossNoneState : public CBossState
{
public:

	CBossNoneState();	// コンストラクタ
	~CBossNoneState();	// デストラクタ

	void Process(CBoss* pBoss) override;		// 状態処理
	void SetData(CBoss* pBoss) override;		// 情報の設定処理

private:

	// メンバ関数
	void StateSelect(CBoss* pBoss);				// 状態の選択処理

	// メンバ変数
	int m_nCount;		// 経過カウント
	float m_fGravity;	// 重力
};

#endif