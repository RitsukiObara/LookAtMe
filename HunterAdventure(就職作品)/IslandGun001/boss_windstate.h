//===================================
//
// ボスのかまいたち状態ヘッダー[boss_windstate.h]
// Author 小原立暉
//
//===================================
#ifndef _BOSS_WIND_STATE_H_
#define _BOSS_WIND_STATE_H_
//***********************************
// インクルードファイル
//***********************************
#include "boss_state.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CWindShot;			// 風攻撃

//-----------------------------------
// クラス定義(ボスのかまいたち状態クラス)
//-----------------------------------
class CBossWindState : public CBossState
{
public:

	// 行動状況
	enum ACTION
	{
		ACTION_CHARGE = 0,	// チャージ行動
		ACTION_WIND,		// 風発生行動
		ACTION_MAX			// この列挙型の総数
	};

	CBossWindState();		// コンストラクタ
	~CBossWindState();		// デストラクタ

	void Process(CBoss* pBoss) override;		// 状態処理
	void SetData(CBoss* pBoss) override;		// 情報の設定処理

private:

	// メンバ関数
	void Chase(CBoss* pBoss);			// 追跡処理

	// メンバ変数
	CWindShot* m_pWindShot;		// 風攻撃の情報
	ACTION m_action;			// 行動状況
	int m_nCount;				// 経過カウント
};

#endif