//============================================
//
// チュートリアル画面ヘッダー[tutorial.h]
// Author：小原立暉
//
//============================================
#ifndef _TUTORIAL_H_			//このマクロ定義がされていなかったら
#define _TUTORIAL_H_			//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "scene.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CBalloonUI;		// ふきだしのUI

//--------------------------------------------
// クラス(チュートリアルクラス)
//--------------------------------------------
class CTutorial : public CScene
{
public:				// 誰でもアクセスできる

	// 列挙型定義(状態)
	typedef enum
	{
		STATE_NONE = 0,		// 通常状態
		STATE_SPEED,		// 速度UI表示
		STATE_BOOST,		// ブースト説明状態
		STATE_POLE,			// 街灯ジャンプ
		STATE_STAMINA,		// スタミナ
		STATE_END,			// 終了状態
		STATE_SKIP,			// スキップ状態
		STATE_MAX			// この列挙型の総数
	}STATE;

	CTutorial();		// コンストラクタ
	~CTutorial();		// デストラクタ

	// メンバ関数
	HRESULT Init(void);					// 初期化処理
	void Uninit(void);					// 終了処理
	void Update(void);					// 更新処理
	void Draw(void);					// 描画処理

	void SetData(const MODE mode);		// 情報の設定処理

	// セット・ゲット関数
	static void SetState(const STATE state);	// 状態の設定処理
	static STATE GetState(void);				// 状態の取得処理

	static CBalloonUI* GetBalloonUI(void);		// ふきだし2Dの取得処理

private:			// 自分だけアクセスできる

	// メンバ関数
	void SkipCreate(void);				// スキップ円の出現処理

	// メンバ変数
	int m_nEndCount;					// 終了までのカウント

	// 静的メンバ変数
	static STATE m_state;				// 状態
	static CBalloonUI* m_pBalloonUI;	// ふきだし2Dの情報
};

#endif