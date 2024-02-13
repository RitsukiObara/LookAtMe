//============================================
//
// ランキングのスコア表示処理[ranking_scoredisp.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "ranking_scoredisp.h"
#include "texture.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define SCOREDISP_POS				(D3DXVECTOR3(500.0f, 80.0f, 0.0f))		// スコア表示の位置
#define SCOREDISP_SIZE				(D3DXVECTOR3(160.0f, 60.0f, 0.0f))		// スコア表示のサイズ
#define SCOREDISP_TEXTURE			"data/TEXTURE/YourScore.png"			// スコア表示のテクスチャ

//========================
// コンストラクタ
//========================
CRankingScoreDisp::CRankingScoreDisp() : CObject2D(CObject::TYPE_RANKINGSCOREDISP, CObject::PRIORITY_UI)
{

}

//========================
// デストラクタ
//========================
CRankingScoreDisp::~CRankingScoreDisp()
{

}

//========================
// 初期化処理
//========================
HRESULT CRankingScoreDisp::Init(void)
{
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 情報の設定
	SetPos(SCOREDISP_POS);					// 位置
	SetPosOld(SCOREDISP_POS);				// 前回の位置
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// 向き
	SetSize(SCOREDISP_SIZE);				// サイズ
	SetAngle();								// 方向
	SetLength();							// 長さ

	SetVertex();							// 頂点座標の設定処理

	// テクスチャの割り当て処理
	BindTexture(CManager::GetTexture()->Regist(SCOREDISP_TEXTURE));

	// 成功を返す
	return S_OK;
}

//========================
// 終了処理
//========================
void CRankingScoreDisp::Uninit(void)
{
	// 破棄処理
	CObject2D::Uninit();
}

//========================
// 更新処理
//========================
void CRankingScoreDisp::Update(void)
{

}

//========================
// 描画処理
//========================
void CRankingScoreDisp::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}

//========================
// 生成処理
//========================
CRankingScoreDisp* CRankingScoreDisp::Create(void)
{
	// ローカルオブジェクトを生成
	CRankingScoreDisp* pScore = nullptr;	// プレイヤーのインスタンスを生成

	if (pScore == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pScore = new CRankingScoreDisp;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pScore != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pScore->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "スコア表示の初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// スコア表示のポインタを返す
	return pScore;
}