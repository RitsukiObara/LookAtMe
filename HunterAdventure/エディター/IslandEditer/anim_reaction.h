//============================================
//
// アニメリアクションヘッダー[anim_reaction.h]
// Author：小原立暉
//
//============================================
#ifndef _ANIM_REACTION_H_			//このマクロ定義がされていなかったら
#define _ANIM_REACTION_H_			//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "objectbillboard.h"

//--------------------------------------------
// クラス(アニメリアクションクラス)
//--------------------------------------------
class CAnimReaction : public CBillboardAnim
{
public:			// 誰でもアクセスできる

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_EXPLOSION = 0,		// 爆発
		TYPE_GUNEXPLOSION,		// 爆薬爆発
		TYPE_SPLASH,			// 飛沫
		TYPE_WATEREXPLOSION,	// 水爆発
		TYPE_MAX				// この列挙型の総数
	};

	CAnimReaction();		// コンストラクタ
	~CAnimReaction();		// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const D3DXCOLOR& col, const TYPE type, const int nCount, const int nLife);		// 情報の設定処理

	// 静的メンバ関数
	static CAnimReaction* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const D3DXCOLOR& col, const TYPE type, const int nCount, const int nLife);		// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	TYPE m_type;		// 種類
	int m_nLife;		// 寿命
};

#endif