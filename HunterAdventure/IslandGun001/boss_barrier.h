//===================================
//
// ボスのバリアヘッダー[boss_barrier.h]
// Author 小原立暉
//
//===================================
#ifndef _BOSS_BARRIER_H_
#define _BOSS_BARRIER_H_

//***********************************
// インクルードファイル
//***********************************
#include "objectbillboard.h"

//-----------------------------------
// クラス定義(CBossBarrier)
//-----------------------------------
class CBossBarrier : public CBillboard
{
public:

	CBossBarrier();				// コンストラクタ
	~CBossBarrier();			// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos);				// 情報の設定処理

	// 静的メンバ関数
	static CBossBarrier* Create(const D3DXVECTOR3& pos);		// 生成処理

private:

	// メンバ変数
	int m_nLife;		// 寿命
};

#endif