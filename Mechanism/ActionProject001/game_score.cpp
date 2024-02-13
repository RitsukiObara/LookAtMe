//============================================
//
// ゲームのスコア処理[game_score.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "game_score.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define NUMBER_SHIFT		(50.0f)							// 数字のずらす幅

//--------------------------------------------
// 静的メンバ変数宣言
//--------------------------------------------
CGameScore* CGameScore::m_pGameScore = nullptr;						// スコアの情報

//========================
// コンストラクタ
//========================
CGameScore::CGameScore() : CScore(TYPE_GAMESCORE, PRIORITY_UI)
{

}

//========================
// デストラクタ
//========================
CGameScore::~CGameScore()
{

}

//========================
// 初期化処理
//========================
HRESULT CGameScore::Init(void)
{
	if (FAILED(CScore::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//========================
// 終了処理
//========================
void CGameScore::Uninit(void)
{
	// 終了処理
	CScore::Uninit();

	// ゲームのスコアを NULL にする
	m_pGameScore = nullptr;
}

//========================
// 更新処理
//========================
void CGameScore::Update(void)
{
	// 更新処理
	CScore::Update();
}

//========================
// 描画処理
//========================
void CGameScore::Draw(void)
{
	// 描画処理
	CScore::Draw();
}

//========================
// 情報の設定処理
//========================
void CGameScore::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size)
{
	// 情報を設定処理
	CScore::SetData(pos, rot, size, NUMBER_SHIFT);
}

//========================
// スコアの取得処理
//========================
CGameScore* CGameScore::Get(void)
{
	if (m_pGameScore != nullptr)
	{ // ゲームのスコアの情報がある場合

		// ゲームのスコアのポインタを返す
		return m_pGameScore;
	}
	else
	{ // 上記以外

		// 停止
		assert(false);

		// ゲームのスコアのポインタを返す
		return m_pGameScore;
	}
}

//========================
// 生成処理
//========================
CGameScore* CGameScore::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size)
{
	if (m_pGameScore == nullptr)
	{ // オブジェクトが NULL の場合

		// メモリを確保する
		m_pGameScore = new CGameScore;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (m_pGameScore != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(m_pGameScore->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		m_pGameScore->SetData(pos, rot, size);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return m_pGameScore;
}