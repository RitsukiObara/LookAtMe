//============================================
//
// チュートリアルのスタミナUIヘッダー[tutorial_staminaUI.h]
// Author：小原立暉
//
//============================================
#ifndef _TUTORIAL_STAMINAUI_H_					// このマクロ定義がされていなかったら
#define _TUTORIAL_STAMINAUI_H_					// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "staminaUI.h"

//--------------------------------------------
// クラス定義(ゲームのスタミナUI)
//--------------------------------------------
class CTutorialStaminaUI : public CStaminaUI
{
public:				// 誰でもアクセスできる

	CTutorialStaminaUI();		// コンストラクタ
	~CTutorialStaminaUI();		// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(void);			// 情報の設定処理

private:			// 自分だけアクセスできる

	// メンバ関数
	void PosCorrect(void);		// 位置の補正処理

	// メンバ変数
	float m_posDest2D[TYPE_MAX];		// 目的の位置
	float m_posDestAnim;				// 目的の位置
};

#endif