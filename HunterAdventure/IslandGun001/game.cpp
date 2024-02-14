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
#include "boss.h"
#include "enemy.h"
#include "list_manager.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define TRANS_COUNT		(80)		// 成功時の遷移カウント

//--------------------------------------------
// 静的メンバ変数宣言
//--------------------------------------------
CPause* CGame::m_pPause = nullptr;							// ポーズの情報
CPlayer* CGame::m_pPlayer = nullptr;						// プレイヤーの情報
CGame::STATE CGame::m_GameState = CGame::STATE_START;		// ゲームの進行状態
int CGame::m_nFinishCount = 0;								// 終了カウント
bool CGame::m_bPause = false;								// ポーズ状況

//=========================================
// コンストラクタ
//=========================================
CGame::CGame()
{
	// 全ての値をクリアする
	m_pPause = nullptr;			// ポーズ
	m_pPlayer = nullptr;		// プレイヤー
	m_nFinishCount = 0;			// 終了カウント
	m_GameState = STATE_START;	// 状態
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
	// テキスト読み込み処理
	CElevation::TxtSet();

	// メッシュのテキスト読み込み
	//CMesh::TxtSet();

	//if (m_pField == NULL)
	//{ // フィールドへのポインタが NULL の場合

	//	// フィールドの設定処理
	//	m_pField = CField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1000.0f, 0.0f, 1000.0f));
	//}

	// モーションの読み込み処理
	CMotion::Load(CMotion::STYLE_PLAYER);		// プレイヤー
	CMotion::Load(CMotion::STYLE_TORDLE);		// タードル
	CMotion::Load(CMotion::STYLE_IWAKARI);		// イワカリ
	CMotion::Load(CMotion::STYLE_BOSS);			// ボス

	// マップをロード
	CManager::Get()->GetFile()->Load(CFile::TYPE_ENEMY);
	CManager::Get()->GetFile()->Load(CFile::TYPE_COIN);
	CManager::Get()->GetFile()->Load(CFile::TYPE_GOLDBONE);
	CManager::Get()->GetFile()->Load(CFile::TYPE_TREE);
	CManager::Get()->GetFile()->Load(CFile::TYPE_ROCK);
	CManager::Get()->GetFile()->Load(CFile::TYPE_BLOCK);
	CManager::Get()->GetFile()->Load(CFile::TYPE_BANGFLOWER);
	CManager::Get()->GetFile()->Load(CFile::TYPE_WALL);

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

		// ポーズ処理
		Pause();

		break;

	case CGame::STATE_PLAY:

		// ポーズ処理
		Pause();

		break;

	case CGame::STATE_GOAL:

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

	if (CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_0) == true)
	{ // 0キーを押した

		// ボスの生成処理
		CBoss::Create(NONE_D3DXVECTOR3, NONE_D3DXVECTOR3);
	}

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

		if (CEnemy::GetList().IsEmpty() == true &&
			m_GameState != STATE_GOAL)
		{ // 敵を全て倒した場合

			// ゴール状態にする
			m_GameState = STATE_GOAL;
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