//============================================
//
// コンボ処理[combo.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "number.h"
#include "combo.h"
#include "combo_circle.h"
#include "combo_score.h"
#include "combo_magni.h"
#include "game.h"
#include "game_score.h"
#include "texture.h"
#include "useful.h"

// マクロ定義
#define LIMIT_POS			(D3DXVECTOR3(1080.0f, 70.0f, 0.0f))			// コンボ制限の位置
#define LIMIT_SIZE			(40.0f)										// コンボ制限のサイズ
#define LIMIT_DISTANCE		(25.0f)										// コンボ制限の距離
#define LIMIT_DIVI			(96)										// コンボ制限の分割数

#define COMBO_POS			(D3DXVECTOR3(900.0f, 70.0f, 0.0f))			// コンボの数値の位置
#define COMBO_SIZE			(D3DXVECTOR3(20.0f, 30.0f, 0.0f))			// コンボの数値のサイズ
#define COMBO_SHIFT			(40.0f)										// コンボの数値のずらす幅

#define SCORE_POS			(D3DXVECTOR3(1050.0f, 40.0f, 0.0f))			// スコアの位置
#define SCORE_SIZE			(D3DXVECTOR3(10.0f, 15.0f, 0.0f))			// スコアのサイズ
#define SCORE_SHIFT			(20.0f)										// スコアのずらす幅

//========================
// コンストラクタ
//========================
CCombo::CCombo() : CObject(TYPE_COMBO, PRIORITY_UI)
{
	// 全ての値をクリアする
	m_pMagni = nullptr;			// コンボの倍率
	m_pTimeLimit = nullptr;		// コンボの制限
	m_pScore = nullptr;			// コンボスコア
}

//========================
// デストラクタ
//========================
CCombo::~CCombo()
{

}

//========================
// 初期化処理
//========================
HRESULT CCombo::Init(void)
{ 
	// コンボの生成処理
	if (FAILED(CreateCombo()))
	{ // 失敗した場合
		
		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// コンボ制限の生成処理
	if (FAILED(CreateLimit()))
	{ // 失敗した場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// コンボスコアの生成処理
	if (FAILED(CreateComboScore()))
	{ // 失敗した場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//========================
// 終了処理
//========================
void CCombo::Uninit(void)
{
	// コンボ倍率の終了処理
	m_pMagni->Uninit();

	// コンボ制限の終了処理
	m_pTimeLimit->Uninit();

	// コンボスコアの終了処理
	m_pScore->Uninit();

	// 数字の終了処理
	Release();
}

//========================
// 更新処理
//========================
void CCombo::Update(void)
{
	// コンボ倍率の更新処理
	m_pMagni->Update();

	// コンボの制限円の更新処理
	m_pTimeLimit->Update();

	// コンボスコアの更新処理
	m_pScore->Update();

	if (m_pTimeLimit->GetEnd() == true)
	{ // 終了状況にする

		// スコアを加算する
		CGameScore::Get()->AddScore(m_pMagni->GetMagni() * m_pScore->GetScore());

		// スコアを初期化する
		m_pScore->SetScore(0);

		// 倍率を初期化する
		m_pMagni->SetMagni(0);
	}
}

//========================
// 描画処理
//========================
void CCombo::Draw(void)
{
	if (m_pTimeLimit != nullptr)
	{ // コンボ制限が NULL じゃない場合

		// 描画処理
		m_pTimeLimit->Draw();
	}

	if (m_pScore != nullptr)
	{ // コンボスコアが NULL じゃない場合

		// 描画処理
		m_pScore->Draw();
	}

	if (m_pMagni != nullptr)
	{ // コンボ倍率が NULL じゃない場合

		// 描画処理
		m_pMagni->Draw();
	}
}

//========================
// コンボの加算処理
//========================
void CCombo::AddCombo(const int nScore)
{
	// 倍率の加算処理
	m_pMagni->AddMagni();

	// 制限時間のリセット処理
	m_pTimeLimit->ResetNumVtx();

	// スコアの加算処理
	m_pScore->AddScore(nScore);
}

//========================
// 生成処理
//========================
CCombo* CCombo::Create(void)
{
	// ローカルオブジェクトを生成
	CCombo* pCombo = nullptr;	// プレイヤーのインスタンスを生成

	if (pCombo == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pCombo = new CCombo;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pCombo != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pCombo->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// コンボのポインタを返す
	return pCombo;
}

//========================
// コンボの生成処理
//========================
HRESULT CCombo::CreateCombo(void)
{
	if (m_pMagni == nullptr)
	{ // ポインタが NULL の場合

		// メモリを確保する
		m_pMagni = CComboMagni::Create(COMBO_POS, NONE_D3DXVECTOR3, COMBO_SIZE, COMBO_SHIFT);
	}
	else
	{ // ポインタが NULL じゃない場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//========================
// コンボ制限の生成処理
//========================
HRESULT CCombo::CreateLimit(void)
{
	if (m_pTimeLimit == nullptr)
	{ // ポインタが NULL の場合

		// メモリを確保する
		m_pTimeLimit = CComboCircle::Create(LIMIT_POS, LIMIT_SIZE, LIMIT_DISTANCE, LIMIT_DIVI);
	}
	else
	{ // ポインタが NULL じゃない場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	if (m_pTimeLimit != nullptr)
	{ // ポインタが NULL じゃない場合

		// テクスチャの割り当て処理
		m_pTimeLimit->BindTexture(CManager::Get()->GetTexture()->Regist(nullptr));
	}
	else
	{ // ポインタが　NULL の場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//========================
// コンボスコアの生成処理
//========================
HRESULT CCombo::CreateComboScore(void)
{
	if (m_pScore == nullptr)
	{ // ポインタが NULL の場合

		// メモリを確保する
		m_pScore = CComboScore::Create(LIMIT_POS, NONE_D3DXVECTOR3, SCORE_SIZE, SCORE_SHIFT);
	}
	else
	{ // ポインタが NULL じゃない場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}