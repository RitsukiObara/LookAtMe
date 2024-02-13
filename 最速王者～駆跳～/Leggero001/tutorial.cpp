//============================================
//
// チュートリアル画面のメイン処理[tutorial.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "tutorial.h"
#include "file.h"
#include "player.h"
#include "input.h"
#include "renderer.h"
#include "object.h"
#include "fade.h"
#include "Objectmesh.h"
#include "skybox.h"
#include "sound.h"

#include "skip_circle.h"
#include "balloonUI.h"
#include "enter_skip.h"

// マクロ定義
#define SKIP_CIRCLE_POS			(D3DXVECTOR3(200.0f, 570.0f, 0.0f))		// スキップ円の位置
#define SKIP_CIRCLE_SIZE		(40.0f)									// スキップ円のサイズ
#define SKIP_CIRCLE_DISTANCE	(30.0f)									// スキップ円の中心からの距離
#define SKIP_CIRCLE_DIVI		(32)									// スキップ円の分割数
#define END_COUNT				(120)									// 終了までのカウント数

//--------------------------------------------
// 静的メンバ変数宣言
//--------------------------------------------
CTutorial::STATE CTutorial::m_state = STATE_NONE;		// 状態
CBalloonUI* CTutorial::m_pBalloonUI = nullptr;			// ふきだし2Dの情報

//=========================================
// コンストラクタ
//=========================================
CTutorial::CTutorial() : CScene(TYPE_NONE, PRIORITY_BG)
{
	// 全ての情報をクリアする
	m_state = STATE_NONE;		// 状態
	m_pBalloonUI = nullptr;		// ふきだし2D
	m_nEndCount = 0;			// 終了までのカウント
}

//=========================================
// デストラクタ
//=========================================
CTutorial::~CTutorial()
{

}

//=========================================
//初期化処理
//=========================================
HRESULT CTutorial::Init(void)
{
	// シーンの初期化
	CScene::Init();

	// テキスト読み込み処理
	CMesh::TxtSet();

	// スカイボックスの設定処理
	CSkyBox::Create();

	// 状態処理を設定する
	m_state = STATE_NONE;		// 状態
	m_pBalloonUI = nullptr;		// ふきだし2D
	m_nEndCount = 0;			// 終了までのカウント

	// 成功を返す
	return S_OK;
}

//=============================================
//終了処理
//=============================================
void CTutorial::Uninit(void)
{
	// 全てのポインタを NULL にする
	m_pBalloonUI = nullptr;			// ふきだし2D

	// 終了処理
	CScene::Uninit();
}

//======================================
//更新処理
//======================================
void CTutorial::Update(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
		CManager::GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_START, 0) == true)
	{ // ENTERキーを押した場合

		// スキップ円の生成処理
		SkipCreate();

		// スキップ円の出現音を鳴らす
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_SKIPCIRCLE);
	}

	if (CManager::GetRenderer() != nullptr)
	{ // レンダラーが NULL じゃない場合

		// レンダラーの更新
		CManager::GetRenderer()->Update();
	}

	if (m_state == STATE_END)
	{ // 終了状態の場合

		// 終了カウントを加算する
		m_nEndCount++;
	}

	if (m_state == STATE_SKIP ||
		m_nEndCount >= END_COUNT)
	{ // スキップ時または、終了時の場合

		// ゲームモードにする
		CManager::GetFade()->SetFade(MODE_GAME);
	}
}

//======================================
//描画処理
//======================================
void CTutorial::Draw(void)
{

}

//======================================
// スキップ円の出現処理
//======================================
void CTutorial::SkipCreate(void)
{
	// ローカル変数宣言
	CObject* pObj = nullptr;		// オブジェクトのポインタ
	CSkipCircle* pSkip = nullptr;	// スキップ円のポインタ

	// 先頭のオブジェクトを取得する
	pObj = CObject::GetTop(PRIORITY_UI);

	while (pObj != nullptr)
	{ // オブジェクトが NULL じゃない場合回す

		if (pObj->GetType() == TYPE_SKIP)
		{ // 種類がスキップの円だった場合

			// スキップにダイナミックキャストする
			pSkip = dynamic_cast<CSkipCircle*>(pObj);

			if (pSkip != nullptr)
			{ // スキップの円が存在していた場合

				// この関数を抜ける
				return;
			}
		}

		// 次のオブジェクトを代入する
		pObj = pObj->GetNext();
	}

	// スキップ円の設定処理
	CSkipCircle::Create(SKIP_CIRCLE_POS, SKIP_CIRCLE_SIZE, SKIP_CIRCLE_DISTANCE, SKIP_CIRCLE_DIVI);
}

//======================================
// 情報の設定処理
//======================================
void CTutorial::SetData(const MODE mode)
{
	// 情報の設定処理
	CScene::SetData(mode);

	if (CManager::GetCamera() != nullptr)
	{ // カメラの情報が NULL じゃない場合

		// 通常カメラに設定する
		CManager::GetCamera()->SetType(CCamera::TYPE_NONE);
	}

	if (CManager::GetFile() != nullptr)
	{ // ファイルの情報が NULL じゃない場合

		// チュートリアルの設定処理
		CManager::GetFile()->SetTutorial();
	}

	// ふきだし2Dの生成処理
	m_pBalloonUI = CBalloonUI::Create();

	// Enterスキップの生成
	CEnterSkip::Create();
}

//======================================
// 状態の設定処理
//======================================
void CTutorial::SetState(const STATE state)
{
	// 状態を設定する
	m_state = state;
}

//======================================
// 状態の取得処理
//======================================
CTutorial::STATE CTutorial::GetState(void)
{
	// 状態を返す
	return m_state;
}

//======================================
// ふきだし2Dの取得処理
//======================================
CBalloonUI* CTutorial::GetBalloonUI(void)
{
	// ふきだし2Dの情報を返す
	return m_pBalloonUI;
}