//===================================
//
// モーションブラー(色)ヘッダー[blur_col.h]
// Author 小原立暉
//
//===================================
#ifndef _BLUR_COL_H_
#define _BLUR_COL_H_

//***********************************
// インクルードファイル
//***********************************
#include "blur.h"

//-----------------------------------
// クラス定義(CBlurCol)
//-----------------------------------
class CBlurCol : public CBlur
{
public:					// 誰でもアクセスできる

	CBlurCol(PRIORITY priority = PRIORITY_EFFECT);		// オーバーロードコンストラクタ
	~CBlurCol();			// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const D3DXCOLOR& col, const CXFile::SXFile& filedata, const int nLife);		// 情報の設定処理(色有り)

	// 静的メンバ関数
	static CBlurCol* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const D3DXCOLOR& col, const CXFile::SXFile& filedata, const int nLife, const PRIORITY priority);	// 生成処理(色有り)

private:				// 自分だけアクセスできる

	// メンバ変数
	D3DXCOLOR m_col;		// 色
};

#endif