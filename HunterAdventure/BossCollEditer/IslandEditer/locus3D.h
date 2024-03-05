//===================================
//
// 3D残像ヘッダー[locus3D.h]
// Author 小原立暉
//
//===================================
#ifndef _LOCUS3D_H_
#define _LOCUS3D_H_

//***********************************
// インクルードファイル
//***********************************
#include "objectbillboard.h"

//-----------------------------------
// クラス定義(CLocus3D)
//-----------------------------------
class CLocus3D : public CBillboard
{
public:			// 誰でもアクセスできる

	CLocus3D();				// コンストラクタ
	~CLocus3D();			// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const float fAlpha, const int nLife, const int nTexIdx, const bool bLookY, const bool bLighting);				// 情報の設定処理

	// 静的メンバ関数
	static CLocus3D* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const float fAlpha, const int nLife, const int nTexIdx, const bool bLookY, const bool bLighting);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	int m_nLife;			// 寿命
	float m_fSub;			// 透明度の減衰量
	float m_fAlpha;			// 透明度
	bool m_bLightOff;		// ライティング状況
};

#endif