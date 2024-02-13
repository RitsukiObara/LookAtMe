//============================================
//
// チュートリアルの速度UIヘッダー[tutorial_speedUI.h]
// Author：小原立暉
//
//============================================
#ifndef _TUTORIALSPEEDUI_H_					// このマクロ定義がされていなかったら
#define _TUTORIALSPEEDUI_H_					// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "speedUI.h"

//--------------------------------------------
// クラス定義(速度UI)
//--------------------------------------------
class CTutorialSpeedUI : public CSpeedUI
{
public:				// 誰でもアクセスできる

	CTutorialSpeedUI();		// コンストラクタ
	~CTutorialSpeedUI();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(void);		// 情報の設定処理

private:			// 自分だけアクセスできる

	// メンバ関数
	void PosCorrect(void);	// 補正処理

	// メンバ変数
	float m_posDest[TYPE_MAX];		// 目的の位置
};

#endif