//===================================
//
// 台ヘッダー[table.h]
// Author 小原立暉
//
//===================================
#ifndef _TABLE_H_
#define _TABLE_H_

//***********************************
// インクルードファイル
//***********************************
#include "model.h"

//-----------------------------------
// クラス定義(台)
//-----------------------------------
class CTable : public CModel
{
public:			// 誰でもアクセスできる

	CTable();					// コンストラクタ
	~CTable();					// デストラクタ

	// リスト構造関係
	void SetPrev(CTable* pPrev);	// 前のポインタの設定処理
	void SetNext(CTable* pNext);	// 後のポインタの設定処理
	CTable* GetPrev(void) const;	// 前のポインタの設定処理
	CTable* GetNext(void) const;	// 次のポインタの設定処理

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos);		// 情報の設定処理

	// 静的メンバ関数
	static CTable* Create(const D3DXVECTOR3& pos);		// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数

	// リスト構造関係
	CTable* m_pPrev;	// 前へのポインタ
	CTable* m_pNext;	// 次へのポインタ
};

#endif