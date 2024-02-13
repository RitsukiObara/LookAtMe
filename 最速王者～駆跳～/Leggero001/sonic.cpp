//============================================
//
// 高速演出のメイン処理[sonic.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "renderer.h"
#include "sonic.h"
#include "texture.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define SONIC_TEXTURE			"data/TEXTURE/SonicEffect.png"		// 速度演出のテクスチャ

//=========================================
// コンストラクタ
//=========================================
CSonic::CSonic() : CScroll(CObject::TYPE_BG, CObject::PRIORITY_BG)
{

}

//=========================================
// デストラクタ
//=========================================
CSonic::~CSonic()
{

}

//===========================================
// 高速演出の初期化処理
//===========================================
HRESULT CSonic::Init(void)
{
	if (FAILED(CScroll::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// テクスチャの割り当て処理
	BindTexture(CManager::GetTexture()->Regist(SONIC_TEXTURE));

	// スクロールの設定処理
	SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));	// 位置設定
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// 向き設定
	SetSize(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));		// サイズ設定
	SetLength();			// 長さ設定
	SetAngle();				// 方向設定
	SetTex(D3DXVECTOR2(0.0f, 0.0f));			// テクスチャ座標の設定処理
	SetSpeed(D3DXVECTOR2(0.005f, 0.0f));		// 速度の設定処理

	// 頂点情報の設定処理
	SetVertex();

	{
		// テクスチャの設定処理
		D3DXVECTOR2 tex = GetTex();

		// テクスチャ設定
		SetVtxTextureScroll(tex);
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// 高速演出の終了処理
//===========================================
void CSonic::Uninit(void)
{
	// 終了処理
	CScroll::Uninit();
}

//===========================================
// 高速演出の更新処理
//===========================================
void CSonic::Update(void)
{
	// 更新処理
	CScroll::Update();
}

//===========================================
// 高速演出の描画処理
//===========================================
void CSonic::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			//アルファテストの有効/無効設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);		//アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, ALPHA_TEST_NUMBER);		//アルファテストの参照値設定

	// 描画処理
	CScroll::Draw();

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			// アルファテストの有効/無効設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);		// アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);						// アルファテストの参照値設定
}

//===========================================
// 生成処理
//===========================================
CSonic* CSonic::Create(void)
{
	// ローカルオブジェクトを生成
	CSonic* pSonic = nullptr;			// 高速演出のインスタンスを生成

	if (pSonic == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pSonic = new CSonic;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pSonic != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pSonic->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "高速演出の初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// 高速演出のポインタを返す
	return pSonic;
}