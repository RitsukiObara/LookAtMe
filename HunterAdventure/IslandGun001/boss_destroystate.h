//===================================
//
// ボスの撃破状態ヘッダー[boss_destroystate.h]
// Author 小原立暉
//
//===================================
#ifndef _BOSS_DESTROY_STATE_H_
#define _BOSS_DESTROY_STATE_H_

//***********************************
// インクルードファイル
//***********************************
#include "boss_state.h"

// クラス定義(ボスの撃破状態クラス)
class CBossDestroyState : public CBossState
{
public:

	CBossDestroyState();		// コンストラクタ
	~CBossDestroyState();		// デストラクタ

	void Process(CBoss* pBoss) override;		// 状態処理
	void SetData(CBoss* pBoss) override;		// 情報の設定処理

private:

	// メンバ関数
	void Move(CBoss* pBoss);	// 移動処理

	// メンバ変数
	int m_nCount;			// 経過カウント
	float m_fAddHeight;		// 高さの加算数
};

#endif