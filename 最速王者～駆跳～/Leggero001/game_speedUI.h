//============================================
//
// ゲームの速度UIヘッダー[game_speedUI.h]
// Author：小原立暉
//
//============================================
#ifndef _GAMESPEEDUI_H_					// このマクロ定義がされていなかったら
#define _GAMESPEEDUI_H_					// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "speedUI.h"

//--------------------------------------------
// クラス定義(速度UI)
//--------------------------------------------
class CGameSpeedUI : public CSpeedUI
{
public:				// 誰でもアクセスできる

	CGameSpeedUI();			// コンストラクタ
	~CGameSpeedUI();		// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(void);		// 情報の設定処理

private:			// 自分だけアクセスできる

};

#endif