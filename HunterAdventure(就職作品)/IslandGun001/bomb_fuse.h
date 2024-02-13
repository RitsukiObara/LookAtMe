//===================================
//
// 爆弾の導火線ヘッダー[bomb_fuse.h]
// Author 小原立暉
//
//===================================
#ifndef _BOMB_FUSE_H_
#define _BOMB_FUSE_H_

//***********************************
// インクルードファイル
//***********************************
#include "model.h"

//-----------------------------------
// クラス定義(爆弾の導火線)
//-----------------------------------
class CBombFuse : public CModel
{
public:			// 誰でもアクセスできる

	CBombFuse();			// コンストラクタ
	~CBombFuse();			// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(D3DXMATRIX* pMtx);					// 情報の設定処理

	// 静的メンバ関数
	static CBombFuse* Create(D3DXMATRIX* pMtx);		// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	D3DXMATRIX* m_pMtxParent;		// 親のマトリックス
};

#endif