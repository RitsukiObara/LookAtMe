//============================================
//
// ゲームのスタミナUI処理[game_staminaUI.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "tutorial.h"
#include "tutorial_staminaUI.h"
#include "object2D.h"
#include "animobject.h"
#include "useful.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define STAMINAUI_MARK_POS			(D3DXVECTOR3(60.0f, 100.0f, 0.0f))														// スタミナUIのマークの位置
#define STAMINAUI_GAGE_POS			(D3DXVECTOR3(STAMINAUI_MARK_POS.x + STAMINAUI_MARK_SIZE.x, STAMINAUI_MARK_POS.y, 0.0f))	// スタミナUIの位置
#define DEST_DIFF					(500.0f)																				// 目的との差分
#define MOVE_CORRECT				(0.3f)																					// 移動量の補正率

//============================
// コンストラクタ
//============================
CTutorialStaminaUI::CTutorialStaminaUI() : CStaminaUI(TYPE_STAMINAUI, PRIORITY_UI)
{
	// 全ての値をクリアする
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_posDest2D[nCnt] = 0.0f;		// 目的の位置
	}

	m_posDestAnim = 0.0f;				// 目的の位置
}

//============================
// デストラクタ
//============================
CTutorialStaminaUI::~CTutorialStaminaUI()
{

}

//============================
// 初期化処理
//============================
HRESULT CTutorialStaminaUI::Init(void)
{
	if (FAILED(CStaminaUI::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_posDest2D[nCnt] = 0.0f;		// 目的の位置
	}

	m_posDestAnim = 0.0f;				// 目的の位置

	// 成功を返す
	return S_OK;
}

//============================
// 終了処理
//============================
void CTutorialStaminaUI::Uninit(void)
{
	// 終了処理
	CStaminaUI::Uninit();
}

//============================
// 更新処理
//============================
void CTutorialStaminaUI::Update(void)
{
	// 更新処理
	CStaminaUI::Update();

	if (CTutorial::GetState() == CTutorial::STATE_STAMINA)
	{ // スタミナ状態だった場合

		// 目的の位置を設定する
		m_posDest2D[TYPE::TYPE_GAGE] = STAMINAUI_GAGE_POS.x;
		m_posDest2D[TYPE::TYPE_METER] = STAMINAUI_GAGE_POS.x;
		m_posDestAnim = STAMINAUI_MARK_POS.x;
	}
	else
	{ // 上記以外

		// 目的の位置を設定する
		m_posDest2D[TYPE::TYPE_GAGE] = STAMINAUI_GAGE_POS.x - DEST_DIFF;
		m_posDest2D[TYPE::TYPE_METER] = STAMINAUI_GAGE_POS.x - DEST_DIFF;
		m_posDestAnim = STAMINAUI_MARK_POS.x - DEST_DIFF;
	}

	// 位置の補正処理
	PosCorrect();
}

//============================
// 描画処理
//============================
void CTutorialStaminaUI::Draw(void)
{
	// 描画処理
	CStaminaUI::Draw();
}

//============================
// 情報の設定処理
//============================
void CTutorialStaminaUI::SetData(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// 位置を設定する
		GetObject2D((TYPE)(nCnt))->SetPos(D3DXVECTOR3(STAMINAUI_GAGE_POS.x - DEST_DIFF, STAMINAUI_GAGE_POS.y, STAMINAUI_GAGE_POS.z));

		// 頂点座標の設定処理
		GetObject2D((TYPE)(nCnt))->SetVertex();
	}

	// マーク
	GetAnim()->SetPos(D3DXVECTOR3(STAMINAUI_MARK_POS.x - DEST_DIFF, STAMINAUI_MARK_POS.y, STAMINAUI_MARK_POS.z));

	// 設定座標の設定処理
	GetAnim()->SetVertex();

	// 全ての値を設定する
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_posDest2D[nCnt] = STAMINAUI_GAGE_POS.x;		// 目的の位置
	}

	m_posDestAnim = STAMINAUI_MARK_POS.x;	// 目的の位置
}

//============================
// 補正処理
//============================
void CTutorialStaminaUI::PosCorrect(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos;		// 位置の変数

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// 位置を設定する
		pos = GetObject2D((TYPE)(nCnt))->GetPos();

		// 位置の補正処理
		Correct(m_posDest2D[nCnt], &pos.x, MOVE_CORRECT);

		// 位置を更新する
		GetObject2D((TYPE)(nCnt))->SetPos(pos);
	}

	// 位置を設定する
	pos = GetAnim()->GetPos();

	// 位置の補正処理
	Correct(m_posDestAnim, &pos.x, MOVE_CORRECT);

	// 位置を更新する
	GetAnim()->SetPos(pos);
}