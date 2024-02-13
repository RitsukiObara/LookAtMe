//===================================
//
// ガレジャンテヘッダー[garrejante.h]
// Author 小原立暉
//
//===================================
#ifndef _GARREJANTE_H_
#define _GARREJANTE_H_

//***********************************
// インクルードファイル
//***********************************
#include "enemy.h"

//-----------------------------------
// クラス定義(ガレジャンテ)
//-----------------------------------
class CGarrejante : public CEnemy
{
public:			// 誰でもアクセスできる

	// 列挙型定義(状態)
	enum STATE
	{
		STATE_NONE = 0,		// 通常状態
		STATE_DEATH,		// 死亡状態
		STATE_SMASH,		// 吹き飛び状態
		STATE_MAX			// この列挙型の総数
	};

	CGarrejante();			// コンストラクタ
	~CGarrejante();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void Hit(void);			// ヒット処理
	void SmashHit(void);	// 吹き飛びヒット処理
	void StunHit(void);		// 気絶のヒット処理
	void SetData(const D3DXVECTOR3& pos, const TYPE type);				// 情報の設定処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void Smash(void);				// 吹き飛び状態処理
	void TableLand(void);			// 台の着地判定処理

	// メンバ変数
	STATE m_state;			// 状態
	int m_nStateCount;		// 状態カウント
};

#endif