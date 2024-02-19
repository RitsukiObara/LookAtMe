//===================================
//
// ボスの気絶状態ヘッダー[boss_stunstate.h]
// Author 小原立暉
//
//===================================
#ifndef _BOSS_STUN_STATE_H_
#define _BOSS_STUN_STATE_H_

//***********************************
// インクルードファイル
//***********************************
#include "boss_state.h"

// クラス定義(ボスの気絶状態クラス)
class CBossStunState : public CBossState
{
public:

	CBossStunState();		// コンストラクタ
	~CBossStunState();		// デストラクタ

	void Process(CBoss* pBoss) override;		// 状態処理
	void SetData(CBoss* pBoss) override;		// 情報の設定処理

private:

	// メンバ変数
	int m_nCount;		// 経過カウント
};

#endif