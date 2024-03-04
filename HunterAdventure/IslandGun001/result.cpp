//============================================
//
// リザルトのメイン処理[result.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "renderer.h"
#include "result.h"
#include "input.h"
#include "fade.h"
#include "Objectmesh.h"
#include "game.h"

#include "file.h"
#include "camera.h"
#include "objectElevation.h"
#include "skybox.h"
#include "ocean.h"
#include "result_score.h"

//--------------------------------------------
// 定数定義
//--------------------------------------------
namespace
{
	const char* ELEVATION_TXT = "data/TXT/Elevation.txt";		// 起伏地面のテキスト
}

//=========================================
// コンストラクタ
//=========================================
CResult::CResult()
{

}

//=========================================
// デストラクタ
//=========================================
CResult::~CResult()
{

}

//=========================================
//初期化処理
//=========================================
HRESULT CResult::Init(void)
{
	// シーンの初期化
	CScene::Init();

	// カメラのリセット処理
	CManager::Get()->GetCamera()->Reset();

	// テキスト読み込み処理
	CElevation::TxtSet(ELEVATION_TXT);

	// マップの生成
	CManager::Get()->GetFile()->SetCoin();
	CManager::Get()->GetFile()->SetGoldBone();
	CManager::Get()->GetFile()->SetTree();
	CManager::Get()->GetFile()->SetRock();
	CManager::Get()->GetFile()->SetBlock();
	CManager::Get()->GetFile()->SetBangFlower();
	CManager::Get()->GetFile()->SetWall();

	// スカイボックスの生成処理
	CSkyBox::Create();

	// テキスト読み込み処理
	CMesh::TxtSet();

	// 海の生成
	COcean::Create();

	{
		// リザルトスコアを生成
		CResultScore* pScore = CResultScore::Create();

		if (pScore != nullptr)
		{ // スコアが NULL じゃない場合

			// スコアを設定する
			pScore->SetScore(CGame::GetScore());
		}
	}

	// 成功を返す
	return S_OK;
}

//=============================================
//終了処理
//=============================================
void CResult::Uninit(void)
{
	// 終了処理
	CScene::Uninit();
}

//======================================
//更新処理
//======================================
void CResult::Update(void)
{
	if (CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A,0) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_START, 0) == true)
	{ // ENTERキーを押した場合

		// ランキングに遷移する
		CManager::Get()->GetFade()->SetFade(CScene::MODE_RANKING);

		// この先の処理を行わない
		return;
	}

	if (CManager::Get()->GetRenderer() != nullptr)
	{ // レンダラーが NULL じゃない場合

		// 更新処理
		CManager::Get()->GetRenderer()->Update();
	}
}

//======================================
//描画処理
//======================================
void CResult::Draw(void)
{

}