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
class CScore;			// スコア
class CEditAll;			// エディットオール
class CGameTime;		// ゲームタイム
class CPause;			// ポーズ

//--------------------------------------------
// クラス(ゲームクラス)
//--------------------------------------------
class CGame : public CScene
{
public:						// 誰でもアクセスできる

	// ゲームの進行状態
	typedef enum
	{
		STATE_START = 0,	// 開始状態
		STATE_COUNTDOWN,	// カウントダウン
		STATE_PLAY,			// プレイ状態
		STATE_GOAL,			// ゴール状態
		STATE_FINISH,		// 終了状態
		STATE_HITSTOP,		// ヒットストップ状態
		STATE_MAX			// この列挙型の総数
	}STATE;

	CGame();				// コンストラクタ
	~CGame();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const MODE mode);		// 情報の設定処理

	// 静的メンバ関数(セット・ゲット)
	static CScore* GetScore(void);					// スコアの取得処理
	static CGameTime* GetTime(void);				// ゲームタイムの取得処理
	static CPause* GetPause(void);					// ポーズの取得処理

	static void SetState(const STATE state);		// ゲームの進行状態の設定処理
	static STATE GetState(void);					// ゲームの進行状態の取得処理

	static void SetHitStop(const int nCount);		// ゲームのヒットストップの設定処理

	// NULL化処理
	static void DeleteScore(void);					// スコアのNULL化処理
	static void DeleteTime(void);					// タイムのNULL化処理
	static void DeletePause(void);					// ポーズのNULL化処理

#ifdef _DEBUG		// デバッグ時のみ

	static bool GetEdit(void);						// エディットモードの取得処理
	static CEditAll* GetEditAll(void);				// エディットオールの取得処理
#endif

private:					// 自分だけアクセスできる

	// メンバ関数
	void GameUpdate(void);		// ゲーム内の更新処理
	void HitStop(void);			// ヒットストップ処理
	void Pause(void);			// ポーズ処理
	void Transition(void);		// 遷移処理

	// 静的メンバ変数(最初から必要な物を配置しておく)
	static CScore* m_pScore;					// スコアの情報
	static CGameTime* m_pTime;					// ゲームタイムの情報
	static CPause* m_pPause;					// ポーズの情報
	static STATE m_GameState;					// ゲームの進行状態
	static int m_nHitStopCount;					// ヒットストップカウント
	static int m_nFinishCount;					// 終了カウント
	static bool m_bStartSkip;					// スタート演出のスキップ

#ifdef _DEBUG		// デバッグ時のみ

	// メンバ関数
	void EditChange(void);						// エディットの切り替え処理
	void StageSave(void);						// ステージセーブ処理
	void EditUpdate(void);						// エディットのアップデート処理

	// 静的メンバ変数
	static bool m_bEdit;						// エディットの情報
	static CEditAll* m_pEditAll;				// エディットオールの情報

#endif
};

#endif