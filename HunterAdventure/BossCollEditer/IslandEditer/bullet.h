//===================================
//
// 弾ヘッダー[bullet.h]
// Author 小原立暉
//
//===================================
#ifndef _BULLET_H_
#define _BULLET_H_

//***********************************
// インクルードファイル
//***********************************
#include "objectbillboard.h"
#include "list_manager.h"

//-----------------------------------
// クラス定義(CBullet)
//-----------------------------------
class CBullet : public CBillboard
{
public:

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_HANDGUN = 0,	// 二丁拳銃
		TYPE_SHOTGUN,		// 散弾
		TYPE_MAX			// この列挙型の総数
	};

	CBullet();				// コンストラクタ
	~CBullet();				// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type);				// 情報の設定処理

	// セット・ゲット関係
	int GetDamage(void) const;		// ダメージの取得処理
	float GetKnockBack(void) const;	// ノックバック値の取得処理

	// 静的メンバ関数
	static CBullet* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type);		// 生成処理

	static CListManager<CBullet*> GetList(void);			// リストの取得処理

private:

	// メンバ変数
	D3DXVECTOR3 m_move;		// 移動量
	TYPE m_type;			// 種類
	int m_nLife;			// 寿命
	int m_nDamage;			// ダメージ
	float m_fKnockBack;		// ノックバック値

	// 静的メンバ変数
	static CListManager<CBullet*> m_list;		// リスト
};

#endif