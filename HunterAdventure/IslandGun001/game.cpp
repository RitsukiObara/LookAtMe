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
#include "renderer.h"
#include "texture.h"

#include "pause.h"
#include "debugproc.h"
#include "sound.h"

#include "objectElevation.h"
#include "skybox.h"
#include "motion.h"

#include "player.h"
#include "ocean.h"
#include "game_score.h"
#include "boss.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define TRANS_COUNT		(80)		// 遷移カウント

//--------------------------------------------
// 静的メンバ変数宣言
//--------------------------------------------
CPause* CGame::m_pPause = nullptr;							// ポーズの情報
CPlayer* CGame::m_pPlayer = nullptr;						// プレイヤーの情報
CGameScore* CGame::m_pGameScore = nullptr;					// ゲームスコアの情報
CGame::STATE CGame::m_GameState = CGame::STATE_START;		// ゲームの進行状態
int CGame::m_nScore = 0;									// スコア
bool CGame::m_bPause = false;								// ポーズ状況

//=========================================
// コンストラクタ
//=========================================
CGame::CGame()
{
	// 全ての値をクリアする
	m_nFinishCount = 0;			// 終了カウント

	m_pPause = nullptr;			// ポーズ
	m_pPlayer = nullptr;		// プレイヤー
	m_pGameScore = nullptr;		// スコア
	m_GameState = STATE_START;	// 状態
	m_nScore = 0;				// スコア
	m_bPause = false;			// ポーズ状況
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
	// スタートカメラで描画
	CManager::Get()->GetCamera()->SetType(CCamera::TYPE_START);

	// テキスト読み込み処理
	CElevation::TxtSet();

	// モーションの読み込み処理
	CMotion::Load(CMotion::STYLE_PLAYER);		// プレイヤー
	CMotion::Load(CMotion::STYLE_TORDLE);		// タードル
	CMotion::Load(CMotion::STYLE_IWAKARI);		// イワカリ
	CMotion::Load(CMotion::STYLE_BOSS);			// ボス

	// マップの生成
	CManager::Get()->GetFile()->SetEnemy();
	CManager::Get()->GetFile()->SetCoin();
	CManager::Get()->GetFile()->SetGoldBone();
	CManager::Get()->GetFile()->SetTree();
	CManager::Get()->GetFile()->SetRock();
	CManager::Get()->GetFile()->SetBlock ();
	CManager::Get()->GetFile()->SetBangFlower();
	CManager::Get()->GetFile()->SetWall();

	// スカイボックスの生成処理
	CSkyBox::Create();

	// シーンの初期化
	CScene::Init();

	// 海の生成
	COcean::Create();

	// プレイヤーの生成処理
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(-400.0f, 0.0f, 300.0f));

	// ゲームスコアの生成
	m_pGameScore = CGameScore::Create();

	// 情報の初期化
	m_nFinishCount = 0;			// 終了カウント
	m_GameState = STATE_START;	// 状態
	m_bPause = false;			// ポーズ状況

	// 成功を返す
	return S_OK;
}

//=============================================
//終了処理
//=============================================
void CGame::Uninit(void)
{
	if (m_pPause != nullptr)
	{ // ポーズが NULL じゃない場合

		// 終了処理
		m_pPause->Uninit();
	}

	// ポインタを NULL にする
	m_pPause = nullptr;			// ポーズ
	m_pPlayer = nullptr;		// プレイヤー
	m_pGameScore = nullptr;		// ゲームスコア

	// 情報を初期化する
	m_GameState = STATE_START;	// ゲームの進行状態
	m_bPause = false;			// ポーズ状況

	// 終了カウントを初期化する
	m_nFinishCount = 0;

	// 終了処理
	CScene::Uninit();
}

//======================================
//更新処理
//======================================
void CGame::Update(void)
{
	switch (m_GameState)
	{
	case CGame::STATE_START:
	case CGame::STATE_PLAY:
	case CGame::STATE_BOSSMOVIE:

		// ポーズ処理
		Pause();

		break;

	case CGame::STATE_FINISH:

		// 遷移処理
		Transition();

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	//if (CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
	//	CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_START, 0) == true ||
	//	CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true)
	//{ // ENTERキーを押した場合

	//	// チュートリアルに遷移する
	//	CManager::Get()->GetFade()->SetFade(CScene::MODE_RESULT);

	//	// この先の処理を行わない
	//	return;
	//}

	if (m_bPause == true)
	{ // ポーズ状況が true の場合

		// 更新処理
		m_pPause->Update();
	}
	else
	{ // 上記以外

		if (CManager::Get()->GetRenderer() != nullptr)
		{ // レンダラーが NULL じゃない場合

			// レンダラーの更新
			CManager::Get()->GetRenderer()->Update();
		}
	}

	CManager::Get()->GetDebugProc()->Print("状態：%d", m_GameState);
}

//======================================
//描画処理
//======================================
void CGame::Draw(void)
{
	if (m_pPause != nullptr &&
		m_bPause == true)
	{ // ポーズ中の場合

		// 描画処理
		m_pPause->Draw();
	}
}

//======================================
// 情報の設定処理
//======================================
void CGame::SetData(const MODE mode)
{
	// 情報の設定処理
	CScene::SetData(mode);

	// 全ての値を設定する
	m_GameState = STATE_START;	// スタート状態にする
	m_bPause = false;			// ポーズ状況

	// 情報の初期化
	m_nFinishCount = 0;				// 終了カウント
}

//======================================
// ポーズ処理
//======================================
void CGame::Pause(void)
{
	if (CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_P) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_BACK, 0) == true)
	{ // Pキーを押した場合

		if (CManager::Get()->GetFade()->GetFade() == CFade::FADE_NONE)
		{ // フェード無し状態かつ、終了以外の場合

			if (m_bPause == false)
			{ // ポーズが false だった場合

				if (m_pPause == nullptr)
				{ // ポーズへのポインタが NULL の場合

					// ポーズの生成処理
					m_pPause = CPause::Create();
				}
				else
				{ // 上記以外
					
					// 停止
					assert(false);
				}

				// ポーズ状況を true にする
				m_bPause = true;
			}
			else
			{ // ポーズが true だった場合

				if (m_pPause != nullptr)
				{ // ポーズへのポインタが NULL じゃない場合

					// 終了処理
					m_pPause->Uninit();
					m_pPause = nullptr;
				}

				// ポーズ状況を false にする
				m_bPause = false;
			}

			// 決定音を鳴らす
			CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_DECIDE);
		}
	}
}

//======================================
// 遷移処理
//======================================
void CGame::Transition(void)
{
	// 終了カウントを加算する
	m_nFinishCount++;

	if (m_nFinishCount % TRANS_COUNT == 0)
	{ // 終了カウントが一定数を超えた場合

		if (m_pGameScore != nullptr)
		{ // ゲームスコアの情報が NULL じゃない場合

			// スコアを取得する
			m_nScore = m_pGameScore->GetScore();
		}

		// リザルトに遷移する
		CManager::Get()->GetFade()->SetFade(CScene::MODE_RESULT);
	}
}

//======================================
// ポーズ状況の設定処理
//======================================
void CGame::SetEnablePause(const bool bPause)
{
	// ポーズ状況を設定する
	m_bPause = bPause;
}

//======================================
// ポーズ状況の取得処理
//======================================
bool CGame::IsPause(void)
{
	// ポーズ状況を取得する
	return m_bPause;
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
// プレイヤーの取得処理
//======================================
CPlayer* CGame::GetPlayer(void)
{
	// プレイヤーのポインタを返す
	return m_pPlayer;
}

//======================================
// ゲームスコアの取得処理
//======================================
CGameScore* CGame::GetGameScore(void)
{
	// ゲームスコアのポインタを返す
	return m_pGameScore;
}

//======================================
// 総合スコアの取得処理
//======================================
int CGame::GetScore(void)
{
	// 総合スコアを返す
	return m_nScore;
}

//======================================
// ポーズのNULL化処理
//======================================
void CGame::DeletePause(void)
{
	// ポーズのポインタを NULL にする
	m_pPause = nullptr;
}

//======================================
// プレイヤーのNULL化処理
//======================================
void CGame::DeletePlayer(void)
{
	// プレイヤーのポインタを NULL にする
	m_pPlayer = nullptr;
}

//======================================
// スコアのNULL化処理
//======================================
void CGame::DeleteGameScore(void)
{
	// スコアのポインタを NULL にする
	m_pGameScore = nullptr;
}