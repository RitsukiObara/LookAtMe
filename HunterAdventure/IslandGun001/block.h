//===================================
//
// ブロックヘッダー[block.h]
// Author 小原立暉
//
//===================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

//***********************************
// インクルードファイル
//***********************************
#include "model.h"
#include "list_manager.h"
#include "area.h"

//-----------------------------------
// クラス定義(ブロック)
//-----------------------------------
class CBlock : public CModel
{
public:			// 誰でもアクセスできる

	CBlock();			// コンストラクタ
	~CBlock();			// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& scale);				// 情報の設定処理

	// 静的メンバ関数
	static CBlock* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& scale);	// 生成処理

	// セット・ゲット関係
	D3DXVECTOR3 GetVtxMax(void) const;		// 頂点の最大値の取得処理
	D3DXVECTOR3 GetVtxMin(void) const;		// 頂点の最小値の取得処理

	static CListManager<CBlock*> GetList(const int nIdx);			// リストの取得処理

private:		// 自分だけアクセスできる
	
	// メンバ変数
	D3DXVECTOR3 m_vtxMax;		// 頂点の最大値
	D3DXVECTOR3 m_vtxMin;		// 頂点の最小値
	int m_nFieldIdx;			// 区分の番号

	// 静的メンバ変数
	static CListManager<CBlock*> m_aList[area::NUM_AREA];		// リスト
};

#endif