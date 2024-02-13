//============================================
//
// チュートリアルの速度UI処理[tutorial_speedUI.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "game_speedUI.h"
#include "object2D.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define SPEEDUI_MARK_POS			(D3DXVECTOR3(60.0f, 200.0f, 0.0f))													// 速度UIのマークの位置
#define SPEEDUI_GAGE_POS			(D3DXVECTOR3(SPEEDUI_MARK_POS.x + SPEEDUI_MARK_SIZE.x, SPEEDUI_MARK_POS.y, 0.0f))	// 速度UIのゲージの位置
#define SPEEDUI_METER_POS			(SPEEDUI_GAGE_POS)																	// 速度UIのメーターの位置

//============================
// コンストラクタ
//============================
CGameSpeedUI::CGameSpeedUI() : CSpeedUI(CObject::TYPE_SPEEDUI, PRIORITY_UI)
{

}

//============================
// デストラクタ
//============================
CGameSpeedUI::~CGameSpeedUI()
{

}

//============================
// 初期化処理
//============================
HRESULT CGameSpeedUI::Init(void)
{
	if (FAILED(CSpeedUI::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================
// 終了処理
//============================
void CGameSpeedUI::Uninit(void)
{
	// 終了処理
	CSpeedUI::Uninit();
}

//============================
// 更新処理
//============================
void CGameSpeedUI::Update(void)
{
	// 更新処理
	CSpeedUI::Update();
}

//============================
// 描画処理
//============================
void CGameSpeedUI::Draw(void)
{
	// 描画処理
	CSpeedUI::Draw();
}

//============================
// 情報の設定処理
//============================
void CGameSpeedUI::SetData(void)
{
	// 位置を設定する
	GetObject2D(TYPE::TYPE_GAGE)->SetPos(SPEEDUI_GAGE_POS);
	GetObject2D(TYPE::TYPE_METER)->SetPos(SPEEDUI_METER_POS);
	GetObject2D(TYPE::TYPE_MARK)->SetPos(SPEEDUI_MARK_POS);
}