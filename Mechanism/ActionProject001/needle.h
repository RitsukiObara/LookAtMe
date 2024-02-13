//===================================
//
// 棘ヘッダー[needle.h]
// Author 小原立暉
//
//===================================
#ifndef _NEEDLE_H_
#define _NEEDLE_H_

//***********************************
// インクルードファイル
//***********************************
#include "model.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CNeedle;		// 棘

//-----------------------------------
// クラス定義(棘)
//-----------------------------------
class CNeedle : public CModel
{
public:			// 誰でもアクセスできる

	CNeedle();				// コンストラクタ
	~CNeedle();				// デストラクタ

	// リスト構造関係
	void SetPrev(CNeedle* pPrev);	// 前のポインタの設定処理
	void SetNext(CNeedle* pNext);	// 後のポインタの設定処理
	CNeedle* GetPrev(void) const;	// 前のポインタの設定処理
	CNeedle* GetNext(void) const;	// 次のポインタの設定処理

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3 rot);			// 情報の設定処理

	// 静的メンバ関数
	static CNeedle* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3 rot);	// 生成処理

private:		// 自分だけアクセスできる

	// リスト構造関係
	CNeedle* m_pPrev;	// 前へのポインタ
	CNeedle* m_pNext;	// 次へのポインタ
};

#endif