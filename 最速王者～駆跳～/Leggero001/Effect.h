//===================================
//
// エフェクトヘッダー[Effect.h]
// Author 小原立暉
//
//===================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//***********************************
// インクルードファイル
//***********************************
#include "objectbillboard.h"

//-----------------------------------
// クラス定義(CEffect)
//-----------------------------------
class CEffect : public CBillboard
{
public:

	// 種類
	typedef enum
	{
		TYPE_NONE = 0,			// 無し
		TYPE_FIRE,				// 炎系
		TYPE_ITEMGET,			// アイテム獲得
		TYPE_BIGJUMP,			// 超ジャンプ
		TYPE_SPEEDSTAR,			// 速度の星
		TYPE_CATCH,				// 崖キャッチ
		TYPE_RUNDUST,			// 走った後の埃
		TYPE_BIGJUMPCHARGE,		// 超ジャンプのチャージ
		TYPE_LANDDUST,			// 着地煙
		TYPE_MAX				// この列挙型の総数
	}TYPE;

	CEffect();			// コンストラクタ
	~CEffect();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);			// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const int nLife, const float fRadius, const TYPE type);				// 情報の設定処理

	// 種類ごとの処理
	void UpdateFire(void);			// 炎系の処理
	void UpdateBJCharge(void);		// 超ジャンプのチャージの処理

	// 静的メンバ関数
	static CEffect* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const int nLife, const float fRadius, const TYPE type);		// 生成処理

private:

	// メンバ変数
	D3DXVECTOR3 m_move;		// 移動量
	D3DXCOLOR m_col;		// 色
	TYPE m_type;			// 種類
	int m_nLife;			// 寿命
	float m_fSub;			// 透明になる間隔
	float m_fContra;		// 半径の縮む間隔
	bool m_bAdd;			// 加算合成状況
};

#endif