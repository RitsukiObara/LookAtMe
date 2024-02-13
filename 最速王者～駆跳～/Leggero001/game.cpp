//============================================
//
// ゲームのメイン処理[game.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "input.h"
#include "game.h"
#include "fade.h"
#include "file.h"
#include "editAll.h"
#include "renderer.h"

#include "player.h"
#include "score.h"
#include "game_time.h"
#include "Item.h"
#include "goal.h"
#include "speedUI.h"
#include "Objectmesh.h"
#include "pause.h"
#include "item_generator.h"
#include "enemy.h"
#include "debugproc.h"
#include "camera_range.h"
#include "announce.h"
#include "sound.h"

#include "countdown.h"
#include "skybox.h"
#include "arrowsign.h"

//--------------------------------------------
// 静的メンバ変数宣言
//--------------------------------------------
CScore* CGame::m_pScore = nullptr;							// スコアの情報
CGameTime* CGame::m_pTime = nullptr;						// タイムの情報
CPause* CGame::m_pPause = nullptr;							// ポーズの情報
CGame::STATE CGame::m_GameState = STATE_START;				// ゲームの進行状態
int CGame::m_nHitStopCount = 0;								// ヒットストップカウント
int CGame::m_nFinishCount = 0;								// 終了カウント
bool CGame::m_bStartSkip = false;							// スタート演出のスキップ

#ifdef _DEBUG		// デバッグ用

bool CGame::m_bEdit = false;								// エディットの情報
CEditAll* CGame::m_pEditAll = nullptr;						// エディットオールの情報

#endif

//=========================================
// コンストラクタ
//=========================================
CGame::CGame() : CScene(TYPE_NONE, PRIORITY_BG)
{
	// 全ての値をクリアする
	m_nHitStopCount = 0;		// ヒットストップカウント
	m_nFinishCount = 0;			// 終了カウント
	m_GameState = STATE_START;	// 状態
}

//=========================================
// デストラクタ
//=========================================
CGame::~CGame()
{

}

//=========================================
//初期化処理
//=========================================
HRESULT CGame::Init(void)
{
	// ステージの設定処理
	CManager::GetFile()->SetBlock();		// ブロックの設置処理
	CManager::GetFile()->SetObstacle();		// 障害物の設置処理
	CManager::GetFile()->SetItem();			// アイテムの設置処理
	CManager::GetFile()->SetCamera();		// カメラの設置処理

	// テキスト読み込み処理
	CMesh::TxtSet();

	// スカイボックスの設定処理
	CSkyBox::Create();

	//if (m_pField == NULL)
	//{ // フィールドへのポインタが NULL の場合

	//	// フィールドの設定処理
	//	m_pField = CField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1000.0f, 0.0f, 1000.0f));
	//}

	// シーンの初期化
	CScene::Init();

	// 情報の初期化
	m_nHitStopCount = 0;			// ヒットストップカウント
	m_nFinishCount = 0;				// 終了カウント

	// 矢印看板の生成処理
	CArrowSign::Create(D3DXVECTOR3(300.0f, 0.0f, 50.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));

	// 成功を返す
	return S_OK;
}

//=============================================
//終了処理
//=============================================
void CGame::Uninit(void)
{
	// ポインタを NULL にする
	m_pScore = nullptr;			// スコア
	m_pTime = nullptr;			// タイム
	m_pPause = nullptr;			// ポーズ

	// 情報を初期化する
	m_GameState = STATE_START;	// ゲームの進行状態

	// 終了カウントを初期化する
	m_nFinishCount = 0;

#ifdef _DEBUG

	if (m_pEditAll != nullptr)
	{ // エディットオールが NULL じゃない場合

		// メモリを開放する
		delete m_pEditAll;
		m_pEditAll = nullptr;
	}

#endif

	// 終了処理
	CScene::Uninit();
}

//======================================
//更新処理
//======================================
void CGame::Update(void)
{
	// ヒットストップ処理
	HitStop();

	// ポーズ処理
	Pause();

#ifdef _DEBUG		// デバッグ用

	// エディットの切り替え処理
	EditChange();

	// ステージのセーブ処理
	StageSave();

	// エディットのアップデート処理
	EditUpdate();

	if (CManager::GetInputKeyboard() != nullptr)
	{ // キーボードが NULL じゃない場合

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_0) == true)
		{ // 0キーを押した場合

			if (CManager::GetCamera() != nullptr)
			{ // カメラが NULL じゃない場合

				// カメラの操作状況の切り替え処理
				CManager::GetCamera()->ChangeControl();
			}
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RSHIFT) == true)
		{ // 右SHIFTキーを押した場合

			if (m_pPause != nullptr)
			{ // ポーズのポインタが NULL の場合

				// 描画状況の切り替え処理
				m_pPause->ChangeDisp();
			}
		}
	}

#else

	// ゲーム内の更新処理
	GameUpdate();
#endif

	// 遷移処理
	Transition();

	CManager::GetDebugProc()->Print("状態：%d", m_GameState);
}

//======================================
//描画処理
//======================================
void CGame::Draw(void)
{

}

//======================================
// 情報の設定処理
//======================================
void CGame::SetData(const MODE mode)
{
	// 情報の設定処理
	CScene::SetData(mode);

	if (m_pScore == nullptr)
	{ // スコアへのポインタが NULL の場合

		// スコアのメモリを確保する
		m_pScore = CScore::Create(CManager::GetFile()->GetGameInfo().ScorePos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), CManager::GetFile()->GetGameInfo().ScoreSize);
	}

	if (m_pTime == nullptr)
	{ // タイムUIへのポインタが NULL の場合

		// 時間の生成処理
		m_pTime = CGameTime::Create(CManager::GetFile()->GetGameInfo().TimePos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), CManager::GetFile()->GetGameInfo().TimeSize);
	}

	// ゴールの設定
	CGoal::Create(CManager::GetFile()->GetGameInfo().GoalPos, MODE_GAME);

	// 残り距離の告知の設定
	CAnnounce::Create();

	if (m_pPause == nullptr)
	{ // ポーズへのポインタが NULL の場合

		// ポーズの生成処理
		m_pPause = CPause::Create();
	}

	if (m_bStartSkip == false)
	{ // スタート演出をスキップする場合

		// スタート状態にする
		m_GameState = STATE_START;
	}
	else
	{ // スキップしない場合

		// カウントダウン状態にする
		m_GameState = STATE_COUNTDOWN;
	}

	// 情報の初期化
	m_nHitStopCount = 0;			// ヒットストップカウント
	m_nFinishCount = 0;				// 終了カウント

	if (m_GameState == STATE_START)
	{ // スタート状態の場合

		if (CManager::GetCamera() != nullptr)
		{ // カメラの情報が NULL じゃない場合

			// ステージ投影カメラに設定する
			CManager::GetCamera()->SetType(CCamera::TYPE_STARTSTAGE);
		}

		// プレイヤーの位置を設定する
		GetPlayer()->SetPos(CManager::GetFile()->GetGameInfo().PlayerPos);
	}
	else
	{ // 上記以外

		if (CManager::GetCamera() != nullptr)
		{ // カメラの情報が NULL じゃない場合

			// 通常カメラに設定する
			CManager::GetCamera()->SetType(CCamera::TYPE_NONE);
		}

		// プレイヤーの位置を設定する
		GetPlayer()->SetPos(CManager::GetFile()->GetGameInfo().StartPos);

		// カウントダウンの生成
		CCountdown::Create();
	}

	// 演出スキップを ON にする
	m_bStartSkip = true;
}

//======================================
// ゲーム内の更新処理
//======================================
void CGame::GameUpdate(void)
{
	if (CManager::GetRenderer() != nullptr)
	{ // レンダラーが NULL じゃない場合

		if (CGame::GetState() != CGame::STATE_HITSTOP)
		{ // ヒットストップ状態以外の場合

			// レンダラーの更新
			CManager::GetRenderer()->Update();
		}
	}
}

//======================================
// ヒットストップ処理
//======================================
void CGame::HitStop(void)
{
	if (m_GameState == STATE_HITSTOP)
	{ // ゲームの進行状態がヒットストップの場合

		// ヒットストップカウントを減算する
		m_nHitStopCount--;

		if (m_nHitStopCount <= 0)
		{ // ヒットストップカウントが 0 以下になった場合

			// ゲームの進行状態を設定する
			m_GameState = STATE_PLAY;
		}
	}
}

//======================================
// ポーズ処理
//======================================
void CGame::Pause(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_P) == true ||
		CManager::GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_BACK, 0) == true)
	{ // Pキーを押した場合

		if (CManager::GetFade()->GetFade() == CFade::FADE_NONE &&
			m_GameState != STATE_GOAL &&
			m_GameState != STATE_FINISH)
		{ // フェード無し状態かつ、プレイ中の場合

			if (m_pPause->GetPause() == false)
			{ // ポーズが false だった場合

				// ポーズ状況を true にする
				m_pPause->SetPause(true);
			}
			else
			{ // ポーズが true だった場合

				// ポーズ状況を false にする
				m_pPause->SetPause(false);
			}

			// 決定音を鳴らす
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_DECIDE);
		}
	}
}

//======================================
// 遷移処理
//======================================
void CGame::Transition(void)
{
	if (m_GameState == STATE_FINISH)
	{ // ゲームが終了状態の場合

		// 終了カウントを加算する
		m_nFinishCount++;

		if (m_nFinishCount % 80 == 0)
		{ // 終了カウントが一定数を超えた場合

			// リザルトに遷移する
			CManager::GetFade()->SetFade(CScene::MODE_RESULT);
		}
	}
}

//======================================
// スコアの取得処理
//======================================
CScore* CGame::GetScore(void)
{
	// スコアの情報を返す
	return m_pScore;
}

//======================================
// タイムの取得処理
//======================================
CGameTime* CGame::GetTime(void)
{
	// タイムの情報を返す
	return m_pTime;
}

//======================================
// ポーズの取得処理
//======================================
CPause* CGame::GetPause(void)
{
	// ポーズの情報を返す
	return m_pPause;
}

//======================================
// ゲームの進行状態の設定処理
//======================================
void CGame::SetState(const STATE state)
{
	// ゲームの進行状態を設定する
	m_GameState = state;
}

//======================================
// ゲームの進行状態の取得処理
//======================================
CGame::STATE CGame::GetState(void)
{
	// ゲームの進行状態を返す
	return m_GameState;
}

//======================================
// ゲームのヒットストップの設定処理
//======================================
void CGame::SetHitStop(const int nCount)
{
	// ゲームの進行状態を設定する
	m_GameState = STATE_HITSTOP;

	// ヒットストップカウントを設定する
	m_nHitStopCount = nCount;
}

//======================================
// スコアのNULL化処理
//======================================
void CGame::DeleteScore(void)
{
	// スコアのポインタを NULL にする
	m_pScore = nullptr;
}

//======================================
// タイムのNULL化処理
//======================================
void CGame::DeleteTime(void)
{
	// タイムのポインタを NULL にする
	m_pTime = nullptr;
}

//======================================
// ポーズのNULL化処理
//======================================
void CGame::DeletePause(void)
{
	// ポーズのポインタを NULL にする
	m_pPause = nullptr;
}

#ifdef _DEBUG		// デバッグ用

//======================================
// エディットの切り替え処理
//======================================
void CGame::EditChange(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F9) == true)
	{ // F9キーを押した場合

		// エディットモードを入れ替える
		m_bEdit = m_bEdit ? false : true;

		if (m_bEdit == true)
		{ // エディットモードを ON にした場合

			if (m_pEditAll == nullptr)
			{ // エディットオールが NULL の場合

				// エディットの生成処理
				m_pEditAll = m_pEditAll->CEditAll::Create();
			}
		}
		else
		{ // エディットモードを OFF にした場合

			if (m_pEditAll != nullptr)
			{ // ポインタが NULL じゃない場合

				// エディットの終了処理
				m_pEditAll->Uninit();

				// メモリを開放する
				delete m_pEditAll;
				m_pEditAll = nullptr;
			}
		}
	}
}

//======================================
// ステージセーブ処理
//======================================
void CGame::StageSave(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F7) == true)
	{ // 7キーを押した場合

		// ブロックのセーブ処理
		CManager::GetFile()->Save(CFile::TYPE_BLOCK);

		// 障害物のセーブ処理
		CManager::GetFile()->Save(CFile::TYPE_OBSTACLE);

		// アイテムのセーブ処理
		CManager::GetFile()->Save(CFile::TYPE_ITEM);

		// カメラ範囲のセーブ処理
		CManager::GetFile()->Save(CFile::TYPE_CAMERA);
	}
}

//======================================
// エディットのアップデート処理
//======================================
void CGame::EditUpdate(void)
{
	if (m_bEdit == true)
	{ // エディットモードの場合

		if (m_pEditAll != nullptr)
		{ // ポインタが NULL じゃない場合

			// 更新処理
			m_pEditAll->Update();

			for (int nCntPriority = 0; nCntPriority < CObject::PRIORITY_MAX; nCntPriority++)
			{
				// 死亡判定処理
				CObject::DeathDecision(nCntPriority);
			}
		}
	}
	else
	{ // 上記以外

		// ゲーム内の更新処理
		GameUpdate();
	}
}

//======================================
// エディットの取得処理
//======================================
bool CGame::GetEdit(void)
{
	// エディットモードの情報を返す
	return m_bEdit;
}

//======================================
// エディットオールの取得処理
//======================================
CEditAll* CGame::GetEditAll(void)
{
	// エディットオールの情報を返す
	return m_pEditAll;
}

#endif