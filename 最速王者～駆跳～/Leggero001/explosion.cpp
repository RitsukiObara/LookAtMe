//============================================
//
// 爆発のメイン処理[explosion.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "explosion.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define EXPLOSION_TEXTURE		"data/TEXTURE/explosion000.png"	// 爆発のテクスチャ

//===========================================
// コンストラクタ
//===========================================
CExplosion::CExplosion() : CBillboardAnim(CObject::TYPE_EXPLOSION,CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_nLife = 0;				// 寿命
}

//===========================================
// デストラクタ
//===========================================
CExplosion::~CExplosion()
{

}

//===========================================
// 初期化処理
//===========================================
HRESULT CExplosion::Init(void)
{
	if (FAILED(CBillboardAnim::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_nLife = 0;				// 寿命

	// 成功を返す
	return S_OK;
}

//===========================================
// 終了処理
//===========================================
void CExplosion::Uninit(void)
{
	// 終了
	CBillboardAnim::Uninit();
}

//===========================================
// 更新処理
//===========================================
void CExplosion::Update(void)
{
	// 寿命を減らす
	m_nLife--;

	// 更新処理
	CBillboardAnim::Update();

	if (m_nLife <= 0)
	{ // パターン数が最大数を超えた場合

		// 終了処理
		Uninit();
	}
}

//===========================================
// 描画処理
//===========================================
void CExplosion::Draw(void)
{
	// 描画処理
	CBillboardAnim::DrawLightOff();
}

//===========================================
// 設定処理
//===========================================
void CExplosion::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size)
{
	// スクロールの設定処理
	SetPos(pos);				// 位置設定
	SetSize(size);				// サイズ設定

	// アニメーションの設定処理
	SetAnim(3, 8);

	// 全ての値を初期化する
	m_nLife = 3 * 8;			// 寿命

	// 頂点情報の初期化
	SetVertex();

	// テクスチャ情報の設定処理
	SetVtxAnim(GetTexPattern(), GetPattern());

	// テクスチャの読み込み処理
	BindTexture(CManager::GetTexture()->Regist(EXPLOSION_TEXTURE));
}

//===========================================
// 生成処理
//===========================================
CExplosion* CExplosion::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size)
{
	// ローカルオブジェクトを生成
	CExplosion* pExplosion = nullptr;	// プレイヤーのインスタンスを生成

	if (pExplosion == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pExplosion = new CExplosion;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pExplosion != nullptr)
	{ // オブジェクトが NULL じゃない場合

	  // 初期化処理
		if (FAILED(pExplosion->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "爆発の初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pExplosion->SetData(pos, size);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pExplosion;
}