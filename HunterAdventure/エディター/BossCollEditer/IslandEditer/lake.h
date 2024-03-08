//===================================
//
// 水場ヘッダー[lake.h]
// Author 小原立暉
//
//===================================
#ifndef _LAKE_H_
#define _LAKE_H_

//***********************************
// インクルードファイル
//***********************************
#include "object3D_multitex.h"
#include "list_manager.h"

//-----------------------------------
// クラス定義(水場)
//-----------------------------------
class CLake : public CObject3DMultiTex
{
public:			// 誰でもアクセスできる

	// 列挙型定義(テクスチャの種類)
	enum NUM
	{
		NUM_RIPPLE = 0,	// 波紋のテクスチャ
		NUM_OCEAN,		// 海のテクスチャ
		NUM_MAX			// この列挙型の総数
	};

	CLake();			// コンストラクタ
	~CLake();			// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size);				// 情報の設定処理

	// 静的メンバ関数
	static CLake* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size);		// 生成処理

	static CListManager<CLake*> GetList(void);			// リストの取得処理

private:		// 自分だけアクセスできる
	
	// メンバ変数
	D3DXVECTOR2 m_aTexU[NUM_MAX];		// テクスチャ座標の開始位置(U値)

	// 静的メンバ変数
	static CListManager<CLake*> m_list;		// リスト
};

#endif