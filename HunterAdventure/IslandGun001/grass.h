//===================================
//
// 草ヘッダー[grass.h]
// Author 小原立暉
//
//===================================
#ifndef _GRASS_H_
#define _GRASS_H_

//***********************************
// インクルードファイル
//***********************************
#include "object3D.h"
#include "list_manager.h"

//-----------------------------------
// クラス定義(CGrass)
//-----------------------------------
class CGrass : public CObject3D
{
public:

	CGrass();			// コンストラクタ
	~CGrass();			// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size);				// 情報の設定処理

	// 静的メンバ関数
	static CGrass* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size);		// 生成処理

	static CListManager<CGrass*> GetList(void);			// リストの取得処理

private:

	// メンバ関数
	//void SetVertexPos(void);		// 頂点座標の設定処理

	// メンバ変数
	D3DXVECTOR3 m_LeftUp;			// 目的の左上の座標
	D3DXVECTOR3 m_RightUp;			// 目的の右上の座標
	D3DXVECTOR3 m_LeftUpDest;		// 目的の左上の座標
	D3DXVECTOR3 m_RightUpDest;		// 目的の右上の座標

	// 静的メンバ変数
	static CListManager<CGrass*> m_list;		// リスト
};

#endif