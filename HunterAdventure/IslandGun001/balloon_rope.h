//===================================
//
// 風船の紐ヘッダー[balloon_rope.h]
// Author 小原立暉
//
//===================================
#ifndef _BALLOON_ROPE_H_
#define _BALLOON_ROPE_H_

//***********************************
// インクルードファイル
//***********************************
#include "model.h"

//-----------------------------------
// クラス定義(風船の紐)
//-----------------------------------
class CBalloonRope : public CModel
{
public:			// 誰でもアクセスできる

	CBalloonRope();			// コンストラクタ
	~CBalloonRope();		// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(D3DXMATRIX* mtx);					// 情報の設定処理

	// 静的メンバ関数
	static CBalloonRope* Create(D3DXMATRIX* mtx);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	D3DXMATRIX* m_pMtxParent;		// 親のマトリックス
};

#endif