//============================================
//
// 崖捕まりのメイン処理[cliffchecker.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "cliffchecker.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define CLIFFCHECKER_TEXTURE		"data/TEXTURE/CliffChecker.png"		// 崖捕まりのテクスチャ
#define CLIFFCHECKER_SIZE			(D3DXVECTOR3(20.0f, 20.0f, 0.0f))	// 崖捕まりチェッカーのサイズ

//===========================================
// コンストラクタ
//===========================================
CCliffChecker::CCliffChecker() : CObject3D(CObject::TYPE_CLIFFCHECKER, CObject::PRIORITY_SHADOW)
{

}

//===========================================
// デストラクタ
//===========================================
CCliffChecker::~CCliffChecker()
{

}

//===========================================
// 初期化処理
//===========================================
HRESULT CCliffChecker::Init(void)
{
	if (FAILED(CObject3D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// 終了処理
//===========================================
void CCliffChecker::Uninit(void)
{
	// 終了
	CObject3D::Uninit();
}

//===========================================
// 更新処理
//===========================================
void CCliffChecker::Update(void)
{
	// 更新
	CObject3D::Update();
}

//===========================================
// 描画処理
//===========================================
void CCliffChecker::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			//アルファテストの有効/無効設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);		//アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, ALPHA_TEST_NUMBER);		//アルファテストの参照値設定

	// 描画処理
	CObject3D::DrawLightOff();

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			// アルファテストの有効/無効設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);		// アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);						// アルファテストの参照値設定
}

//===========================================
// 情報の設定処理
//===========================================
void CCliffChecker::SetData(const D3DXVECTOR3& pos)
{
	// スクロールの設定処理
	SetPos(pos);								// 位置設定
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// 向き設定
	SetSize(CLIFFCHECKER_SIZE);					// サイズ設定

	// 頂点情報の設定処理
	SetVertex();

	// 全ての値を初期化する
	BindTexture(CManager::GetTexture()->Regist(CLIFFCHECKER_TEXTURE));			// テクスチャの読み込み
}

//===========================================
// 生成処理
//===========================================
CCliffChecker* CCliffChecker::Create(const D3DXVECTOR3& pos)
{
	// ローカルオブジェクトを生成
	CCliffChecker* pCliff = nullptr;	// プレイヤーのインスタンスを生成

	if (pCliff == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pCliff = new CCliffChecker;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pCliff != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pCliff->Init()))
		{ // 崖捕まりの初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "崖捕まりの初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pCliff->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// 崖捕まりチェッカーのポインタを返す
	return pCliff;
}