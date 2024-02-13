//============================================
//
// リザルトヘッダー[result.h]
// Author：小原立暉
//
//============================================
#ifndef _RESULT_H_			//このマクロ定義がされていなかったら
#define _RESULT_H_			//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "scene.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CResultTime;				// リザルトタイマー

//--------------------------------------------
// クラス(ロゴ画面クラス)
//--------------------------------------------
class CResult : public CScene
{
public:				// 誰でもアクセスできる

	// 列挙型定義(状態)
	enum STATE
	{
		STATE_TIMEDISP = 0,		// タイム表示状態
		STATE_SCOREDISP,		// スコア表示状態
		STATE_STOP,				// 停止状態
		STATE_MAX				// この列挙型の総数
	};

	// 列挙型定義(種類)
	enum RANK
	{
		RANK_SS = 0,			// SSランク
		RANK_S,					// Sランク
		RANK_A,					// Aランク
		RANK_B,					// Bランク
		RANK_C,					// Cランク
		RANK_MAX				// この列挙型の総数
	};

	CResult();		// コンストラクタ
	~CResult();		// デストラクタ

	// メンバ関数
	HRESULT Init(void);					// 初期化処理
	void Uninit(void);					// 終了処理
	void Update(void);					// 更新処理
	void Draw(void);					// 描画処理

	// セット・ゲット関係
	static void SetState(const STATE state);		// 状態の設定処理
	static STATE GetState(void);					// 状態の取得処理

private:					// 自分だけアクセスできる

	// メンバ変数
	static STATE m_state;			// 状態
};

#endif