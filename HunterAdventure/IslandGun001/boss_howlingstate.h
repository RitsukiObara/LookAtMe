//===================================
//
// ボスの雄たけび状態ヘッダー[boss_howlingstate.h]
// Author 小原立暉
//
//===================================
#ifndef _BOSS_HOWLING_STATE_H_
#define _BOSS_HOWLING_STATE_H_
//***********************************
// インクルードファイル
//***********************************
#include "boss_state.h"

// クラス定義(ボスの遠吠え状態クラス)
class CBossHowlingState : public CBossState
{
public:

	CBossHowlingState();	// コンストラクタ
	~CBossHowlingState();	// デストラクタ

	void Process(CBoss* pBoss) override;		// 状態処理
	void SetData(CBoss* pBoss) override;		// 情報の設定処理

private:

	// メンバ変数
	int m_nCount;		// 経過カウント
};

#endif