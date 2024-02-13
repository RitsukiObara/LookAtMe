//============================================
//
// マネージャーのメイン処理[manager.cpp]
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

#include "file.h"
#include "camera.h"
#include "result_score.h"
#include "skybox.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define SCORE_SIZE		(D3DXVECTOR3(60.0f, 80.0f, 0.0f))		// スコアのサイズ
#define SCORE_SHIFT		(120.0f)								// スコアのずらす幅
#define SCORE_POS		(D3DXVECTOR3((SCREEN_WIDTH * 0.5f) - ((SCORE_SHIFT * 3) + SCORE_SIZE.x), SCREEN_HEIGHT * 0.5f, 0.0f))		// スコアの位置

//--------------------------------------------
// 静的メンバ変数宣言
//--------------------------------------------
CResult::STATE CResult::m_state = STATE_TIMEDISP;		// 状態

//=========================================
// コンストラクタ
//=========================================
CResult::CResult() : CScene(TYPE_NONE, PRIORITY_BG)
{
	m_state = STATE_TIMEDISP;
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

	// スカイボックスの生成処理
	CSkyBox::Create();

	// テキスト読み込み処理
	CMesh::TxtSet();

	// リザルトスコアを生成する
	CResultScore::Create(SCORE_POS, NONE_D3DXVECTOR3, SCORE_SIZE, SCORE_SHIFT);

	// 全ての値を初期化する
	m_state = STATE_TIMEDISP;		// 状態

	// 成功を返す
	return S_OK;
}

//=============================================
//終了処理
//=============================================
void CResult::Uninit(void)
{
	// 破棄処理
	Release();
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

//======================================
// 状態の設定処理
//======================================
void CResult::SetState(const CResult::STATE state)
{
	// 状態を設定する
	m_state = state;
}

//======================================
// 状態の取得処理
//======================================
CResult::STATE CResult::GetState(void)
{
	// 状態を返す
	return m_state;
}