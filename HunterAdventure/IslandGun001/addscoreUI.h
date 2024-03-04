//===================================
//
// スコア追加UIヘッダー[addscoreUI.h]
// Author 小原立暉
//
//===================================
#ifndef _ADDSCORE_UI_H_
#define _ADDSCORE_UI_H_

//***********************************
// インクルードファイル
//***********************************
#include "objectbillboard.h"

//-----------------------------------
// クラス定義(CAddScoreUI)
//-----------------------------------
class CAddScoreUI : public CBillboard
{
public:

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_COIN = 0,		// コイン
		TYPE_TORDLE,		// タードル
		TYPE_IWAKARI,		// イワカリ
		TYPE_MAX			// この列挙型の総数
	};

	CAddScoreUI();			// コンストラクタ
	~CAddScoreUI();			// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const TYPE type);					// 情報の設定処理

	// 静的メンバ関数
	static CAddScoreUI* Create(const D3DXVECTOR3& pos, const TYPE type);	// 生成処理

private:

	// メンバ変数
	TYPE m_type;			// 種類
	int m_nLife;			// 寿命
	float m_fAlpha;			// 透明度
};

#endif