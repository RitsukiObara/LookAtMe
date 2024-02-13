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
#include "result_score.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

#include "game.h"



//--------------------------------------------
// 静的メンバ変数宣言
//--------------------------------------------
CResultScore* CResultScore::m_pResultScore = nullptr;						// スコアの情報

//========================
// コンストラクタ
//========================
CResultScore::CResultScore() : CScore(TYPE_GAMESCORE, PRIORITY_UI)
{

}

//========================
// デストラクタ
//========================
CResultScore::~CResultScore()
{

}

//========================
// 初期化処理
//========================
HRESULT CResultScore::Init(void)
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
void CResultScore::Uninit(void)
{
	// 終了処理
	CScore::Uninit();

	// ゲームのスコアを NULL にする
	m_pResultScore = nullptr;
}

//========================
// 更新処理
//========================
void CResultScore::Update(void)
{
	// 更新処理
	CScore::Update();
}

//========================
// 描画処理
//========================
void CResultScore::Draw(void)
{
	// 描画処理
	CScore::Draw();
}

//========================
// 情報の設定処理
//========================
void CResultScore::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size, const float fShift)
{
	// 情報を設定処理
	CScore::SetData(pos, rot, size, fShift);

	// スコアを設定する
	CScore::SetScore(CGame::GetScore());
	CScore::SetScoreDisp(CGame::GetScore());

	// 数値を反映する
	CScore::Update();
}

//========================
// スコアの取得処理
//========================
CResultScore* CResultScore::Get(void)
{
	if (m_pResultScore != nullptr)
	{ // ゲームのスコアの情報がある場合

		// ゲームのスコアのポインタを返す
		return m_pResultScore;
	}
	else
	{ // 上記以外

		// 停止
		assert(false);

		// ゲームのスコアのポインタを返す
		return m_pResultScore;
	}
}

//========================
// 生成処理
//========================
CResultScore* CResultScore::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size, const float fShift)
{
	if (m_pResultScore == nullptr)
	{ // オブジェクトが NULL の場合

		// メモリを確保する
		m_pResultScore = new CResultScore;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (m_pResultScore != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(m_pResultScore->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		m_pResultScore->SetData(pos, rot, size, fShift);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return m_pResultScore;
}