//===================================
//
// タイトルロゴヘッダー[title_logo.h]
// Author 小原立暉
//
//===================================
#ifndef _TITLE_LOGO_H_
#define _TITLE_LOGO_H_

//***********************************
// インクルードファイル
//***********************************
#include "object2D.h"

//-----------------------------------
// クラス定義(タイトルロゴ)
//-----------------------------------
class CTitleLogo : public CObject2D
{
public:			// 誰でもアクセスできる

	// 列挙型定義(STATE)
	enum STATE
	{
		STATE_MOVE = 0,		// 移動状態
		STATE_STOP,			// 停止状態
		STATE_MAX			// この列挙型の総数
	};

	CTitleLogo();			// コンストラクタ
	~CTitleLogo();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(void);		// 情報の設定処理

	// 静的メンバ関数
	static CTitleLogo* Get(void);		// 取得処理
	static CTitleLogo* Create(void);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	STATE m_state;		// 状態

	// 静的メンバ変数
	static CTitleLogo* m_pTitleLogo;	// タイトルロゴのポインタ
};

#endif