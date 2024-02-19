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
class CPlayer;			// プレイヤー
class CGameScore;		// ゲームスコア

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
		STATE_BOSSMOVIE,	// ボス出現状態
		STATE_FINISH,		// 終了状態
		STATE_MAX			// この列挙型の総数
	};

	CGame();				// コンストラクタ
	~CGame();				// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(const MODE mode);		// 情報の設定処理

	// 静的メンバ関数(セット・ゲット)
	static void SetEnablePause(const bool bPause);	// ポーズ状況の設定処理
	static bool IsPause(void);						// ポーズ状況の取得処理

	static void SetState(const STATE state);		// ゲームの進行状態の設定処理
	static STATE GetState(void);					// ゲームの進行状態の取得処理

	static CPlayer* GetPlayer(void);			// プレイヤーの取得処理

	static CGameScore* GetGameScore(void);		// ゲームスコアの取得処理

	static int GetScore(void);					// 総合スコアの取得処理

	// NULL化処理
	static void DeletePause(void);		// ポーズのNULL化処理
	static void DeletePlayer(void);		// プレイヤーのNULL化処理
	static void DeleteGameScore(void);	// スコアのNULL化処理

private:					// 自分だけアクセスできる

	// メンバ関数
	void Pause(void);			// ポーズ処理
	void Transition(void);		// 遷移処理

	// メンバ変数
	int m_nFinishCount;			// 終了カウント

	// 静的メンバ変数
	static CPause* m_pPause;			// ポーズの情報
	static CPlayer* m_pPlayer;			// プレイヤーの情報
	static CGameScore* m_pGameScore;	// ゲームスコアの情報
	static STATE m_GameState;			// ゲームの進行状態
	static int m_nScore;				// スコア
	static bool m_bPause;				// ポーズ状況
};

#endif