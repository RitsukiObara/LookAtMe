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

	//-----------------------------------
	// 種類
	//-----------------------------------
	typedef enum
	{
		TYPE_DIRT = 0,		// 地面ブロック
		TYPE_ROCK,			// 岩ブロック
		TYPE_ICE,			// 氷ブロック
		TYPE_MAGMA,			// マグマブロック
		TYPE_MAX			// この列挙型の総数
	}TYPE;

	CBlock();															// コンストラクタ
	CBlock(CObject::TYPE type, PRIORITY priority = PRIORITY_BLOCK);		// オーバーロードコンストラクタ
	virtual ~CBlock();													// デストラクタ
	void Box(void);														// コンストラクタの箱

	// メンバ関数
	virtual HRESULT Init(void);		// 初期化処理
	virtual void Uninit(void);		// 終了処理
	virtual void Update(void);		// 更新処理
	virtual void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const D3DXMATERIAL* pMat, const TYPE type, const bool bCliffR, const bool bCliffL);		// 情報の設定処理

	// セット・ゲット関係
	void SetType(const TYPE type);			// 種類の設定処理
	TYPE GetType(void) const;				// 種類の取得処理

	void SetCliffRight(const bool bCliff);	// 右崖の設定処理
	bool GetCliffRight(void) const;			// 右崖の取得処理
	void SetCliffLeft(const bool bCliff);	// 左崖の設定処理
	bool GetCliffLeft(void) const;			// 左崖の取得処理

	D3DXMATERIAL GetMaterial(int nIdx)const;		// マテリアルの取得処理

// デバッグ処理
#ifdef _DEBUG

	bool GetDelete(void) const;			// 消去状況の取得処理
	void SetDelete(const bool bDelete);	// 消去状況の設定処理

#endif 

	// 静的メンバ関数
	static CBlock* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const D3DXMATERIAL* pMat, const TYPE type, const bool bCliffR, const bool bCliffL);		// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	TYPE m_type;						// 種類
	D3DXMATERIAL m_aMat[MAX_MATERIAL];	// マトリックスへのポインタ
	bool m_bCliffRight;					// 右側が崖つかまり出来るか
	bool m_bCliffLeft;					// 左側が崖つかまり出来るか

// デバッグ処理
#ifdef _DEBUG

	bool m_bDelete;			// 消去状況
#endif 

};

#endif