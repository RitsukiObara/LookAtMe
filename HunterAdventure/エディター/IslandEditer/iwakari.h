//===================================
//
// イワカリヘッダー[iwakari.h]
// Author 小原立暉
//
//===================================
#ifndef _IWAKARI_H_
#define _IWAKARI_H_

//***********************************
// インクルードファイル
//***********************************
#include "enemy.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CIwakariShell;		// イワカリの殻

//-----------------------------------
// クラス定義(CIwakari)
//-----------------------------------
class CIwakari : public CEnemy
{
public:					// 誰でもアクセスできる

	// 列挙型定義(状態)
	enum ACTION
	{
		ACTION_NONE = 0,	// 通常状態
		ACTION_MOVE,		// 移動状態
		ACTION_MAX			// この列挙型の総数
	};

	CIwakari();			// コンストラクタ
	~CIwakari();		// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type) override;		// 情報の設定処理
	void Hit(const int nDamage, const float fKnockback) override;		// ヒット処理

private:				// 自分だけアクセスできる

	// メンバ関数
	void Chase(void);			// 追跡処理
	void Move(void);			// 移動処理
	void RotMove(void);			// 向きの移動処理

	// メンバ変数
	CIwakariShell* m_pShell;	// 殻の情報
	D3DXVECTOR3 m_rotDest;		// 目標の向き
	ACTION m_action;			// 状態
	float m_fMoveX;				// 移動量(X軸)
	float m_fMoveZ;				// 移動量(Z軸)
};

#endif