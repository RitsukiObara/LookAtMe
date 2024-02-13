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
#include "game_staminaUI.h"
#include "object2D.h"
#include "animobject.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define STAMINAUI_MARK_POS			(D3DXVECTOR3(60.0f, 100.0f, 0.0f))														// スタミナUIのマークの位置
#define STAMINAUI_GAGE_POS			(D3DXVECTOR3(STAMINAUI_MARK_POS.x + STAMINAUI_MARK_SIZE.x, STAMINAUI_MARK_POS.y, 0.0f))	// スタミナUIの位置

//============================
// コンストラクタ
//============================
CGameStaminaUI::CGameStaminaUI() : CStaminaUI(TYPE_STAMINAUI, PRIORITY_UI)
{

}

//============================
// デストラクタ
//============================
CGameStaminaUI::~CGameStaminaUI()
{

}

//============================
// 初期化処理
//============================
HRESULT CGameStaminaUI::Init(void)
{
	if (FAILED(CStaminaUI::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================
// 終了処理
//============================
void CGameStaminaUI::Uninit(void)
{
	// 終了処理
	CStaminaUI::Uninit();
}

//============================
// 更新処理
//============================
void CGameStaminaUI::Update(void)
{
	// 更新処理
	CStaminaUI::Update();
}

//============================
// 描画処理
//============================
void CGameStaminaUI::Draw(void)
{
	// 描画処理
	CStaminaUI::Draw();
}

//============================
// 情報の設定処理
//============================
void CGameStaminaUI::SetData(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// 位置を設定する
		GetObject2D((TYPE)(nCnt))->SetPos(STAMINAUI_GAGE_POS);

		// 頂点座標の設定処理
		GetObject2D((TYPE)(nCnt))->SetVertex();
	}

	// マーク
	GetAnim()->SetPos(STAMINAUI_MARK_POS);

	// 設定座標の設定処理
	GetAnim()->SetVertex();
}