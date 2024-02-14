//===================================
//
// ボスの状態ヘッダー[boss_state.h]
// Author 小原立暉
//
//===================================
#ifndef _BOSS_STATE_H_
#define _BOSS_STATE_H_
//***********************************
// インクルードファイル
//***********************************
#include "main.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CBoss;		// ボス

// クラス定義(ボスの状態の基本クラス)
class CBossState
{
public:

	CBossState();			// コンストラクタ
	virtual ~CBossState();	// デストラクタ

	void Uninit();			// 終了処理

	virtual void Process(CBoss* pBoss) = 0;	// 状態処理
	virtual void SetData(CBoss* pBoss) = 0;	// 情報の設定処理
};

#endif