//============================================
//
// ランクスコアのスコア処理[rank_score.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "rank_score.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

//--------------------------------------------
// 定数定義
//--------------------------------------------
namespace
{
	// 数字関係
	const char* NUMBER_TEXTURE = "data\\TEXTURE\\Number.png";			// 数字のテクスチャ
	const D3DXVECTOR3 SCORE_SIZE = D3DXVECTOR3(30.0f, 40.0f, 0.0f);		// サイズ
	const float NUMBER_SHIFT = 60.0f;		// 数字のずらす幅
}

//========================
// コンストラクタ
//========================
CRankScore::CRankScore() : CScore()
{

}

//========================
// デストラクタ
//========================
CRankScore::~CRankScore()
{

}

//========================
// 初期化処理
//========================
HRESULT CRankScore::Init(void)
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
void CRankScore::Uninit(void)
{
	// 終了処理
	CScore::Uninit();
}

//========================
// 更新処理
//========================
void CRankScore::Update(void)
{
	// 更新処理
	CScore::Update();
}

//========================
// 描画処理
//========================
void CRankScore::Draw(void)
{
	// 描画処理
	CScore::Draw();
}

//========================
// 情報の設定処理
//========================
void CRankScore::SetData(const D3DXVECTOR3& pos, const int nScore)
{
	// 情報を設定処理
	CScore::SetData(pos, NONE_D3DXVECTOR3, SCORE_SIZE, NUMBER_SHIFT, NUMBER_TEXTURE, 0);

	// スコアを設定する
	SetScore(nScore);		// スコア
	SetScoreDisp(nScore);	// 描画スコア
}

//========================
// 生成処理
//========================
CRankScore* CRankScore::Create(const D3DXVECTOR3& pos, const int nScore)
{
	// スコアのポインタを宣言
	CRankScore* pScore = nullptr;

	if (pScore == nullptr)
	{ // オブジェクトが NULL の場合

		// メモリを確保する
		pScore = new CRankScore;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pScore != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pScore->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pScore->SetData(pos, nScore);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// スコアのポインタを返す
	return pScore;
}