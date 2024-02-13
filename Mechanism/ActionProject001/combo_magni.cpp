//============================================
//
// コンボ倍率処理[combo_magni.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "combo_magni.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define SCORE_TEXTURE		"data\\TEXTURE\\Number.png"		// スコアのテクスチャ

//========================
// コンストラクタ
//========================
CComboMagni::CComboMagni() : CObject(TYPE_NONE, PRIORITY_UI)
{
	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < MAX_COMBO_DIGIT; nCnt++)
	{
		// 数字の情報
		m_apNumber[nCnt] = nullptr;
	}
	m_nMagni = 0;		// コンボ倍率
}

//========================
// デストラクタ
//========================
CComboMagni::~CComboMagni()
{

}

//========================
// 初期化処理
//========================
HRESULT CComboMagni::Init(void)
{
	//ローカル変数宣言
	int nTexIdx = NONE_TEXIDX;		// テクスチャのインデックス

	// テクスチャの読み込み処理
	nTexIdx = CManager::Get()->GetTexture()->Regist(SCORE_TEXTURE);

	// 全ての値を初期化する
	m_nMagni = 0;			// コンボ倍率

	// メモリを確保する
	for (int nCnt = 0; nCnt < MAX_COMBO_DIGIT; nCnt++)
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
			m_apNumber[nCnt]->BindTexture(nTexIdx);
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
// 終了処理
//========================
void CComboMagni::Uninit(void)
{
	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < MAX_COMBO_DIGIT; nCnt++)
	{
		// 終了処理
		m_apNumber[nCnt]->Uninit();
	}

	// 数字の終了処理
	Release();
}

//========================
// 更新処理
//========================
void CComboMagni::Update(void)
{
	// 計算処理
	Calculate();

	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < MAX_COMBO_DIGIT; nCnt++)
	{
		// テクスチャ座標の設定処理
		m_apNumber[nCnt]->SetVtxTextureAnim(NUMBER_TEXTURE_PATTERN, m_apNumber[nCnt]->GetNumber());
	}
}

//========================
// 描画処理
//========================
void CComboMagni::Draw(void)
{
	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < MAX_COMBO_DIGIT; nCnt++)
	{
		// 描画処理
		m_apNumber[nCnt]->Draw();
	}
}

//========================
// 情報の設定処理
//========================
void CComboMagni::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size, const float fShift)
{
	for (int nCnt = 0; nCnt < MAX_COMBO_DIGIT; nCnt++)
	{
		// 設定処理
		m_apNumber[nCnt]->SetPos(D3DXVECTOR3(pos.x + (nCnt * fShift), pos.y, 0.0f));		// 位置設定
		m_apNumber[nCnt]->SetRot(rot);				// 向き設定
		m_apNumber[nCnt]->SetSize(size);			// サイズ設定
		m_apNumber[nCnt]->SetLength();				// 長さ設定
		m_apNumber[nCnt]->SetAngle();				// 方向設定

		m_apNumber[nCnt]->SetType(CNumber::TYPE_DECIMAL);	// 番号の種類
		m_apNumber[nCnt]->SetNumber(0);				// 番号を設定する

		// 頂点情報の設定処理
		m_apNumber[nCnt]->SetVertex();

		// テクスチャの設定処理(アニメーションバージョン)
		m_apNumber[nCnt]->SetVtxTextureAnim(NUMBER_TEXTURE_PATTERN, 0);
	}
}

//========================
// 計算処理
//========================
void CComboMagni::Calculate(void)
{
	// ローカル変数宣言
	int aNum[MAX_COMBO_DIGIT];		// 数値

	// 10進数への計算処理
	useful::DecimalCalculation(MAX_COMBO_DIGIT, m_nMagni, &aNum[0]);

	for (int nCnt = 0; nCnt < MAX_COMBO_DIGIT; nCnt++)
	{
		//値の計算
		m_apNumber[nCnt]->SetNumber(aNum[nCnt]);
	}
}

//========================
// 倍率の設定処理
//========================
void CComboMagni::SetMagni(const int nMagni)
{
	// コンボ倍率を設定する
	m_nMagni = nMagni;
}

//========================
// スコアの取得処理
//========================
int CComboMagni::GetMagni(void) const
{
	// スコアを返す
	return m_nMagni;
}

//========================
// スコアの加算処理
//========================
void CComboMagni::AddMagni(void)
{
	// スコアを設定する
	m_nMagni++;
}

//========================
// 生成処理
//========================
CComboMagni* CComboMagni::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size, const float fShift)
{
	// ローカルオブジェクトを生成
	CComboMagni* pMagni = nullptr;	// プレイヤーのインスタンスを生成

	if (pMagni == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pMagni = new CComboMagni;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pMagni != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pMagni->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pMagni->SetData(pos, rot, size, fShift);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pMagni;
}