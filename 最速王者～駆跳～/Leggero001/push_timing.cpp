//============================================
//
// 押しタイミングの表示処理[push_timing.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "push_timing.h"
#include "input.h"
#include "texture.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define BUTTON_TEXTURE	"data/TEXTURE/PushTiming.png"			// ボタンのテクスチャ
#define GAMEPAD_TEXTURE	"data/TEXTURE/PushGamePad.png"			// ゲームボタンのテクスチャ
#define ANIM_COUNT		(8)			// アニメーションカウント
#define ANIM_PATTERN	(2)			// アニメーションパターン
#define BUTTON_SIZE		(D3DXVECTOR3(60.0f, 30.0f, 0.0f))		// ボタンのサイズ
#define GAMEPAD_SIZE	(D3DXVECTOR3(30.0f, 30.0f, 0.0f))		// ゲームパッドのサイズ

//===========================================
// コンストラクタ
//===========================================
CPushTiming::CPushTiming() : CBillboardAnim(CObject::TYPE_PUSHTIMING,CObject::PRIORITY_UI)
{
	// 全ての値をクリアする
	m_bDisp = false;			// 描画状況
}

//===========================================
// デストラクタ
//===========================================
CPushTiming::~CPushTiming()
{

}

//===========================================
// 初期化処理
//===========================================
HRESULT CPushTiming::Init(void)
{
	if (FAILED(CBillboardAnim::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_bDisp = false;			// 描画状況

	// 成功を返す
	return S_OK;
}

//===========================================
// 終了処理
//===========================================
void CPushTiming::Uninit(void)
{
	// 終了
	CBillboardAnim::Uninit();
}

//===========================================
// 更新処理
//===========================================
void CPushTiming::Update(void)
{
	// 更新処理
	CBillboardAnim::Update();

	if (CManager::GetInputGamePad() != nullptr &&
		CManager::GetInputGamePad()->GetConnect() == true)
	{ // ゲームパッドが接続されている場合

		// テクスチャの読み込み処理
		BindTexture(CManager::GetTexture()->Regist(GAMEPAD_TEXTURE));

		// サイズを設定する
		SetSize(GAMEPAD_SIZE);

		// 頂点情報の設定処理
		SetVertex();
	}
	else
	{ // 上記以外

		// テクスチャの読み込み処理
		BindTexture(CManager::GetTexture()->Regist(BUTTON_TEXTURE));

		// サイズを設定する
		SetSize(BUTTON_SIZE);

		// 頂点情報の設定処理
		SetVertex();
	}
}

//===========================================
// 描画処理
//===========================================
void CPushTiming::Draw(void)
{
	if (m_bDisp == true)
	{ // 描画状況が true の場合

		// 描画処理
		CBillboardAnim::DrawLightOff();
	}
}

//===========================================
// 設定処理
//===========================================
void CPushTiming::SetData(const D3DXVECTOR3& pos)
{
	// 全ての値を設定する
	m_bDisp = false;			// 描画状況

	// スクロールの設定処理
	SetPos(pos);				// 位置設定
	SetSize(BUTTON_SIZE);		// サイズ設定

	// アニメーションの設定処理
	SetAnim(ANIM_COUNT, ANIM_PATTERN);

	// 頂点情報の初期化
	SetVertex();

	// テクスチャ情報の設定処理
	SetVtxAnim(GetTexPattern(), GetPattern());

	// テクスチャの読み込み処理
	BindTexture(CManager::GetTexture()->Regist(BUTTON_TEXTURE));
}

//===========================================
// 描画状況の設定処理
//===========================================
void CPushTiming::SetDisp(const bool bDisp)
{
	// 描画状況を設定する
	m_bDisp = bDisp;
}

//===========================================
// 生成処理
//===========================================
CPushTiming* CPushTiming::Create(const D3DXVECTOR3& pos)
{
	// ローカルオブジェクトを生成
	CPushTiming* pPushTiming = nullptr;			// 押しタイミングのインスタンスを生成

	if (pPushTiming == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pPushTiming = new CPushTiming;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pPushTiming != nullptr)
	{ // オブジェクトが NULL じゃない場合

	  // 初期化処理
		if (FAILED(pPushTiming->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "押しタイミングの初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pPushTiming->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// 押しタイミングのポインタを返す
	return pPushTiming;
}