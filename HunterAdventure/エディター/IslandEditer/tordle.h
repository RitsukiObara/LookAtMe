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
	void Chase(void);			// 追跡処理
	void Move(void);			// 移動処理
	void RotMove(void);			// 向きの移動処理

	// メンバ変数
	float m_fMoveX;				// 移動量(X軸)
	float m_fMoveZ;				// 移動量(Z軸)
	D3DXVECTOR3 m_rotDest;		// 目標の向き
};

#endif