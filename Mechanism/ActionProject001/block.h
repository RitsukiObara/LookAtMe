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

//-----------------------------------
// クラス定義(ブロック)
//-----------------------------------
class CBlock : public CModel
{
public:			// 誰でもアクセスできる

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_IRON = 0,		// 鉄箱
		TYPE_WOOD,			// 木箱
		TYPE_MAX			// この列挙型の総数
	};

	// 列挙型定義(ドロップの種類)
	enum DROPTYPE
	{
		DROPTYPE_NONE = 0,	// ドロップ無し
		DROPTYPE_ITEM,		// アイテムドロップ
		DROPTYPE_AIRPLANE,	// 飛行機ドロップ
		DROPTYPE_MAX		// この列挙型の総数
	};

	CBlock();				// コンストラクタ
	~CBlock();				// デストラクタ

	// リスト構造関係
	void SetPrev(CBlock* pPrev);	// 前のポインタの設定処理
	void SetNext(CBlock* pNext);	// 後のポインタの設定処理
	CBlock* GetPrev(void) const;	// 前のポインタの設定処理
	CBlock* GetNext(void) const;	// 次のポインタの設定処理

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void Drop(void);		// ドロップ処理
	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const TYPE type, const DROPTYPE dropType);				// 情報の設定処理

	// セット・ゲット関数
	TYPE GetType(void) const;					// 種類の設定処理
	DROPTYPE GetDropType(void) const;			// ドロップの種類の設定処理
	void SetBreakCount(const int nCount);		// 破壊カウントの設定処理
	int GetBreakCount(void) const;				// 破壊カウントの取得処理
	void SetEnableBreak(const bool bBreak);		// 破壊状況の設定処理
	bool IsBreak(void);							// 破壊状況の取得処理

	// 静的メンバ関数
	static CBlock* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const TYPE type, const DROPTYPE dropType);		// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	TYPE m_type;			// 種類
	DROPTYPE m_dropType;	// ドロップの種類
	int m_nBreakCount;		// 破壊カウント
	bool m_bBreak;			// 破壊状況

	// リスト構造関係
	CBlock* m_pPrev;	// 前へのポインタ
	CBlock* m_pNext;	// 次へのポインタ
};

#endif