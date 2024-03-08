//=======================================
//
// 3D残像のメイン処理[locus3D.cpp]
// Author 小原立暉
//
//=======================================
#include "manager.h"
#include "locus3D.h"
#include "renderer.h"
#include "texture.h"

//---------------------------------------
// マクロ定義
//---------------------------------------

//=========================
// コンストラクタ
//=========================
CLocus3D::CLocus3D() : CBillboard(CBillboard::TYPE_LOCUS, CObject::PRIORITY_PLAYER)
{
	// 全ての値をクリアする
	m_nLife = 0;			// 寿命
	m_fSub = 0.0f;			// 透明度の減衰量
	m_fAlpha = 0.0f;		// 透明度
	m_bLightOff = false;	// ライティング状況
}

//=========================
// デストラクタ
//=========================
CLocus3D::~CLocus3D()
{

}

//=========================
// 初期化処理
//=========================
HRESULT CLocus3D::Init(void)
{
	if (FAILED(CBillboard::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_nLife = 0;			// 寿命
	m_fSub = 0.0f;			// 透明度の減衰量
	m_fAlpha = 0.0f;		// 透明度
	m_bLightOff = false;	// ライティング状況

	// 成功を返す
	return S_OK;
}

//=========================
// 終了処理
//=========================
void CLocus3D::Uninit(void)
{
	// 終了
	CBillboard::Uninit();
}

//=========================
// 更新処理
//=========================
void CLocus3D::Update(void)
{
	// 寿命を減算する
	m_nLife--;

	// 透明度を減算する
	m_fAlpha -= m_fSub;

	if (m_fAlpha <= 0.0f)
	{ // 透明度が 0.0f 以下になった場合

		// 透明度を補正する
		m_fAlpha = 0.0f;
	}

	if (m_nLife <= 0)
	{ // 寿命が 0 以下になった場合

		// 終了処理
		Uninit();

		// この先の処理を行わない
		return;
	}

	// 頂点情報の初期化
	SetVertex();

	// 頂点カラーの設定処理
	SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));
}

//=========================
// 描画処理
//=========================
void CLocus3D::Draw(void)
{
	if (m_bLightOff == true)
	{ // ライティング状況が true の場合

		// 描画処理
		CBillboard::DrawLightOff(true);
	}
	else
	{ // 上記以外

		// 描画処理
		CBillboard::Draw();
	}
}
//=========================
// 情報の設定処理
//=========================
void CLocus3D::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const float fAlpha, const int nLife, const int nTexIdx, const bool bLookY, const bool bLighting)
{
	// スクロールの設定処理
	SetPos(pos);			// 位置設定
	SetRot(0.0f);			// 向き設定
	SetSize(size);			// サイズ設定
	SetLength();			// 長さ設定
	SetAngle();				// 方向設定
	SetEnableLookY(bLookY);	// Y軸設定

	// 全ての値を初期化する
	m_nLife = nLife;			// 寿命
	m_fSub = fAlpha / (float)(nLife);	// 透明度の減衰量
	m_fAlpha = fAlpha;			// 透明度
	m_bLightOff = bLighting;	// ライティング状況

	// 頂点情報の初期化
	SetVertex();

	// テクスチャの読み込み処理
	BindTexture(nTexIdx);
}

//=========================
// 生成処理
//=========================
CLocus3D* CLocus3D::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const float fAlpha, const int nLife, const int nTexIdx, const bool bLookY, const bool bLighting)
{
	// ローカルオブジェクトを生成
	CLocus3D* pLocus3D = nullptr;	// 残像のインスタンスを生成

	if (pLocus3D == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pLocus3D = new CLocus3D;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pLocus3D != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pLocus3D->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pLocus3D->SetData(pos, size, fAlpha, nLife, nTexIdx, bLookY, bLighting);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 残像のポインタを返す
	return pLocus3D;
}