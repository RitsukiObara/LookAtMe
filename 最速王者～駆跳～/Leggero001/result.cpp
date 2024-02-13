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
#include "result_time.h"
#include "Objectmesh.h"
#include "skybox.h"

#include "file.h"
#include "camera.h"

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

	// ステージの設定処理
	CManager::GetFile()->SetBlock();		// ブロックの設置処理
	CManager::GetFile()->SetObstacle();		// 障害物の設置処理
	CManager::GetFile()->SetItem();			// アイテムの設置処理
	CManager::GetFile()->SetCamera();		// カメラの設置処理

	// テキスト読み込み処理
	CMesh::TxtSet();

	// スカイボックスの設定処理
	CSkyBox::Create();

	// 全ての値を初期化する
	m_state = STATE_TIMEDISP;		// 状態

	// リザルトタイマーの生成処理
	CResultTime::Create();

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
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
		CManager::GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A,0) == true ||
		CManager::GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_START, 0) == true)
	{ // ENTERキーを押した場合

		// ランキングに遷移する
		CManager::GetFade()->SetFade(CScene::MODE_RANKING);

		// この先の処理を行わない
		return;
	}

	if (CManager::GetRenderer() != nullptr)
	{ // レンダラーが NULL じゃない場合

		// 更新処理
		CManager::GetRenderer()->Update();
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