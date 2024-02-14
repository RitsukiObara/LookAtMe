//===========================================
//
// 丸影のメイン処理[shadowCircle.cpp]
// Author 小原立暉
//
//===========================================
#include "main.h"
#include "shadowCircle.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "collision.h"

//-------------------------------------------
// 無名名前空間
//-------------------------------------------
namespace
{
	const D3DXCOLOR INIT_SHADOW_COL = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);	// 初期の影の色
	const char* SHADOW_TEXTURE = "data\\TEXTURE\\shadow.jpg";				// 影のテクスチャ
}

//====================
// コンストラクタ
//====================
CShadowCircle::CShadowCircle() : CObject3D(TYPE_SHADOW, PRIORITY_SHADOW)
{
	// 全ての値をクリアする
	m_posParent = nullptr;			// 親の位置
	m_col = INIT_SHADOW_COL;		// 色
	m_fRadiusInit = 0.0f;			// 初期の半径
}

//====================
// デストラクタ
//====================
CShadowCircle::~CShadowCircle()
{

}

//====================
// 初期化処理
//====================
HRESULT CShadowCircle::Init(void)
{
	// 初期化処理
	if (FAILED(CObject3D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_posParent = nullptr;			// 親の位置
	m_col = INIT_SHADOW_COL;		// 色
	m_fRadiusInit = 0.0f;			// 初期の半径

	// 成功を返す
	return S_OK;
}

//====================
// 終了処理
//====================
void CShadowCircle::Uninit(void)
{
	// オブジェクトの終了処理
	CObject3D::Uninit();
}

//====================
// 更新処理
//====================
void CShadowCircle::Update(void)
{
	// 距離による設定処理
	Distance();

	// 頂点情報の設定処理
	SetVertex();

	// 頂点カラーの設定処理
	SetVtxColor(m_col);
}

//====================
// 描画処理
//====================
void CShadowCircle::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);			// Zバッファ更新の有効 / 無効の設定

	//減算合成
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 描画処理
	CObject3D::Draw();

	//通常の合成に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Zバッファ更新の有効 / 無効の設定
}

//====================
// 情報の設定処理
//====================
void CShadowCircle::SetData(const D3DXVECTOR3& pos, const float fRadius, D3DXVECTOR3* posParent)
{
	// 情報の初期化
	m_posParent = posParent;		// 親の位置
	m_col = INIT_SHADOW_COL;		// 色
	m_fRadiusInit = fRadius;		// 初期の半径

	// 情報の設定
	SetPos(pos);				// 位置
	SetPosOld(pos);				// 前回の位置
	SetRot(NONE_D3DXVECTOR3);	// 向き
	SetSize(D3DXVECTOR3(fRadius, 0.0f, fRadius));		// サイズ

	// 設定処理
	Distance();

	// 頂点情報の設定処理
	SetVertex();

	// 頂点カラーの設定処理
	SetVtxColor(m_col);

	// テクスチャの読み込み処理
	BindTexture(CManager::Get()->GetTexture()->Regist(SHADOW_TEXTURE));
}

//====================
// 生成処理
//====================
CShadowCircle* CShadowCircle::Create(const D3DXVECTOR3& pos, const float fRadius, D3DXVECTOR3* posParent)
{
	// ローカルオブジェクトを生成
	CShadowCircle* pShadow = nullptr;	// 影のインスタンスを生成

	if (pShadow == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pShadow = new CShadowCircle;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pShadow != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pShadow->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pShadow->SetData(pos, fRadius, posParent);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 影のポインタを返す
	return pShadow;
}

//====================
// 透明度の設定処理
//====================
void CShadowCircle::Distance(void)
{
	if (m_posParent != nullptr)
	{ // 親の位置のポインタが NULL じゃない場合

		D3DXVECTOR3 pos = GetPos();		// 位置を取得する
		float fDis = 0.0f;				// 距離

		// 距離を測る
		fDis = m_posParent->y - pos.y;
	}
}