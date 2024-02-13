//============================================
//
// ランキングの文字処理[ranking_word.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "ranking_word.h"
#include "texture.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define RANKING_WORD_POS				(D3DXVECTOR3(120.0f, 40.0f, 0.0f))		// ランキングの文字の位置
#define RANKING_WORD_TRANS_POS			(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 40.0f, 0.0f))		// ランキングの自動遷移後の文字の位置
#define RANKING_WORD_SIZE				(D3DXVECTOR3(120.0f, 40.0f, 0.0f))		// ランキングの文字のサイズ
#define RANKING_WORD_TEXTURE			"data/TEXTURE/RankingWord.png"			// ランキングの文字のテクスチャ

//========================
// コンストラクタ
//========================
CRankingWord::CRankingWord() : CObject2D(CObject::TYPE_RANKINGWORD, CObject::PRIORITY_UI)
{

}

//========================
// デストラクタ
//========================
CRankingWord::~CRankingWord()
{

}

//========================
// 初期化処理
//========================
HRESULT CRankingWord::Init(void)
{
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	//// 情報の設定
	//if (CManager::GetClear() == true)
	//{ // クリア後の場合

		// 位置
		SetPos(RANKING_WORD_POS);
	//}
	//else
	//{ // 上記以外

	//	// 位置
	//	SetPos(RANKING_WORD_TRANS_POS);
	//}
	SetPosOld(GetPos());			// 前回の位置
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// 向き
	SetSize(RANKING_WORD_SIZE);				// サイズ
	SetAngle();								// 方向
	SetLength();							// 長さ

	SetVertex();							// 頂点座標の設定処理

	// テクスチャの割り当て処理
	BindTexture(CManager::Get()->GetTexture()->Regist(RANKING_WORD_TEXTURE));

	// 成功を返す
	return S_OK;
}

//========================
// 終了処理
//========================
void CRankingWord::Uninit(void)
{
	// 破棄処理
	CObject2D::Uninit();
}

//========================
// 更新処理
//========================
void CRankingWord::Update(void)
{

}

//========================
// 描画処理
//========================
void CRankingWord::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}

//========================
// 生成処理
//========================
CRankingWord* CRankingWord::Create(void)
{
	// ローカルオブジェクトを生成
	CRankingWord* pScore = nullptr;			// プレイヤーのインスタンスを生成

	if (pScore == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pScore = new CRankingWord;
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