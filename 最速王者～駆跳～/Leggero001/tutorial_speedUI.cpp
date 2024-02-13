//============================================
//
// チュートリアルの速度UI処理[tutorial_speedUI.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "tutorial.h"
#include "tutorial_speedUI.h"
#include "object2D.h"
#include "balloonUI.h"
#include "useful.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define SPEEDUI_MARK_POS		(D3DXVECTOR3(60.0f, 200.0f, 0.0f))													// 速度UIのマークの位置
#define SPEEDUI_GAGE_POS		(D3DXVECTOR3(SPEEDUI_MARK_POS.x + SPEEDUI_MARK_SIZE.x, SPEEDUI_MARK_POS.y, 0.0f))	// 速度UIのゲージの位置
#define SPEEDUI_METER_POS		(SPEEDUI_GAGE_POS)																	// 速度UIのメーターの位置
#define DEST_DIFF				(500.0f)																			// 目的との差分
#define MOVE_CORRECT			(0.3f)																				// 移動量の補正率

//============================
// コンストラクタ
//============================
CTutorialSpeedUI::CTutorialSpeedUI() : CSpeedUI(CObject::TYPE_SPEEDUI, PRIORITY_UI)
{
	// 全ての値をクリアする
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_posDest[nCnt] = 0.0f;		// 頂点座標
	}
}

//============================
// デストラクタ
//============================
CTutorialSpeedUI::~CTutorialSpeedUI()
{

}

//============================
// 初期化処理
//============================
HRESULT CTutorialSpeedUI::Init(void)
{
	if (FAILED(CSpeedUI::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値をクリアする
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_posDest[nCnt] = 0.0f;		// 頂点座標
	}

	// 成功を返す
	return S_OK;
}

//============================
// 終了処理
//============================
void CTutorialSpeedUI::Uninit(void)
{
	// 終了処理
	CSpeedUI::Uninit();
}

//============================
// 更新処理
//============================
void CTutorialSpeedUI::Update(void)
{
	// 更新処理
	CSpeedUI::Update();

	if (CTutorial::GetState() == CTutorial::STATE_BOOST ||
		CTutorial::GetState() == CTutorial::STATE_SPEED)
	{ // ブースト状態または、速度UI表示状態だった場合

		// 目的の位置を設定する
		m_posDest[TYPE::TYPE_GAGE] = SPEEDUI_GAGE_POS.x;
		m_posDest[TYPE::TYPE_METER] = SPEEDUI_METER_POS.x;
		m_posDest[TYPE::TYPE_MARK] = SPEEDUI_MARK_POS.x;
	}
	else
	{ // 上記以外

		// 目的の位置を設定する
		m_posDest[TYPE::TYPE_GAGE] = SPEEDUI_GAGE_POS.x - DEST_DIFF;
		m_posDest[TYPE::TYPE_METER] = SPEEDUI_METER_POS.x - DEST_DIFF;
		m_posDest[TYPE::TYPE_MARK] = SPEEDUI_MARK_POS.x - DEST_DIFF;
	}

	// 補正処理
	PosCorrect();
}

//============================
// 描画処理
//============================
void CTutorialSpeedUI::Draw(void)
{
	// 描画処理
	CSpeedUI::Draw();
}

//============================
// 情報の設定処理
//============================
void CTutorialSpeedUI::SetData(void)
{
	// 位置を設定する
	GetObject2D(TYPE::TYPE_GAGE)->SetPos(D3DXVECTOR3(SPEEDUI_GAGE_POS.x - DEST_DIFF, SPEEDUI_GAGE_POS.y, 0.0f));		// ゲージ
	GetObject2D(TYPE::TYPE_METER)->SetPos(D3DXVECTOR3(SPEEDUI_METER_POS.x - DEST_DIFF, SPEEDUI_METER_POS.y, 0.0f));		// メーター
	GetObject2D(TYPE::TYPE_MARK)->SetPos(D3DXVECTOR3(SPEEDUI_MARK_POS.x - DEST_DIFF, SPEEDUI_MARK_POS.y, 0.0f));		// マーク

	// 目的の位置を設定する
	m_posDest[TYPE::TYPE_GAGE] = SPEEDUI_GAGE_POS.x;
	m_posDest[TYPE::TYPE_METER] = SPEEDUI_METER_POS.x;
	m_posDest[TYPE::TYPE_MARK] = SPEEDUI_MARK_POS.x;

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// 頂点座標の設定処理
		GetObject2D((TYPE)(nCnt))->SetVertex();
	}
}

//============================
// 補正処理
//============================
void CTutorialSpeedUI::PosCorrect(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos;		// 位置の変数

	for (int nCnt = 0; nCnt < CSpeedUI::TYPE_MAX; nCnt++)
	{
		// 位置を設定する
		pos = GetObject2D((CSpeedUI::TYPE)(nCnt))->GetPos();

		// 位置の補正処理
		Correct(m_posDest[nCnt], &pos.x, MOVE_CORRECT);

		// 位置を設定する
		GetObject2D((CSpeedUI::TYPE)(nCnt))->SetPos(pos);
	}
}