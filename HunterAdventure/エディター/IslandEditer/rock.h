//===================================
//
// 岩ヘッダー[rock.h]
// Author 小原立暉
//
//===================================
#ifndef _ROCK_H_
#define _ROCK_H_

//***********************************
// インクルードファイル
//***********************************
#include "model.h"
#include "list_manager.h"

//-----------------------------------
// クラス定義(岩)
//-----------------------------------
class CRock : public CModel
{
public:			// 誰でもアクセスできる

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_BROWN = 0,	// 茶色岩
		TYPE_GRAY,		// 灰色岩
		TYPE_BREAK,		// 壊れる岩
		TYPE_MAX		// この列挙型の総数
	};

	CRock();			// コンストラクタ
	~CRock();			// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const TYPE type);			// 情報の設定処理
	void Break(void);				// 破壊処理

	// セット・ゲット関係
	TYPE GetType(void) const;			// 種類の取得処理
	float GetRadius(void) const;		// 半径の取得処理
	float GetTopHeight(void) const;		// 上の高さの取得処理
	float GetBottomHeight(void) const;	// 下の高さの取得処理

	// 静的メンバ関数
	static CRock* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const TYPE type);	// 生成処理

	static CListManager<CRock*> GetList(void);			// リストの取得処理

private:		// 自分だけアクセスできる
	
	// メンバ変数
	TYPE m_type;		// 種類
	float m_fRadius;	// 半径
	float m_fTop;		// 上の高さ
	float m_fBottom;	// 下の高さ

	// 静的メンバ変数
	static CListManager<CRock*> m_list;		// リスト
};

#endif