//=======================================
//
// 画面エフェクトのメイン処理[screen_effect.cpp]
// Author 小原立暉
//
//=======================================
#include "manager.h"
#include "screen_effect.h"
#include "texture.h"

//---------------------------------------
// 無名名前空間
//---------------------------------------
namespace
{
	const char* TEXTURE[CScreenEffect::TYPE_MAX] =		// テクスチャ
	{
		"data\\TEXTURE\\Platform\\Dmg_Screen.png",		// 周辺赤
		"data\\TEXTURE\\Platform\\Blood_Screen.png",	// 血
		"data\\TEXTURE\\Platform\\Concent_Screen.png",	// 集中線
		"data\\TEXTURE\\Platform\\Red_Screen.png",		// 全部分赤
		"data\\TEXTURE\\Platform\\Anxiety.png",			// 黒
	};
}

//=========================
// コンストラクタ
//=========================
CScreenEffect::CScreenEffect() : CObject2D(CObject::TYPE_SCREENEFFECT, CObject::PRIORITY_UI)
{
	// 全ての値をクリアする
	m_type = TYPE_RED_AROUND;		// 種類
	m_fAlpha = 1.0f;				// 透明度
}

//=========================
// デストラクタ
//=========================
CScreenEffect::~CScreenEffect()
{

}

//=========================
// 初期化処理
//=========================
HRESULT CScreenEffect::Init(void)
{
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_type = TYPE_RED_AROUND;		// 種類
	m_fAlpha = 1.0f;				// 透明度

	// 成功を返す
	return S_OK;
}

//=========================
// 終了処理
//=========================
void CScreenEffect::Uninit(void)
{
	// 終了
	CObject2D::Uninit();
}

//=========================
// 更新処理
//=========================
void CScreenEffect::Update(void)
{
	// 頂点情報の設定処理
	SetVertex();

	// 頂点カラーの設定処理
	SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));
}

//=========================
// 描画処理
//=========================
void CScreenEffect::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}

//=========================
// 情報の設定処理
//=========================
void CScreenEffect::SetData(const TYPE type)
{
	// スクロールの設定処理
	SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));		// 位置設定
	SetRot(NONE_D3DXVECTOR3);			// 向き設定
	SetSize(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));		// サイズ設定
	SetLength();			// 長さ設定
	SetAngle();				// 方向設定

	// 全ての値を初期化する
	m_type = type;				// 種類
	m_fAlpha = 1.0f;			// 透明度

	// 頂点情報の設定処理
	SetVertex();

	// 頂点カラーの設定処理
	SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));

	// テクスチャの読み込み処理
	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE[m_type]));
}

//=========================
// 生成処理
//=========================
CScreenEffect* CScreenEffect::Create(const TYPE type)
{
	// ローカルオブジェクトを生成
	CScreenEffect* pEffect = nullptr;	// エフェクトのインスタンスを生成

	if (pEffect == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pEffect = new CScreenEffect;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pEffect != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pEffect->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pEffect->SetData(type);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// エフェクトのポインタを返す
	return pEffect;
}

//=========================
// 透明度の設定処理
//=========================
void CScreenEffect::SetAlpha(const float fAlpha)
{
	// 透明度を設定する
	m_fAlpha = fAlpha;
}

//=========================
// 透明度の取得処理
//=========================
float CScreenEffect::GetAlpha(void) const
{
	// 透明度を返す
	return m_fAlpha;
}