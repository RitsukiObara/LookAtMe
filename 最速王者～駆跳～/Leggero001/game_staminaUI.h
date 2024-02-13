//============================================
//
// ゲームのスタミナUIヘッダー[game_staminaUI.h]
// Author：小原立暉
//
//============================================
#ifndef _GAME_STAMINAUI_H_					// このマクロ定義がされていなかったら
#define _GAME_STAMINAUI_H_					// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "staminaUI.h"

//--------------------------------------------
// クラス定義(ゲームのスタミナUI)
//--------------------------------------------
class CGameStaminaUI : public CStaminaUI
{
public:				// 誰でもアクセスできる

	CGameStaminaUI();		// コンストラクタ
	~CGameStaminaUI();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(void);			// 情報の設定処理

private:			// 自分だけアクセスできる

};

#endif