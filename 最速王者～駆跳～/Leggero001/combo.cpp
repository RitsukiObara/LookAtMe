//============================================
//
// 褒め言葉UI処理[praiseUI.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "game.h"
#include "object2D.h"
#include "number.h"
#include "combo.h"
#include "input.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define WORD_TEXTURE		"data/TEXTURE/Combo.png"				// コンボの文字のテクスチャ
#define MULTI_TEXTURE		"data/TEXTURE/Multi.png"				// ×マークのテクスチャ
#define NUMBER_TEXTURE		"data/TEXTURE/Number.png"				// 数字のテクスチャ
#define WORD_POS			(D3DXVECTOR3(1050.0f, 50.0f, 0.0f))		// 言葉の位置
#define WORD_SIZE			(D3DXVECTOR3(60.0f, 30.0f, 0.0f))		// 言葉のサイズ
#define MULTI_POS			(D3DXVECTOR3(1130.0f, 50.0f, 0.0f))		// ×マークの位置
#define MULTI_SIZE			(D3DXVECTOR3(20.0f, 20.0f, 0.0f))		// ×マークのサイズ
#define NUMBER_POS			(D3DXVECTOR3(1160.0f, 50.0f, 0.0f))		// 数字の位置
#define NUMBER_SIZE			(D3DXVECTOR3(10.0f, 20.0f, 0.0f))		// 数字のサイズ
#define NUMBER_SHIFT		(20.0f)									// 数字のずらす幅
#define MAX_COMBO_VALUE		(999)									// コンボの最大値

//============================
// コンストラクタ
//============================
CCombo::CCombo() : CObject(CObject::TYPE_PRAISEUI, PRIORITY_UI)
{
	// 全ての値をクリアする
	for (int nCntObj = 0; nCntObj < TYPE_MAX; nCntObj++)
	{
		m_apObject2D[nCntObj] = nullptr;		// 2Dポリゴンの情報
	}

	for (int nCntNum = 0; nCntNum < COMBO_DIGIT; nCntNum++)
	{
		m_aNumber[nCntNum].pNumber = nullptr;	// 数字の情報
		m_aNumber[nCntNum].bDisp = false;		// 描画状況
	}
	m_nCombo = 0;								// コンボ数
}

//============================
// デストラクタ
//============================
CCombo::~CCombo()
{

}

//============================
// 初期化処理
//============================
HRESULT CCombo::Init(void)
{
	// 全ての値をクリアする
	for (int nCntObj = 0; nCntObj < TYPE_MAX; nCntObj++)
	{
		m_apObject2D[nCntObj] = m_apObject2D[nCntObj]->Create(CObject2D::TYPE_NONE, TYPE_NONE, PRIORITY_UI);		// 2Dポリゴンの情報

		if (m_apObject2D[nCntObj] != nullptr)
		{ // ポインタが NULL じゃない場合

			switch (nCntObj)
			{
			case TYPE_WORD:		// 言葉

				// 情報を設定する
				m_apObject2D[nCntObj]->SetPos(WORD_POS);						// 位置
				m_apObject2D[nCntObj]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// 向き
				m_apObject2D[nCntObj]->SetSize(WORD_SIZE);						// サイズ

				// テクスチャの割り当て処理
				m_apObject2D[nCntObj]->BindTexture(CManager::GetTexture()->Regist(WORD_TEXTURE));

				break;

			case TYPE_MULTI:	// ×マーク

				// 情報を設定する
				m_apObject2D[nCntObj]->SetPos(MULTI_POS);						// 位置
				m_apObject2D[nCntObj]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// 向き
				m_apObject2D[nCntObj]->SetSize(MULTI_SIZE);						// サイズ

				// テクスチャの割り当て処理
				m_apObject2D[nCntObj]->BindTexture(CManager::GetTexture()->Regist(MULTI_TEXTURE));

				break;
			}

			m_apObject2D[nCntObj]->SetAngle();		// 方向
			m_apObject2D[nCntObj]->SetLength();		// 長さ

			m_apObject2D[nCntObj]->SetVertex();		// 頂点情報の設定処理
		}
	}

	for (int nCntNum = 0; nCntNum < COMBO_DIGIT; nCntNum++)
	{
		// 数字の生成処理
		m_aNumber[nCntNum].pNumber = new CNumber(TYPE_NONE, PRIORITY_UI);

		if (m_aNumber[nCntNum].pNumber != nullptr)
		{ // 数字のポインタが NULL じゃない場合

			if (SUCCEEDED(m_aNumber[nCntNum].pNumber->Init()))
			{ // 数字の初期化に成功した場合

				// 情報の設定処理
				m_aNumber[nCntNum].pNumber->SetPos(D3DXVECTOR3(NUMBER_POS.x + (NUMBER_SHIFT * nCntNum), NUMBER_POS.y, NUMBER_POS.z));			// 位置
				m_aNumber[nCntNum].pNumber->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// 向き
				m_aNumber[nCntNum].pNumber->SetSize(NUMBER_SIZE);		// サイズ
				m_aNumber[nCntNum].pNumber->SetAngle();		// 方向
				m_aNumber[nCntNum].pNumber->SetLength();	// 長さ

				m_aNumber[nCntNum].pNumber->SetNumber(0);	// 数字
				m_aNumber[nCntNum].pNumber->SetType(CNumber::TYPE_DECIMAL);		// 種類

				m_aNumber[nCntNum].pNumber->SetVertex();	// 頂点情報の設定処理

				// テクスチャの割り当て処理
				m_aNumber[nCntNum].pNumber->BindTexture(CManager::GetTexture()->Regist(NUMBER_TEXTURE));

				m_aNumber[nCntNum].bDisp = false;			// 描画状況
			}
		}
	}

	m_nCombo = 0;								// コンボ数

	// 成功を返す
	return S_OK;
}

//============================
// 終了処理
//============================
void CCombo::Uninit(void)
{
	for (int nCntObj = 0; nCntObj < TYPE_MAX; nCntObj++)
	{
		if (m_apObject2D[nCntObj] != nullptr)
		{ // ポインタが NULL じゃない場合

			// 終了処理
			m_apObject2D[nCntObj]->Uninit();
			m_apObject2D[nCntObj] = nullptr;
		}
	}

	for (int nCntNum = 0; nCntNum < COMBO_DIGIT; nCntNum++)
	{
		if (m_aNumber[nCntNum].pNumber != nullptr)
		{ // ポインタが NULL じゃない場合

			// 終了処理
			m_aNumber[nCntNum].pNumber->Uninit();
			m_aNumber[nCntNum].pNumber = nullptr;
		}
	}

	// 破棄処理
	Release();
}

//============================
// 更新処理
//============================
void CCombo::Update(void)
{
	// 描画判定処理
	DispDecide();

	// 計算処理
	Calculation();

	// 全ての値を初期化する
	for (int nCntNum = 0; nCntNum < COMBO_DIGIT; nCntNum++)
	{
		// テクスチャ座標の設定処理
		m_aNumber[nCntNum].pNumber->SetVtxTextureAnim(NUMBER_TEXTURE_PATTERN, m_aNumber[nCntNum].pNumber->GetNumber());
	}

	if (CManager::GetInputKeyboard()->GetPress(DIK_0) == true)
	{
		// コンボ数を加算する
		m_nCombo++;
	}
}

//============================
// 描画処理
//============================
void CCombo::Draw(void)
{
	for (int nCntObj = 0; nCntObj < TYPE_MAX; nCntObj++)
	{
		if (m_apObject2D[nCntObj] != nullptr)
		{ // ポインタが NULL じゃない場合

			// 終了処理
			m_apObject2D[nCntObj]->Draw();
		}
	}

	for (int nCntNum = 0; nCntNum < COMBO_DIGIT; nCntNum++)
	{
		if (m_aNumber[nCntNum].pNumber != nullptr &&
			m_aNumber[nCntNum].bDisp == true)
		{ // ポインタが NULL じゃない場合

			// 終了処理
			m_aNumber[nCntNum].pNumber->Draw();
		}
	}
}

//============================
// 計算処理
//============================
void CCombo::Calculation(void)
{
	// ローカル変数宣言
	int nNum[COMBO_DIGIT];

	// 10進数の計算処理
	DecimalCalculation(COMBO_DIGIT, m_nCombo, &nNum[0]);

	for (int nCnt = 0; nCnt < COMBO_DIGIT; nCnt++)
	{
		// 数字を設定する
		m_aNumber[nCnt].pNumber->SetNumber(nNum[nCnt]);
	}
}

//============================
// 描画判定処理
//============================
void CCombo::DispDecide(void)
{
	if (m_nCombo >= MAX_COMBO_VALUE)
	{ // コンボの最大値を超えた場合

		// 補正する
		m_nCombo = MAX_COMBO_VALUE;
	}

	for (int nCnt = 0; nCnt < COMBO_DIGIT; nCnt++)
	{
		if (m_nCombo >= pow(10, (COMBO_DIGIT - (nCnt + 1))))
		{ // コンボ数が一定数を超えていなかった場合

			// 描画する
			m_aNumber[nCnt].bDisp = true;
		}
		else
		{ // 上記以外

			// 描画しない
			m_aNumber[nCnt].bDisp = false;
		}
	}
}

//============================
//生成処理
//============================
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

		// NULL を返す
		return nullptr;
	}

	if (pCombo != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pCombo->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "コンボの初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pCombo;
}