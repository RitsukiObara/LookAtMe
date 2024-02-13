//============================================
//
// スタミナUI処理[staminaUI.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "staminaUI.h"
#include "game_staminaUI.h"
#include "tutorial_staminaUI.h"
#include "object2D.h"
#include "animobject.h"
#include "texture.h"

#include "player.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define STAMINAUI_CONVERSION		(3.0f)																		// 変換倍率
#define STAMINAUI_GAGE_SIZE			(D3DXVECTOR3(PLAYER_MAX_STAMINA * STAMINAUI_CONVERSION, 40.0f, 0.0f))		// スタミナUIのサイズ
#define STAMINAUI_SUB_MAGNI			(0.1f)																		// スタミナの減算量の倍率
#define STAMINAUI_COLOR				(1.0f / PLAYER_MAX_STAMINA)													// スタミナUIの色の倍率
#define STAMINAUI_FLASH				(PLAYER_MAX_STAMINA * 0.3f)													// スタミナUIが点滅するスタミナ数
#define STAMINAUI_FLASH_COUNT		(8)																			// スタミナUIが点滅するカウント数
#define STAMINAUI_FLASH_COLOR		(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))											// スタミナUIが点滅している時の色
#define STAMINAUI_ANIM_COUNT_GO		(180)																		// スタミナUIが動くタイミング
#define STAMINAUI_ANIM_COUNT_BACK	(10)																		// スタミナUIが戻るまでの時間
#define STAMINAUI_ANIM_PATTERN		(2)																			// スタミナUIのアニメーションパターン
#define STAMINAUI_HEALSTATE_COUNT	(50)																		// スタミナUIの回復状態のカウント数
#define STAMINAUI_METER_TEXTURE		((float)(1.0f / STAMINAUI_GAGE_SIZE.x))										// 速度UIの1メモリのテクスチャの値

//--------------------------------------------
// 静的メンバ変数宣言
//--------------------------------------------
const char* CStaminaUI::m_apTexturename[CStaminaUI::TEX_MAX] =			// テクスチャのパス名
{
	"data/TEXTURE/StaminaGage.png",			// ゲージ
	"data/TEXTURE/StaminaMeter.png",		// メーター
	"data/TEXTURE/StaminaMark.png",			// マーク
};

//============================
// コンストラクタ
//============================
CStaminaUI::CStaminaUI() : CObject(TYPE_STAMINAUI, PRIORITY_UI)
{
	// コンストラクタの箱
	Box();
}

//============================
// オーバーロードコンストラクタ
//============================
CStaminaUI::CStaminaUI(CObject::TYPE type, PRIORITY priority) : CObject(type, priority)
{
	// コンストラクタの箱
	Box();
}

//============================
// デストラクタ
//============================
CStaminaUI::~CStaminaUI()
{

}

//============================
// コンストラクタの箱
//============================
void CStaminaUI::Box(void)
{
	// 全ての値をクリアする
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_apObject2D[nCnt] = nullptr;		// 2Dオブジェクトの情報
	}

	m_pAnim = nullptr;			// アニメーションの情報
	m_fStaminaDest = 0.0f;		// 目標のスタミナ
	m_fSubStamina = 0.0f;		// スタミナの減算値
	m_bFlash = false;			// 点滅状況
	m_nFlashCount = 0;			// 点滅カウント
	m_nStateCount = 0;			// 状態カウント
	m_state = STATE_NONE;		// 状態
}

//============================
// 初期化処理
//============================
HRESULT CStaminaUI::Init(void)
{
	for (int nCntUI = 0; nCntUI < TYPE_MAX; nCntUI++)
	{
		// 生成処理
		m_apObject2D[nCntUI] = CObject2D::Create(CObject2D::TYPE_NONE, CObject::TYPE_NONE, CObject::PRIORITY_UI);

		if (m_apObject2D[nCntUI] != nullptr)
		{ // オブジェクトの情報が NULL じゃない場合

			// スクロールの設定処理
			m_apObject2D[nCntUI]->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// 位置設定
			m_apObject2D[nCntUI]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// 向き設定
			m_apObject2D[nCntUI]->SetSize(STAMINAUI_GAGE_SIZE);				// サイズ設定
			m_apObject2D[nCntUI]->SetLength();								// 長さ設定
			m_apObject2D[nCntUI]->SetAngle();								// 方向設定
			
			// 頂点情報の初期化
			m_apObject2D[nCntUI]->SetVtxWidthGage();

			// テクスチャの割り当て処理
			m_apObject2D[nCntUI]->BindTexture(CManager::GetTexture()->Regist(m_apTexturename[nCntUI]));
		}

			// 頂点情報の初期化
			m_apObject2D[nCntUI]->SetVertex();
	}

	if (m_pAnim == nullptr)
	{ // アニメーションの情報が NULL の場合

		// アニメーションの生成
		m_pAnim = CAnim::Create(CObject::TYPE_NONE, CObject::PRIORITY_UI);

		if (m_pAnim != nullptr)
		{ // アニメーションの情報が NULL じゃない場合

			// 情報の設定処理
			m_pAnim->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// 位置
			m_pAnim->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// 向き
			m_pAnim->SetSize(STAMINAUI_MARK_SIZE);				// サイズ
			m_pAnim->SetAngle();								// 方向
			m_pAnim->SetLength();								// 長さ
			m_pAnim->SetAnim(STAMINAUI_ANIM_COUNT_GO, STAMINAUI_ANIM_PATTERN);		// アニメーションの情報

			// テクスチャの割り当て処理
			m_pAnim->BindTexture(CManager::GetTexture()->Regist(m_apTexturename[TEX_MARK]));

			// 頂点の設定処理
			m_pAnim->SetVertex();

			// テクスチャ座標の設定処理
			m_pAnim->SetVtxTextureAnim(m_pAnim->GetTexPattern(), m_pAnim->GetPattern());
		}
	}

	// 情報の初期化
	m_fStaminaDest = m_apObject2D[TYPE_METER]->GetSize().x;		// 目標のスタミナ
	m_fSubStamina = 0.0f;										// スタミナの減算値
	m_bFlash = false;											// 点滅状況
	m_nFlashCount = 0;											// 点滅カウント
	m_nStateCount = 0;											// 状態カウント
	m_state = STATE_NONE;										// 状態

	// 成功を返す
	return S_OK;
}

//============================
// 終了処理
//============================
void CStaminaUI::Uninit(void)
{
	for (int nCntUI = 0; nCntUI < TYPE_MAX; nCntUI++)
	{
		// 終了処理
		m_apObject2D[nCntUI]->Uninit();
		m_apObject2D[nCntUI] = nullptr;
	}

	// アニメーションの終了処理
	m_pAnim->Uninit();
	m_pAnim = nullptr;

	// 破棄処理
	Release();
}

//============================
// 更新処理
//============================
void CStaminaUI::Update(void)
{
	// スタミナの反映処理
	StaminaInfluence();

	// サイズの減算処理
	SizeSub();

	// 色の設定処理
	ColorCalc();

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{ // オブジェクトが NULL じゃない場合

			// 頂点情報の設定処理
			m_apObject2D[nCnt]->SetVtxWidthGage();
		}
	}

	switch (m_state)
	{
	case STATE_NONE:		// 通常状態

		if (m_pAnim != nullptr)
		{ // アニメーションの情報が NULL じゃなかった場合

			// 通常状態の処理
			NoneStateProcess();
		}

		break;

	case STATE_HEAL:		// 回復状態

		if (m_pAnim != nullptr)
		{ // アニメーションの情報が NULL じゃなかった場合

			// 回復状態の処理
			HealStateProcess();
		}

		// 状態カウントを加算する
		m_nStateCount++;

		if (m_nStateCount >= STAMINAUI_HEALSTATE_COUNT)
		{ // 状態カウントが一定数以上になった場合

			// 通常状態にする
			m_state = STATE_NONE;

			// 状態カウントを初期化する
			m_nStateCount = 0;
		}

		break;
	}

	// 頂点座標の設定処理
	m_pAnim->SetVertex();
}

//============================
// 描画処理
//============================
void CStaminaUI::Draw(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{ // 2Dオブジェクトの情報が NULL じゃない場合

			// 描画処理
			m_apObject2D[nCnt]->Draw();
		}
	}

	if (m_pAnim != nullptr)
	{ // アニメーションが NULL じゃない場合

		// アニメーションの描画処理
		m_pAnim->Draw();
	}
}

//============================
// 状態の設定処理
//============================
void CStaminaUI::SetState(const STATE state)
{
	// 状態を設定する
	m_state = state;
}

//============================
// 状態の取得処理
//============================
CStaminaUI::STATE CStaminaUI::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//============================
// オブジェクト2Dの取得処理
//============================
CObject2D* CStaminaUI::GetObject2D(const TYPE type) const
{
	// オブジェクト2Dの情報を返す
	return m_apObject2D[type];
}

//============================
// アニメーションの取得処理
//============================
CAnim* CStaminaUI::GetAnim(void) const
{
	// アニメーションの情報を返す
	return m_pAnim;
}

//============================
//生成処理
//============================
CStaminaUI* CStaminaUI::Create(const CScene::MODE mode)
{
	// ローカルオブジェクトを生成
	CStaminaUI* pStamina = nullptr;	// プレイヤーのインスタンスを生成

	if (pStamina == nullptr)
	{ // オブジェクトが NULL の場合

		switch (mode)
		{
		case CScene::MODE_TUTORIAL:		// チュートリアル

			// オブジェクトを生成
			pStamina = new CTutorialStaminaUI;

			break;

		case CScene::MODE_GAME:			// ゲーム

			// オブジェクトを生成
			pStamina = new CGameStaminaUI;

			break;

		default:			// 上記以外

			// NULL を返す
			return nullptr;

			break;
		}
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pStamina != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pStamina->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "スタミナUIの初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// モードの設定処理
		pStamina->SetData();
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pStamina;
}

//============================
// スタミナ反映処理
//============================
void CStaminaUI::StaminaInfluence(void)
{
	// ローカル変数宣言
	CPlayer* pPlayer = nullptr;					// プレイヤーのポインタ
	float fStaminaDestOld = m_fStaminaDest;		// 前回のスタミナ値

	// プレイヤーの情報を取得する
	pPlayer = CScene::GetPlayer();

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		// 目標のスタミナを設定する
		m_fStaminaDest = pPlayer->GetStamina() * STAMINAUI_CONVERSION;

		if (m_fStaminaDest != fStaminaDestOld)
		{ // スタミナ値が変化していた場合

			// スタミナの減算量計算処理
			CalcStaminaSub();
		}
	}
}

//============================
// サイズの減算処理
//============================
void CStaminaUI::SizeSub(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 size = m_apObject2D[TYPE_METER]->GetSize();		// サイズを取得する

	if (m_fStaminaDest < m_apObject2D[TYPE_METER]->GetSize().x)
	{ // 目標のスタミナに達していない場合

		// サイズを減らす
		size.x += m_fSubStamina;
	}
	else
	{ // 上記以外

		// スタミナの値を補正する
		size.x = m_fStaminaDest;

		// スタミナの差分を初期化する
		m_fSubStamina = 0.0f;
	}

	// サイズを設定する
	m_apObject2D[TYPE_METER]->SetSize(size);

	// テクスチャの設定処理
	m_apObject2D[TYPE_METER]->SetVtxTextureWidth(size.x * STAMINAUI_METER_TEXTURE);
}

//============================
// スタミナの減算量計算処理
//============================
void CStaminaUI::CalcStaminaSub(void)
{
	if (m_apObject2D[TYPE_METER] != nullptr)
	{ // メーターのポインタが使われている場合

		// スタミナの減算量を算出する
		m_fSubStamina = (m_fStaminaDest - m_apObject2D[TYPE_METER]->GetSize().x) * STAMINAUI_SUB_MAGNI;
	}
}

//============================
// スタミナのカラー計算処理
//============================
void CStaminaUI::ColorCalc(void)
{
	// ローカル変数宣言
	CPlayer* pPlayer = nullptr;					// プレイヤーのポインタ

	// プレイヤーの情報を取得する
	pPlayer = CScene::GetPlayer();

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		if (pPlayer->GetStamina() <= STAMINAUI_FLASH)
		{ // スタミナが一定数以下になった場合

			// 点滅カウントを加算する
			m_nFlashCount++;

			if(m_nFlashCount % STAMINAUI_FLASH_COUNT == 0)
			{ // 点滅カウントが一定数になった場合

				// 点滅状況を切り替える
				m_bFlash = m_bFlash ? false : true;
			}

			if (m_bFlash == true)
			{ // 点滅状況が true だった場合

				// 頂点カラーの設定処理
				m_apObject2D[TYPE_METER]->SetVtxColor(STAMINAUI_FLASH_COLOR);
			}
			else
			{ // 上記以外

				// 頂点カラーの設定処理
				m_apObject2D[TYPE_METER]->SetVtxColor(D3DXCOLOR(1.0f - STAMINAUI_COLOR * pPlayer->GetStamina(), STAMINAUI_COLOR * pPlayer->GetStamina(), 0.0f, 1.0f));
			}
		}
		else
		{ // スタミナがまだ十分にあった場合

			// 頂点カラーの設定処理
			m_apObject2D[TYPE_METER]->SetVtxColor(D3DXCOLOR(1.0f - STAMINAUI_COLOR * pPlayer->GetStamina(), STAMINAUI_COLOR * pPlayer->GetStamina(), 0.0f, 1.0f));

			// 点滅状況を false にする
			m_bFlash = false;

			// 点滅カウントを初期化する
			m_nFlashCount = 0;
		}
	}
}

//============================
// 通常状態の処理
//============================
void CStaminaUI::NoneStateProcess(void)
{
	// アニメーションの更新処理
	m_pAnim->Update();

	if (m_pAnim->GetPattern() == 0)
	{ // アニメーションが 0コマ目の場合

		// アニメーションを設定する
		m_pAnim->SetAnim(STAMINAUI_ANIM_COUNT_GO, STAMINAUI_ANIM_PATTERN);
	}
	else
	{ // 上記以外

		// アニメーションを設定する
		m_pAnim->SetAnim(STAMINAUI_ANIM_COUNT_BACK, STAMINAUI_ANIM_PATTERN);
	}
}

//============================
// 回復状態の処理
//============================
void CStaminaUI::HealStateProcess(void)
{
	// アニメーションの更新処理
	m_pAnim->Update();

	// アニメーションを設定する
	m_pAnim->SetAnim(STAMINAUI_ANIM_COUNT_BACK, STAMINAUI_ANIM_PATTERN);
}