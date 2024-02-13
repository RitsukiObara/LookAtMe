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

#include "object2D.h"

//--------------------------------------------
// 定数定義
//--------------------------------------------
namespace
{
	// 数字関係
	const char* NUMBER_TEXTURE = "data\\TEXTURE\\Number.png";			// 数字のテクスチャ
	const D3DXVECTOR3 SCORE_POS = D3DXVECTOR3(1050.0f, 25.0f, 0.0f);	// 位置
	const D3DXVECTOR3 SCORE_SIZE = D3DXVECTOR3(15.0f, 20.0f, 0.0f);		// サイズ
	const float NUMBER_SHIFT = 30.0f;		// 数字のずらす幅
	const int ADD_DISP = 20;				// 表示スコアの加算数

	// 文字関係
	const char* WORD_TEXTURE = "data\\TEXTURE\\ScoreWord.png";			// 文字のテクスチャ
	const D3DXVECTOR3 WORD_POS = D3DXVECTOR3(950.0f, 25.0f, 0.0f);		// 文字の位置
	const D3DXVECTOR3 WORD_SIZE = D3DXVECTOR3(80.0f, 20.0f, 0.0f);		// 文字のサイズ
}

//========================
// コンストラクタ
//========================
CGameScore::CGameScore() : CScore()
{
	// 全ての値をクリアする
	m_pScoreWord = nullptr;		// 文字の情報
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
	if (m_pScoreWord != nullptr)
	{ // 文字の情報が NULL じゃない場合

		// スコアの終了処理
		m_pScoreWord->Uninit();
		m_pScoreWord = nullptr;
	}

	// 終了処理
	CScore::Uninit();
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
	if (m_pScoreWord != nullptr)
	{ // 文字が NULL じゃない場合

		// 文字の描画処理
		m_pScoreWord->Draw();
	}

	// 描画処理
	CScore::Draw();
}

//========================
// 情報の設定処理
//========================
void CGameScore::SetData(void)
{
	// 情報を設定処理
	CScore::SetData(SCORE_POS, NONE_D3DXVECTOR3, SCORE_SIZE, NUMBER_SHIFT, NUMBER_TEXTURE, ADD_DISP);

	// 全ての値を設定する
	if (m_pScoreWord == nullptr)
	{ // 文字が NULL の場合

		// 文字を生成
		m_pScoreWord = CObject2D::Create(CObject2D::TYPE_NONE, TYPE_NONE, PRIORITY_UI);
		
		// 情報の設定
		m_pScoreWord->SetPos(WORD_POS);				// 位置
		m_pScoreWord->SetPosOld(WORD_POS);			// 前回の位置
		m_pScoreWord->SetRot(NONE_D3DXVECTOR3);		// 向き
		m_pScoreWord->SetSize(WORD_SIZE);			// サイズ
	
		// テクスチャの割り当て処理
		m_pScoreWord->BindTexture(CManager::Get()->GetTexture()->Regist(WORD_TEXTURE));

		// サイズの設定処理
		m_pScoreWord->SetVertex();
	}

}

//========================
// 生成処理
//========================
CGameScore* CGameScore::Create(void)
{
	// スコアのポインタを宣言
	CGameScore* pScore = nullptr;

	if (pScore == nullptr)
	{ // オブジェクトが NULL の場合

		// メモリを確保する
		pScore = new CGameScore;
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
		pScore->SetData();
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