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
// 無名名前空間
//--------------------------------------------
namespace
{
	const D3DXVECTOR3 LOGO_SIZE = D3DXVECTOR3(600.0f, 200.0f, 0.0f);
	const char* TEXTURE = "data/TEXTURE/TitleLogo.png";
}

//============================
// コンストラクタ
//============================
CTitleLogo::CTitleLogo() : CObject2D(CObject::TYPE_TITLELOGO, PRIORITY_UI)
{

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
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

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
}

//============================
// 更新処理
//============================
void CTitleLogo::Update(void)
{

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
	// 情報の設定処理
	SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));	// 位置
	SetPosOld(GetPos());			// 前回の位置
	SetRot(NONE_D3DXVECTOR3);		// 向き
	SetSize(LOGO_SIZE);				// サイズ
	SetAngle();						// 方向
	SetLength();					// 長さ

	// 頂点座標の設定処理
	SetVertex();

	// テクスチャの割り当て処理
	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE));
}

//============================
//生成処理
//============================
CTitleLogo* CTitleLogo::Create(void)
{
	// ローカルオブジェクトを生成
	CTitleLogo* pLogoMark = nullptr;	// プレイヤーのインスタンスを生成

	if (pLogoMark == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pLogoMark = new CTitleLogo;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pLogoMark != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pLogoMark->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pLogoMark->SetData();
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// ロゴマークのポインタを返す
	return pLogoMark;
}