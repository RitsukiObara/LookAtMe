//============================================
//
// タイトルロゴ処理[title_logo.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "title_logo.h"
#include "texture.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define LOGO_TEXTURE		"data/TEXTURE/TitleLogo.png"							// タイトルロゴのテクスチャ
#define LOGO_POS			(D3DXVECTOR3(-450.0f, SCREEN_HEIGHT * 0.5f, 0.0f))		// タイトルロゴの位置
#define LOGO_SIZE			(D3DXVECTOR3(400.0f, 100.0f, 0.0f))						// タイトルロゴのサイズ
#define LOGO_SPEED			(20.0f)													// タイトルロゴの速度

//--------------------------------------------
// 静的メンバ変数宣言
//--------------------------------------------
CTitleLogo* CTitleLogo::m_pTitleLogo = nullptr;		// タイトルロゴのポインタ

//============================
// コンストラクタ
//============================
CTitleLogo::CTitleLogo() : CObject2D(CObject::TYPE_TITLELOGO, PRIORITY_UI)
{
	// 全ての値をクリアする
	m_state = STATE_MOVE;		// 状態
}

//============================
// デストラクタ
//============================
CTitleLogo::~CTitleLogo()
{

}

//============================
// 初期化処理
//============================
HRESULT CTitleLogo::Init(void)
{
	if (FAILED(CObject2D::Init()))
	{ // 失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_state = STATE_MOVE;		// 状態

	// 成功を返す
	return S_OK;
}

//============================
// 終了処理
//============================
void CTitleLogo::Uninit(void)
{
	// 終了処理
	CObject2D::Uninit();

	// タイトルロゴを NULL にする
	m_pTitleLogo = nullptr;
}

//============================
// 更新処理
//============================
void CTitleLogo::Update(void)
{
	switch (m_state)
	{
	case CTitleLogo::STATE_MOVE:

	{
		D3DXVECTOR3 pos = GetPos();

		pos.x += LOGO_SPEED;

		SetPos(pos);
	}

	if (GetPos().x >= SCREEN_WIDTH * 0.5f)
	{ // 一定の位置を過ぎた場合

		// 停止状態にする
		m_state = STATE_STOP;
	}

		break;

	case CTitleLogo::STATE_STOP:


		break;

	default:

		// 停止
		assert(false);

		break;
	}

	// 頂点情報の設定処理
	SetVertex();
}

//============================
// 描画処理
//============================
void CTitleLogo::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}

//============================
// 情報の設定処理
//============================
void CTitleLogo::SetData(void)
{
	// 情報を設定する
	SetPos(LOGO_POS);			// 位置
	SetPosOld(LOGO_POS);		// 前回の位置
	SetRot(NONE_D3DXVECTOR3);	// 向き
	SetSize(LOGO_SIZE);			// サイズ
	SetAngle();					// 方向
	SetLength();				// 長さ

	// テクスチャの設定処理
	BindTexture(CManager::Get()->GetTexture()->Regist(LOGO_TEXTURE));

	// 頂点情報処理
	SetVertex();

	// 全ての値を設定する
	m_state = STATE_MOVE;		// 状態
}

//============================
// 取得処理
//============================
CTitleLogo* CTitleLogo::Get(void)
{
	if (m_pTitleLogo != nullptr)
	{ // タイトルロゴの情報がある場合

		// タイトルロゴのポインタを返す
		return m_pTitleLogo;
	}
	else
	{ // 上記以外

		// 停止
		assert(false);

		// タイトルロゴのポインタを返す
		return m_pTitleLogo;
	}
}

//============================
//生成処理
//============================
CTitleLogo* CTitleLogo::Create(void)
{
	if (m_pTitleLogo == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		m_pTitleLogo = new CTitleLogo;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (m_pTitleLogo != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(m_pTitleLogo->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		m_pTitleLogo->SetData();
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// タイトルロゴのポインタを返す
	return m_pTitleLogo;
}