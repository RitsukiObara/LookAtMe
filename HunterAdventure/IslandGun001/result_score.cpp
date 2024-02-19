//============================================
//
// リザルトゲームのスコア処理[result_score.cpp]
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

//--------------------------------------------
// 定数定義
//--------------------------------------------
namespace
{
	// 数字関係
	const char* NUMBER_TEXTURE = "data\\TEXTURE\\Number.png";			// 数字のテクスチャ
	const D3DXVECTOR3 SCORE_SIZE = D3DXVECTOR3(60.0f, 80.0f, 0.0f);		// サイズ
	const D3DXVECTOR3 SCORE_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f - SCORE_SIZE.x * 7, SCREEN_HEIGHT * 0.5f, 0.0f);	// 位置
	const float NUMBER_SHIFT = 120.0f;		// 数字のずらす幅
	const int ADD_DISP = 100;				// 表示スコアの加算数
}

//========================
// コンストラクタ
//========================
CResultScore::CResultScore() : CScore()
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
void CResultScore::SetData(void)
{
	// 情報を設定処理
	CScore::SetData(SCORE_POS, NONE_D3DXVECTOR3, SCORE_SIZE, NUMBER_SHIFT, NUMBER_TEXTURE, ADD_DISP);
}

//========================
// 生成処理
//========================
CResultScore* CResultScore::Create(void)
{
	// スコアのポインタを宣言
	CResultScore* pScore = nullptr;

	if (pScore == nullptr)
	{ // オブジェクトが NULL の場合

		// メモリを確保する
		pScore = new CResultScore;
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