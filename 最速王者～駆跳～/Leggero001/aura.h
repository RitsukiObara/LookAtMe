//===================================
//
// オーラヘッダー[aura.h]
// Author 小原立暉
//
//===================================
#ifndef _AURA_H_
#define _AURA_H_

//***********************************
// インクルードファイル
//***********************************
#include "objectbillboard.h"

//-----------------------------------
// クラス定義(CAura)
//-----------------------------------
class CAura : public CBillboard
{
public:			// 誰でもアクセスできる

	CAura();			// コンストラクタ
	~CAura();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const int nLife);		// 情報の設定処理

	// 静的メンバ関数
	static CAura* Create(const D3DXVECTOR3& pos, const int nLife);		// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	D3DXVECTOR3 m_move;		// 移動量
	int m_nLife;			// 寿命
	float m_fContra;		// 半径の縮む間隔
};

#endif