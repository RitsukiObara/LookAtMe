//===================================
//
// ボスのチャージ状態ヘッダー[boss_chargestate.h]
// Author 小原立暉
//
//===================================
#ifndef _BOSS_CHARGE_STATE_H_
#define _BOSS_CHARGE_STATE_H_

//***********************************
// インクルードファイル
//***********************************
#include "boss_state.h"

//-----------------------------------
// クラス定義(チャージ状態クラス)
//-----------------------------------
class CBossChargeState : public CBossState
{
public:

	CBossChargeState(CBossState* next, const int nCount);		// オーバーロードコンストラクタ
	~CBossChargeState();	// デストラクタ

	void Delete(void) override;	// 完全消去処理

	void Process(CBoss* pBoss) override;		// 状態処理
	void SetData(CBoss* pBoss) override;		// 情報の設定処理

private:

	// メンバ変数
	CBossState* m_pNext;		// 次の状態
	int m_nCount;				// 経過カウント
};

#endif