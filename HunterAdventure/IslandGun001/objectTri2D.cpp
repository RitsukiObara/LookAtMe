//============================================
//
// オブジェクト三角形2Dのメイン処理[objectTri2D.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "objectTri2D.h"
#include "renderer.h"
#include "texture.h"

//=========================================
// オーバーロードコンストラクタ
//=========================================
CObjectTri2D::CObjectTri2D(CObject::TYPE type, PRIORITY priority) : CObject(type, DIM_2D, priority)
{
	// 全ての値をクリアする
	m_pVtxBuff = nullptr;			// 頂点バッファへのポインタ
	m_pos = NONE_D3DXVECTOR3;		// 位置
	m_posOld = m_pos;				// 前回の位置
	m_rot = NONE_D3DXVECTOR3;		// 向き
	m_size = NONE_D3DXVECTOR3;		// サイズ
	m_fLength = 0.0f;				// 長さ
	m_nTexIdx = NONE_TEXIDX;		// テクスチャのインデックス
}

//=========================================
// デストラクタ
//=========================================
CObjectTri2D::~CObjectTri2D()
{

}

//===========================================
// 三角ポリゴンの初期化処理
//===========================================
HRESULT CObjectTri2D::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 3,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{ // バッファの生成に失敗した場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);		// 位置
	m_posOld = m_pos;				// 前回の位置
	m_rot = NONE_D3DXVECTOR3;		// 向き
	m_size = D3DXVECTOR3(30.0f, 50.0f, 0.0f);	// サイズ
	m_fLength = (m_size.x + m_size.y) * 0.5f;	// 長さ
	m_nTexIdx = NONE_TEXIDX;		// テクスチャのインデックス

	// 頂点情報の設定処理
	SetVertexAll();

	if (m_pVtxBuff == nullptr)
	{ // 頂点バッファのポインタがNULLの場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 値を返す
	return S_OK;
}

//===========================================
// 三角ポリゴンの終了処理
//===========================================
void CObjectTri2D::Uninit(void)
{
	if (m_pVtxBuff != nullptr)
	{ // 頂点バッファの破棄

		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// 破棄処理
	Release();
}

//===========================================
// 三角ポリゴンの更新処理
//===========================================
void CObjectTri2D::Update(void)
{
	// 頂点設定処理
	SetVertexRot();

	// テクスチャの設定処理
	SetVtxTexture();
}

//===========================================
// 三角ポリゴンの描画処理
//===========================================
void CObjectTri2D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		m_pVtxBuff,								// 頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D));						// 頂点情報構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, CManager::Get()->GetTexture()->GetAddress(m_nTexIdx));

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLELIST,	// プリミティブの種類
		0,										// 描画する最初の頂点インデックス
		1);										// 描画するプリミティブ数
}

//===========================================
// テクスチャの割り当て処理
//===========================================
void CObjectTri2D::BindTexture(const int nIdx)
{
	// テクスチャを割り当てる
	m_nTexIdx = nIdx;
}

//===========================================
// 位置設定処理
//===========================================
void CObjectTri2D::SetPos(const D3DXVECTOR3& pos)
{
	// 位置を設定する
	m_pos = pos;
}

//===========================================
// 位置取得処理
//===========================================
D3DXVECTOR3 CObjectTri2D::GetPos(void) const
{
	// 位置を返す
	return m_pos;
}

//===========================================
// 前回の位置設定処理
//===========================================
void CObjectTri2D::SetPosOld(const D3DXVECTOR3& posOld)
{
	// 前回の位置を設定する
	m_posOld = posOld;
}

//===========================================
// 前回の位置取得処理
//===========================================
D3DXVECTOR3 CObjectTri2D::GetPosOld(void) const
{
	// 前回の位置を返す
	return m_posOld;
}

//===========================================
// 向き設定処理
//===========================================
void CObjectTri2D::SetRot(const D3DXVECTOR3& rot)
{
	// 向きを設定する
	m_rot = rot;
}

//===========================================
// 向き取得処理
//===========================================
D3DXVECTOR3 CObjectTri2D::GetRot(void) const
{
	// 向きを返す
	return m_rot;
}

//===========================================
// サイズ設定処理
//===========================================
void CObjectTri2D::SetSize(const D3DXVECTOR3& size)
{
	// サイズを設定する
	m_size = size;
}

//===========================================
// サイズ取得処理
//===========================================
D3DXVECTOR3 CObjectTri2D::GetSize(void) const
{
	// サイズを返す
	return m_size;
}

//===========================================
// 長さ設定処理
//===========================================
void CObjectTri2D::SetLength(void)
{
	// 長さを設定する
	m_fLength = (m_size.x + m_size.y) * 0.5f;
}

//===========================================
// 長さ取得処理
//===========================================
float CObjectTri2D::GetLength(void) const
{
	// 長さを返す
	return m_fLength;
}

//===========================================
// テクスチャのインデックスの取得処理
//===========================================
int CObjectTri2D::GetTexIdx(void) const
{
	// テクスチャのインデックスを返す
	return m_nTexIdx;
}

//===========================================
// 頂点情報の設定
//===========================================
void CObjectTri2D::SetVertex(void)
{
	VERTEX_2D * pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// 頂点情報の設定(向き有り)
//===========================================
void CObjectTri2D::SetVertexRot(void)
{
	VERTEX_2D * pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z + (-D3DX_PI * 0.25f)) * m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z + (-D3DX_PI * 0.25f)) * m_fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + D3DX_PI) * m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + D3DX_PI) * m_fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z + (D3DX_PI * 0.25f)) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI * 0.25f)) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// 頂点情報の設定(ベタ打ち)
//===========================================
void CObjectTri2D::SetVtxHardCoding(const D3DXVECTOR3& Left, const D3DXVECTOR3& Up, const D3DXVECTOR3& Right)
{
	VERTEX_2D* pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + Left.x, m_pos.y + Left.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + Up.x, m_pos.y + Up.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + Right.x, m_pos.y + Right.y, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// 頂点の色設定処理
//===========================================
void CObjectTri2D::SetVtxColor(const D3DXCOLOR& col)
{
	VERTEX_2D* pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// テクスチャの設定
//===========================================
void CObjectTri2D::SetVtxTexture(void)
{
	VERTEX_2D * pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// 全ての頂点情報の設定
//===========================================
void CObjectTri2D::SetVertexAll(void)
{
	VERTEX_2D * pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// 生成処理
//===========================================
CObjectTri2D* CObjectTri2D::Create(const CObject::TYPE type, const PRIORITY priority)
{
	// ローカルオブジェクトを生成
	CObjectTri2D* pObject2D = nullptr;		// 三角ポリゴンのインスタンスを生成

	if (pObject2D == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pObject2D = new CObjectTri2D(type, priority);
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pObject2D != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pObject2D->Init()))
		{ // 失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 三角ポリゴンのポインタを返す
	return pObject2D;
}