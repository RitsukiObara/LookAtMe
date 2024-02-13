//============================================
//
// ゲームヘッダー[game.h]
// Author：小原立暉
//
//============================================
#ifndef _GAME_H_			//このマクロ定義がされていなかったら
#define _GAME_H_			//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "scene.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CPause;			// ポーズ
class CScore;			// スコア
class CEdit;			// エディット

//--------------------------------------------
// クラス(ゲームクラス)
//--------------------------------------------
class CGame : public CScene
{
public:						// 誰でもアクセスできる

	// ゲームの進行状態
	enum STATE
	{
		STATE_START = 0,	// 開始状態
		STATE_PLAY,			// プレイ状態
		STATE_GOAL,			// ゴール状態
		STATE_LEAVE,		// 退場状態
		STATE_FINISH,		// 終了状態
		STATE_DEATH,		// 死亡状態
		STATE_MAX			// この列挙型の総数
	};

	CGame();				// コンストラクタ
	~CGame();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const MODE mode);		// 情報の設定処理

	// 静的メンバ関数(セット・ゲット)
	static CPause* GetPause(void);					// ポーズの取得処理

	static void SetState(const STATE state);		// ゲームの進行状態の設定処理
	static STATE GetState(void);					// ゲームの進行状態の取得処理

	static void ResetScore(void);		// スコアのリセット処理
	static int GetScore(void);			// スコアの取得処理

	// NULL化処理
	static void DeletePause(void);		// ポーズのNULL化処理

// デバッグ版
#ifdef _DEBUG

	static CEdit* GetEdit(void);		// エディットの取得処理
	static bool IsEdit(void);			// エディット状況の取得処理
#endif

private:					// 自分だけアクセスできる

	// メンバ関数
	void Pause(void);			// ポーズ処理
	void Transition(void);		// 遷移処理
	void DeathTransition(void);	// 死亡時の遷移処理

	// 静的メンバ変数(最初から必要な物を配置しておく)
	static CPause* m_pPause;			// ポーズの情報
	static STATE m_GameState;			// ゲームの進行状態
	static int m_nFinishCount;			// 終了カウント
	static int m_nScore;				// スコア

// デバッグ版
#ifdef _DEBUG

	static CEdit* m_pEdit;				// エディットの情報
	static bool m_bEdit;				// エディット状態かどうか
#endif

};

#endif