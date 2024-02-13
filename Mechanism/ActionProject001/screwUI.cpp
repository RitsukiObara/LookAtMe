//============================================
//
// ネジUI処理[screwUI.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "screwUI.h"
#include "number.h"
#include "object2D.h"
#include "texture.h"
#include "useful.h"

#include "player.h"

// マクロ定義
#define BACK_TEXTURE		"data\\TEXTURE\\ScrewUI.png"							// 背景のテクスチャ
#define SCORE_TEXTURE		"data\\TEXTURE\\Number.png"								// スコアのテクスチャ
#define BACK_POS			(D3DXVECTOR3(100.0f, 150.0f, 0.0f))						// 背景の位置
#define SCORE_POS			(D3DXVECTOR3(BACK_POS.x, BACK_POS.y + 10.0f, 0.0f))		// 数値の位置
#define SCORE_SHIFT			(40.0f)													// 数値のずらす幅
#define BACK_SIZE			(D3DXVECTOR3(80.0f, 60.0f, 0.0f))						// 背景のサイズ
#define SCORE_SIZE			(D3DXVECTOR3(20.0f, 30.0f, 0.0f))						// 数値のサイズ
#define ADD_LIFE_SCORE		(100)													// 体力が増えるスコア

//========================
// コンストラクタ
//========================
CScrewUI::CScrewUI() : CObject(TYPE_SCREWUI, PRIORITY_UI)
{
	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < MAX_SCREW_DIGIT; nCnt++)
	{
		// 数字の情報
		m_apNumber[nCnt] = nullptr;
	}
	m_pBack = nullptr;	// 背景
	m_nScore = 0;		// スコア
}

//========================
// デストラクタ
//========================
CScrewUI::~CScrewUI()
{

}

//========================
// 初期化処理
//========================
HRESULT CScrewUI::Init(void)
{
	// 全ての値を初期化する
	m_nScore = 0;		// スコア
 
	// スコアの生成処理
	if (FAILED(CreateScore()))
	{ // 失敗した場合
		
		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 背景の生成処理
	if (FAILED(CreateBack()))
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
void CScrewUI::Uninit(void)
{
	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < MAX_SCREW_DIGIT; nCnt++)
	{
		// 数字の終了処理
		m_apNumber[nCnt]->Uninit();
	}

	// 背景の終了処理
	m_pBack->Uninit();

	// 数字の終了処理
	Release();
}

//========================
// 更新処理
//========================
void CScrewUI::Update(void)
{
	// 計算処理
	Calculate();

	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < MAX_SCREW_DIGIT; nCnt++)
	{
		// テクスチャ座標の設定処理
		m_apNumber[nCnt]->SetVtxTextureAnim(NUMBER_TEXTURE_PATTERN, m_apNumber[nCnt]->GetNumber());
	}
}

//========================
// 描画処理
//========================
void CScrewUI::Draw(void)
{
	if (m_pBack != nullptr)
	{ // 背景が NULL じゃない場合

		// 描画処理
		m_pBack->Draw();
	}

	for (int nCnt = 0; nCnt < MAX_SCREW_DIGIT; nCnt++)
	{
		if (m_apNumber[nCnt] != nullptr)
		{ // 数字が NULL じゃない場合

			// 描画処理
			m_apNumber[nCnt]->Draw();
		}
	}
}

//========================
// 情報の設定処理
//========================
void CScrewUI::SetData(void)
{
	// スコアの情報の設定処理
	SetDataScore();

	// 背景の情報の設定処理
	SetDataBack();
}

//========================
// スコアの情報の設定処理
//========================
void CScrewUI::SetDataScore(void)
{
	for (int nCnt = 0; nCnt < MAX_SCREW_DIGIT; nCnt++)
	{
		// 設定処理
		m_apNumber[nCnt]->SetPos(D3DXVECTOR3(SCORE_POS.x + (nCnt * SCORE_SHIFT), SCORE_POS.y, 0.0f));				// 位置設定
		m_apNumber[nCnt]->SetRot(NONE_D3DXVECTOR3);		// 向き設定
		m_apNumber[nCnt]->SetSize(SCORE_SIZE);			// サイズ設定
		m_apNumber[nCnt]->SetLength();					// 長さ設定
		m_apNumber[nCnt]->SetAngle();					// 方向設定

		m_apNumber[nCnt]->SetType(CNumber::TYPE_DECIMAL);	// 番号の種類
		m_apNumber[nCnt]->SetNumber(0);					// 番号を設定する

		// 頂点情報の設定処理
		m_apNumber[nCnt]->SetVertex();

		// テクスチャの設定処理(アニメーションバージョン)
		m_apNumber[nCnt]->SetVtxTextureAnim(NUMBER_TEXTURE_PATTERN, 0);
	}
}

//========================
// 背景の情報の設定処理
//========================
void CScrewUI::SetDataBack(void)
{
	// 設定処理
	m_pBack->SetPos(BACK_POS);				// 位置設定
	m_pBack->SetRot(NONE_D3DXVECTOR3);		// 向き設定
	m_pBack->SetSize(BACK_SIZE);			// サイズ設定
	m_pBack->SetLength();					// 長さ設定
	m_pBack->SetAngle();					// 方向設定

	// 頂点情報の設定処理
	m_pBack->SetVertex();
}

//========================
// スコアの生成処理
//========================
HRESULT CScrewUI::CreateScore(void)
{
	// メモリを確保する
	for (int nCnt = 0; nCnt < MAX_SCREW_DIGIT; nCnt++)
	{
		if (m_apNumber[nCnt] == nullptr)
		{ // ポインタが NULL の場合

			// メモリを確保する
			m_apNumber[nCnt] = new CNumber(CObject::TYPE_NONE, CObject::PRIORITY_UI);
		}
		else
		{ // ポインタが NULL じゃない場合

			// 停止
			assert(false);

			// 失敗を返す
			return E_FAIL;
		}

		if (m_apNumber[nCnt] != nullptr)
		{ // ポインタが NULL じゃない場合

			// 初期化処理
			if (FAILED(m_apNumber[nCnt]->Init()))
			{ // 初期化処理に失敗した場合

				// 停止
				assert(false);

				// 失敗を返す
				return E_FAIL;
			}

			// テクスチャの割り当て処理
			m_apNumber[nCnt]->BindTexture(CManager::Get()->GetTexture()->Regist(SCORE_TEXTURE));
		}
		else
		{ // ポインタが　NULL の場合

		  // 停止
			assert(false);

			// 失敗を返す
			return E_FAIL;
		}
	}

	// 成功を返す
	return S_OK;
}

//========================
// 背景の生成処理
//========================
HRESULT CScrewUI::CreateBack(void)
{
	if (m_pBack == nullptr)
	{ // ポインタが NULL の場合

		// メモリを確保する
		m_pBack = new CObject2D(CObject::TYPE_NONE, CObject::PRIORITY_UI);
	}
	else
	{ // ポインタが NULL じゃない場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	if (m_pBack != nullptr)
	{ // ポインタが NULL じゃない場合

		// 初期化処理
		if (FAILED(m_pBack->Init()))
		{ // 初期化処理に失敗した場合

			// 停止
			assert(false);

			// 失敗を返す
			return E_FAIL;
		}

		// テクスチャの割り当て処理
		m_pBack->BindTexture(CManager::Get()->GetTexture()->Regist(BACK_TEXTURE));
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
// 計算処理
//========================
void CScrewUI::Calculate(void)
{
	// ローカル変数宣言
	int aNum[MAX_SCREW_DIGIT];		// 数値

	// 10進数への計算処理
	useful::DecimalCalculation(MAX_SCREW_DIGIT, m_nScore, &aNum[0]);

	for (int nCnt = 0; nCnt < MAX_SCREW_DIGIT; nCnt++)
	{
		//値の計算
		m_apNumber[nCnt]->SetNumber(aNum[nCnt]);
	}
}

//========================
// スコアの設定処理
//========================
void CScrewUI::SetScore(const int nScore)
{
	// スコアを代入する
	m_nScore = nScore;
}

//========================
// スコアの取得処理
//========================
int CScrewUI::GetScore(void) const
{
	// スコアを返す
	return m_nScore;
}

//========================
// スコアの加算処理
//========================
void CScrewUI::AddScore(const int nScore)
{
	// スコアを設定する
	m_nScore += nScore;

	if (m_nScore >= ADD_LIFE_SCORE)
	{ // スコアが一定数に達した場合

		// スコアを初期化する
		m_nScore = 0;

		// 体力を1増やす
		CPlayer::Get()->SetLife(CPlayer::Get()->GetLife() + 1);
	}
}

//========================
// 生成処理
//========================
CScrewUI* CScrewUI::Create(void)
{
	// ローカルオブジェクトを生成
	CScrewUI* pScrew = nullptr;	// プレイヤーのインスタンスを生成

	if (pScrew == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pScrew = new CScrewUI;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pScrew != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pScrew->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pScrew->SetData();
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// ネジUIのポインタを返す
	return pScrew;
}