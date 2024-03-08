//===================================
//
// 画面エフェクトヘッダー[screen_effect.h]
// Author 小原立暉
//
//===================================
#ifndef _SCREEN_EFFECT_H_
#define _SCREEN_EFFECT_H_

//***********************************
// インクルードファイル
//***********************************
#include "object2D.h"

//-----------------------------------
// クラス定義(CScreenEffect)
//-----------------------------------
class CScreenEffect : public CObject2D
{
public:			// 誰でもアクセスできる

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_RED_AROUND = 0,	// 周辺赤
		TYPE_BLOOD,				// 血
		TYPE_CONCENTRATE,		// 集中線
		TYPE_ALL_RED,			// 全部分赤
		TYPE_BLACK,				// 黒
		TYPE_MAX				// この列挙型の総数
	};

	CScreenEffect();		// コンストラクタ
	~CScreenEffect();		// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const TYPE type);			// 情報の設定処理

	// セット・ゲット関数
	void SetAlpha(const float fAlpha);		// 透明度の設定処理
	float GetAlpha(void) const;				// 透明度の取得処理

	// 静的メンバ関数
	static CScreenEffect* Create(const TYPE type);		// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	TYPE m_type;		// 種類
	float m_fAlpha;		// 透明度
};

#endif