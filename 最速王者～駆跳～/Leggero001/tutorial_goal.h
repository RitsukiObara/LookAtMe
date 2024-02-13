//===================================
//
// チュートリアルゴールヘッダー[tutorial_goal.h]
// Author：小原立暉
//
//===================================
#ifndef _TUTORIAL_GOAL_H_			// このマクロ定義がされていなかったら
#define _TUTORIAL_GOAL_H_			// 2重インクルード防止のマクロを定義する

//***********************************
// インクルードファイル
//***********************************
#include "goal.h"

//-----------------------------------
// クラス定義(ゴール)
//-----------------------------------
class CTutorialGoal : public CGoal
{
public:		// 誰でもアクセスできる

	CTutorialGoal();			// コンストラクタ
	~CTutorialGoal();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

private:	// 自分だけアクセスできる

};

#endif