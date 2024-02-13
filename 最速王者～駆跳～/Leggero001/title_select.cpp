//============================================
//
// タイトルの選択肢処理[title_select.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "renderer.h"
#include "title_select.h"
#include "texture.h"
#include "useful.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define TITLE_SELECT_TEXTURE		"data/TEXTURE/TitleSelect.png"						// タイトルの選択肢のテクスチャ
#define TITLE_SELECT_POS			(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 500.0f, 0.0f))	// タイトルの選択肢の位置
#define TITLE_SELECT_SIZE			(D3DXVECTOR3(400.0f, 40.0f, 0.0f))					// タイトルの選択肢のサイズ
#define TITLE_SELECT_ALPHA_ADD		(-0.02f)											// タイトルの選択肢の透明度の加算数

//============================
// コンストラクタ
//============================
CTitleSelect::CTitleSelect() : CObject2D(CObject2D::TYPE_LANDUI, PRIORITY_UI)
{
	// 全ての値をクリアする
	m_fAlpha = 1.0f;		// 透明度
	m_fAlphaAdd = 0.0f;		// 透明度の加算数
	m_fAlphaDest = 0.0f;	// 目的の透明度
}

//============================
// デストラクタ
//============================
CTitleSelect::~CTitleSelect()
{

}

//============================
// 初期化処理
//============================
HRESULT CTitleSelect::Init(void)
{
	// 全ての値を初期化する
	m_fAlpha = 1.0f;						// 透明度
	m_fAlphaAdd = TITLE_SELECT_ALPHA_ADD;	// 透明度の加算数
	m_fAlphaDest = 0.0f;					// 目的の透明度

	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を設定する
	SetPos(TITLE_SELECT_POS);				// 位置
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// 向き
	SetSize(TITLE_SELECT_SIZE);				// サイズ設定
	SetLength();							// 長さ設定
	SetAngle();								// 方向設定

	// 頂点情報の初期化
	SetVertex();

	// テクスチャの割り当て処理
	BindTexture(CManager::GetTexture()->Regist(TITLE_SELECT_TEXTURE));

	// 成功を返す
	return S_OK;
}

//============================
// 終了処理
//============================
void CTitleSelect::Uninit(void)
{
	// 終了
	CObject2D::Uninit();
}

//============================
// 更新処理
//============================
void CTitleSelect::Update(void)
{
	// 透明度に加算する
	m_fAlpha += m_fAlphaAdd;

	if (m_fAlpha >= 1.0f ||
		m_fAlpha <= 0.0f)
	{ // 透明度が一定数以内になった場合

		// 透明度の加算数の符号を反転させる
		m_fAlphaAdd *= -1;
	}

	{
		// 色を設定する
		D3DXCOLOR col(1.0f, 1.0f, 1.0f, m_fAlpha);

		// 頂点カラーを設定する
		SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));
	}
}

//============================
// 描画処理
//============================
void CTitleSelect::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}

//============================
//生成処理
//============================
CTitleSelect* CTitleSelect::Create(void)
{
	// ローカルオブジェクトを生成
	CTitleSelect* pTitleSelect = nullptr;	// プレイヤーのインスタンスを生成

	if (pTitleSelect == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pTitleSelect = new CTitleSelect;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pTitleSelect != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pTitleSelect->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "タイトル選択肢の初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// タイトルの選択肢のポインタを返す
	return pTitleSelect;
}