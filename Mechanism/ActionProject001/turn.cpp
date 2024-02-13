//============================================
//
// 振り向きのメイン処理[turn.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "turn.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define TURN_TEXTURE		"data/TEXTURE/SmokeStaging.png"		// 振り向きのテクスチャ
#define TURN_SIZE			(D3DXVECTOR3(50.0f, 40.0f, 0.0f))	// 振り向きのサイズ
#define TURN_COUNT			(2)									// 振り向きのカウント
#define TURN_PATTERN		(8)									// 振り向きのパターン数

//===========================================
// コンストラクタ
//===========================================
CTurn::CTurn() : CBillboardAnim(CObject::TYPE_TURN,CObject::PRIORITY_EFFECT)
{
	// 全ての値をクリアする
	m_nLife = 0;				// 寿命
}

//===========================================
// デストラクタ
//===========================================
CTurn::~CTurn()
{

}

//===========================================
// 初期化処理
//===========================================
HRESULT CTurn::Init(void)
{
	if (FAILED(CBillboardAnim::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_nLife = 0;				// 寿命

	// 成功を返す
	return S_OK;
}

//===========================================
// 終了処理
//===========================================
void CTurn::Uninit(void)
{
	// 終了
	CBillboardAnim::Uninit();
}

//===========================================
// 更新処理
//===========================================
void CTurn::Update(void)
{
	// 寿命を減らす
	m_nLife--;

	// 更新処理
	CBillboardAnim::Update();

	if (m_bRight == true)
	{ // 右向きの場合

		// テクスチャ座標の設定処理
		SetVtxAnimRev(GetTexPattern(), GetPattern());
	}
	else
	{ // 上記以外

		// テクスチャ座標の設定処理
		SetVtxAnim(GetTexPattern(), GetPattern());
	}

	if (m_nLife <= 0)
	{ // パターン数が最大数を超えた場合

		// 終了処理
		Uninit();
	}
}

//===========================================
// 描画処理
//===========================================
void CTurn::Draw(void)
{
	// 描画処理
	CBillboardAnim::DrawLightOff();
}

//===========================================
// 設定処理
//===========================================
void CTurn::SetData(const D3DXVECTOR3& pos, const bool bRight)
{
	// スクロールの設定処理
	SetPos(pos);				// 位置設定
	SetSize(TURN_SIZE);			// サイズ設定

	// アニメーションの設定処理
	SetAnim(TURN_COUNT, TURN_PATTERN);

	// 全ての値を初期化する
	m_nLife = TURN_COUNT * TURN_PATTERN;	// 寿命
	m_bRight = bRight;						// 右向き状況

	// 頂点情報の初期化
	SetVertex();

	if (m_bRight == true)
	{ // 右向きの場合

		// テクスチャ座標の設定処理
		SetVtxAnimRev(GetTexPattern(), GetPattern());
	}
	else
	{ // 上記以外

		// テクスチャ座標の設定処理
		SetVtxAnim(GetTexPattern(), GetPattern());
	}

	// テクスチャの読み込み処理
	BindTexture(CManager::Get()->GetTexture()->Regist(TURN_TEXTURE));
}

//===========================================
// 生成処理
//===========================================
CTurn* CTurn::Create(const D3DXVECTOR3& pos, const bool bRight)
{
	// ローカルオブジェクトを生成
	CTurn* pTurn = nullptr;	// プレイヤーのインスタンスを生成

	if (pTurn == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pTurn = new CTurn;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pTurn != nullptr)
	{ // オブジェクトが NULL じゃない場合

	  // 初期化処理
		if (FAILED(pTurn->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pTurn->SetData(pos, bRight);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 振り向きのポインタを返す
	return pTurn;
}