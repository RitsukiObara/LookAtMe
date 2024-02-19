//============================================
//
// タイトル画面のメイン処理[title.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "title.h"
#include "fade.h"
#include "file.h"
#include "input.h"
#include "renderer.h"

#include "skybox.h"
#include "title_logo.h"
#include "objectElevation.h"
#include "Objectmesh.h"

// マクロ定義
#define SET_RANKING_TIMER		(600)		// ランキング画面に遷移するカウント数

//=========================================
// コンストラクタ
//=========================================
CTitle::CTitle()
{
	// 全ての値をクリアする
	m_nTransCount = 0;
}

//=========================================
// デストラクタ
//=========================================
CTitle::~CTitle()
{

}

//=========================================
//初期化処理
//=========================================
HRESULT CTitle::Init(void)
{
	//　シーンの初期化
	CScene::Init();

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
	CManager::Get()->GetFile()->SetBlock();
	CManager::Get()->GetFile()->SetBangFlower();
	CManager::Get()->GetFile()->SetWall();

	// テキスト読み込み処理
	CMesh::TxtSet();

	// スカイボックスの生成処理
	CSkyBox::Create();

	// タイトルロゴの生成
	CTitleLogo::Create();

	// 全ての値を初期化する
	m_nTransCount = 0;

	// 成功を返す
	return S_OK;
}

//=============================================
//終了処理
//=============================================
void CTitle::Uninit(void)
{
	// 終了処理
	CScene::Uninit();
}

//======================================
//更新処理
//======================================
void CTitle::Update(void)
{
	// 遷移カウントを加算する
	m_nTransCount++;

	// レンダラーの更新
	CManager::Get()->GetRenderer()->Update();

	//if (m_nTransCount % SET_RANKING_TIMER == 0)
	//{ // 遷移カウントが一定数に達した場合

	//	// ランキングに遷移する
	//	CManager::Get()->GetFade()->SetFade(CScene::MODE_RANKING);

	//	// この先の処理を行わない
	//	return;
	//}

	if (CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_START, 0) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true)
	{ // ENTERキーを押した場合

		// ゲームに遷移する
		CManager::Get()->GetFade()->SetFade(CScene::MODE_GAME);

		// この先の処理を行わない
		return;
	}
}

//======================================
//描画処理
//======================================
void CTitle::Draw(void)
{

}