//============================================
//
// 体力UI処理[lifeUI.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "lifeUI.h"
#include "object2D.h"
#include "texture.h"
#include "useful.h"

#include "player.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define BACK_TEXTURE		"data\\TEXTURE\\LifeUIBack.png"			// 背景のテクスチャ
#define LIFE_TEXTURE		"data\\TEXTURE\\LifeUIMark.png"			// 体力のテクスチャ
#define BACK_POS			(D3DXVECTOR3(170.0f, 50.0f, 0.0f))		// 背景の位置
#define LIFE_POS			(D3DXVECTOR3(80.0f, 50.0f, 0.0f))		// 体力の位置
#define BACK_SIZE			(D3DXVECTOR3(160.0f, 40.0f, 0.0f))		// 背景のサイズ
#define LIFE_SIZE			(D3DXVECTOR3(30.0f, 30.0f, 0.0f))		// 体力のサイズ
#define LIFE_SHIFT			(90.0f)									// 体力のずらす幅

//========================
// コンストラクタ
//========================
CLifeUI::CLifeUI() : CObject(TYPE_LIFEUI, PRIORITY_UI)
{
	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < MAX_LIFE_POLYGON; nCnt++)
	{
		// 数字の情報
		m_apLife[nCnt] = nullptr;
	}
	m_pBack = nullptr;	// 背景
}

//========================
// デストラクタ
//========================
CLifeUI::~CLifeUI()
{

}

//========================
// 初期化処理
//========================
HRESULT CLifeUI::Init(void)
{
	if (FAILED(CreateBack()))
	{ // 生成に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	if (FAILED(CreateLife()))
	{ // 生成に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//========================
// 終了処理
//========================
void CLifeUI::Uninit(void)
{
	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < MAX_LIFE_POLYGON; nCnt++)
	{
		// 体力の終了処理
		m_apLife[nCnt]->Uninit();
	}

	// 背景の終了処理
	m_pBack->Uninit();

	// 数字の終了処理
	Release();
}

//========================
// 更新処理
//========================
void CLifeUI::Update(void)
{
	// プレイヤーの体力を取得する
	int nLife = CPlayer::Get()->GetLife();

	for (int nCnt = 0; nCnt < MAX_LIFE_POLYGON; nCnt++)
	{
		if (nLife > nCnt)
		{ // 体力があるのを示すポリゴンの場合

			// 頂点カラーの設定処理
			m_apLife[nCnt]->SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{ // 上記以外

			// 頂点カラーの設定処理
			m_apLife[nCnt]->SetVtxColor(D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));
		}
	}
}

//========================
// 描画処理
//========================
void CLifeUI::Draw(void)
{
	if (m_pBack != nullptr)
	{ // 背景が NULL じゃない場合

		// 描画処理
		m_pBack->Draw();
	}

	for (int nCnt = 0; nCnt < MAX_LIFE_POLYGON; nCnt++)
	{
		if (m_apLife[nCnt] != nullptr)
		{ // 数字が NULL じゃない場合

			// 描画処理
			m_apLife[nCnt]->Draw();
		}
	}
}

//========================
// 情報の設定処理
//========================
void CLifeUI::SetData(void)
{
	// 体力の情報の設定処理
	SetDataLife();

	// 背景の情報の設定処理
	SetDataBack();
}

//========================
// 背景の情報の設定処理
//========================
void CLifeUI::SetDataBack(void)
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
// 体力の生成処理
//========================
void CLifeUI::SetDataLife(void)
{
	for (int nCnt = 0; nCnt < MAX_LIFE_POLYGON; nCnt++)
	{
		// 設定処理
		m_apLife[nCnt]->SetPos(D3DXVECTOR3(LIFE_POS.x + (nCnt * LIFE_SHIFT), LIFE_POS.y, 0.0f));		// 位置設定
		m_apLife[nCnt]->SetRot(NONE_D3DXVECTOR3);	// 向き設定
		m_apLife[nCnt]->SetSize(LIFE_SIZE);			// サイズ設定
		m_apLife[nCnt]->SetLength();				// 長さ設定
		m_apLife[nCnt]->SetAngle();					// 方向設定

		// 頂点情報の設定処理
		m_apLife[nCnt]->SetVertex();
	}
}

//========================
// 背景の生成処理
//========================
HRESULT CLifeUI::CreateBack(void)
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
// 体力の生成処理
//========================
HRESULT CLifeUI::CreateLife(void)
{
	// メモリを確保する
	for (int nCnt = 0; nCnt < MAX_LIFE_POLYGON; nCnt++)
	{
		if (m_apLife[nCnt] == nullptr)
		{ // ポインタが NULL の場合

			// メモリを確保する
			m_apLife[nCnt] = new CObject2D(CObject::TYPE_NONE, CObject::PRIORITY_UI);
		}
		else
		{ // ポインタが NULL じゃない場合

			// 停止
			assert(false);

			// 失敗を返す
			return E_FAIL;
		}

		if (m_apLife[nCnt] != nullptr)
		{ // ポインタが NULL じゃない場合

			// 初期化処理
			if (FAILED(m_apLife[nCnt]->Init()))
			{ // 初期化処理に失敗した場合

				// 停止
				assert(false);

				// 失敗を返す
				return E_FAIL;
			}

			// テクスチャの割り当て処理
			m_apLife[nCnt]->BindTexture(CManager::Get()->GetTexture()->Regist(LIFE_TEXTURE));
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
// 生成処理
//========================
CLifeUI* CLifeUI::Create(void)
{
	// ローカルオブジェクトを生成
	CLifeUI* pLife = nullptr;	// プレイヤーのインスタンスを生成

	if (pLife == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pLife = new CLifeUI;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pLife != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pLife->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pLife->SetData();
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 体力UIのポインタを返す
	return pLife;
}