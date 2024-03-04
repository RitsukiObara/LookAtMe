//===================================
//
// タードルヘッダー[tordle.h]
// Author 小原立暉
//
//===================================
#ifndef _TORDLE_H_
#define _TORDLE_H_

//***********************************
// インクルードファイル
//***********************************
#include "enemy.h"

//-----------------------------------
// クラス定義(CTordle)
//-----------------------------------
class CTordle : public CEnemy
{
public:					// 誰でもアクセスできる

	// 列挙型定義(状態)
	enum ACTION
	{
		ACTION_WALK = 0,	// 徘徊状態
		ACTION_CHASE,		// 追跡状態
		ACTION_MAX			// この列挙型の総数
	};

	// 構造体定義(徘徊経路関係)
	struct SWalking
	{
		D3DXVECTOR3* pPosDest;	// 目的の位置
		int nNumPos;			// 位置の総数
		int nNowPosIdx;			// 現在の位置の番号
	};

	CTordle();			// コンストラクタ
	~CTordle();			// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type) override;		// 情報の設定処理
	void Hit(const int nDamage, const float fKnockback) override;		// ヒット処理

private:				// 自分だけアクセスできる

	// メンバ関数
	void SetRoute(void);		// 経路の設定処理
	void Walking(void);			// 徘徊処理
	void Arrival(void);			// 到着処理
	void Chase(void);			// 追跡処理
	void Move(void);			// 移動処理
	void RotMove(void);			// 向きの移動処理
	void StateJudge(void);		// 状態判断処理

	// メンバ変数
	ACTION m_action;			// 行動
	float m_fMoveX;				// 移動量(X軸)
	float m_fMoveZ;				// 移動量(Z軸)
	D3DXVECTOR3 m_rotDest;		// 目標の向き
	SWalking m_walking;			// 徘徊経路関係の情報
};

#endif