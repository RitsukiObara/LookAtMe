//============================================
//
// マルチテクスチャのオブジェクト3Dのメイン処理[object3D_multitex.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "main.h"
#include "manager.h"
#include "object3D_multitex.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

//=========================================
// オーバーロードコンストラクタ
//=========================================
CObject3DMultiTex::CObject3DMultiTex(CObject::TYPE type, PRIORITY priority) : CObject(type, DIM_3D, priority)
{
	// 全ての値をクリアする
	m_pTexStyle = nullptr;							// テクスチャ描画の情報
	m_pVtxBuff = nullptr;							// 頂点バッファへのポインタ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	m_posOld = m_pos;								// 前回の位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// サイズ
	ZeroMemory(&m_mtxWorld, sizeof(D3DXMATRIX));	// ワールドマトリックス
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_aTexIdx[nCnt] = NONE_TEXIDX;				// テクスチャのインデックス
	}
}

//=========================================
// デストラクタ
//=========================================
CObject3DMultiTex::~CObject3DMultiTex()
{

}

//===========================================
// オブジェクト3Dの初期化処理
//===========================================
HRESULT CObject3DMultiTex::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D_MULTI) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D_MULTI,
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
	m_pTexStyle = nullptr;						// テクスチャ描画の情報
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_posOld = m_pos;							// 前回の位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// サイズ

	VERTEX_3D_MULTI * pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, +m_size.y, +m_size.z);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x, +m_size.y, +m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x, -m_size.y, -m_size.z);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].texM = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].texM = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].texM = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].texM = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

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
// オブジェクト3Dの終了処理
//===========================================
void CObject3DMultiTex::Uninit(void)
{
	if (m_pVtxBuff != nullptr)
	{ // 頂点バッファの破棄

		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	if (m_pTexStyle != nullptr)
	{ // テクスチャ描画が NULL じゃない場合

		delete m_pTexStyle;
		m_pTexStyle = nullptr;
	}

	// 破棄処理
	Release();
}

//===========================================
// オブジェクト3Dの更新処理
//===========================================
void CObject3DMultiTex::Update(void)
{
	// 頂点の設定処理
	SetVertex();
}

//===========================================
// オブジェクト3Dの描画処理
//===========================================
void CObject3DMultiTex::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;			// 計算用マトリックス
	D3DXMATRIX mtxView;						// ビューマトリックス取得用

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D_MULTI);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	if (m_pTexStyle != nullptr)
	{ // テクスチャ描画が NULL じゃない場合

		// 描画終わりの処理
		m_pTexStyle->DispStart();
	}

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		m_pVtxBuff,									// 頂点バッファへのポインタ
		0,
		sizeof(VERTEX_3D_MULTI));					// 頂点情報構造体のサイズ

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(nCnt, CManager::Get()->GetTexture()->GetAddress(m_aTexIdx[nCnt]));
	}

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,											// 描画する最初の頂点インデックス
		2);											// 描画するプリミティブ数

	if (m_pTexStyle != nullptr)
	{ // テクスチャ描画が NULL じゃない場合

		// 描画終わりの処理
		m_pTexStyle->DispEnd();
	}
}

//===========================================
// 描画処理(ライティング無し)
//===========================================
void CObject3DMultiTex::DrawLightOff(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;			// 計算用マトリックス
	D3DXMATRIX mtxView;						// ビューマトリックス取得用

	// ライティングをOFFにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D_MULTI);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	if (m_pTexStyle != nullptr)
	{ // テクスチャ描画が NULL じゃない場合

		// 描画終わりの処理
		m_pTexStyle->DispStart();
	}

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		m_pVtxBuff,									// 頂点バッファへのポインタ
		0,
		sizeof(VERTEX_3D_MULTI));							// 頂点情報構造体のサイズ

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(nCnt, CManager::Get()->GetTexture()->GetAddress(m_aTexIdx[nCnt]));
	}

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,											// 描画する最初の頂点インデックス
		2);											// 描画するプリミティブ数

	// ライティングをONにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	if (m_pTexStyle != nullptr)
	{ // テクスチャ描画が NULL じゃない場合

		// 描画終わりの処理
		m_pTexStyle->DispEnd();
	}
}

//===========================================
// テクスチャの割り当て処理
//===========================================
void CObject3DMultiTex::BindTexture(int nIdx, int nCnt)
{
	// 番号を割り当てる
	m_aTexIdx[nCnt] = nIdx;
}

//===========================================
// 位置設定処理
//===========================================
void CObject3DMultiTex::SetPos(const D3DXVECTOR3& pos)
{
	// 位置を設定する
	m_pos = pos;
}

//===========================================
// 位置取得処理
//===========================================
D3DXVECTOR3 CObject3DMultiTex::GetPos(void) const
{
	// 位置を返す
	return m_pos;
}

//===========================================
// 前回の位置設定処理
//===========================================
void CObject3DMultiTex::SetPosOld(const D3DXVECTOR3& posOld)
{
	// 前回の位置を設定する
	m_posOld = posOld;
}

//===========================================
// 前回の位置取得処理
//===========================================
D3DXVECTOR3 CObject3DMultiTex::GetPosOld(void) const
{
	// 前回の位置を返す
	return m_posOld;
}

//===========================================
// 向き設定処理
//===========================================
void CObject3DMultiTex::SetRot(const D3DXVECTOR3& rot)
{
	// 向きを設定する
	m_rot = rot;
}

//===========================================
// 向き取得処理
//===========================================
D3DXVECTOR3 CObject3DMultiTex::GetRot(void) const
{
	// 向きを返す
	return m_rot;
}

//===========================================
// サイズ設定処理
//===========================================
void CObject3DMultiTex::SetSize(const D3DXVECTOR3& size)
{
	// サイズを設定する
	m_size = size;
}

//===========================================
// サイズ取得処理
//===========================================
D3DXVECTOR3 CObject3DMultiTex::GetSize(void) const
{
	// サイズを返す
	return m_size;
}

//===========================================
// マトリックスの取得処理
//===========================================
D3DXMATRIX CObject3DMultiTex::GetMatrix(void) const
{
	// ワールドマトリックスのデータを返す
	return m_mtxWorld;
}

//===========================================
// マトリックスポインタの取得処理
//===========================================
D3DXMATRIX* CObject3DMultiTex::GetMatrixPoint(void)
{
	// ワールドマトリックスのポインタを返す
	return &m_mtxWorld;
}

//===========================================
// 種類の設定処理
//===========================================
void CObject3DMultiTex::SetType(const TYPE type)
{
	if (m_pTexStyle != nullptr)
	{ // テクスチャ描画の情報が NULL の場合

		// メモリを開放する
		delete m_pTexStyle;
		m_pTexStyle = nullptr;							// テクスチャ描画の情報
	}

	switch (type)
	{
	case CObject3DMultiTex::TYPE_NONE:

		// 無状態なので特になし

		break;

	case CObject3DMultiTex::TYPE_PULL_OUT_WHITE:

		// 型抜き(背景白)
		m_pTexStyle = new CPulloutWhite;

		break;

	case CObject3DMultiTex::TYPE_BACKGROUND_WHITE:

		// 背景切り抜き(背景白)
		m_pTexStyle = new CBackGroundWhite;

		break;

	case CObject3DMultiTex::TYPE_PULL_OUT_BLACK:

		// 型抜き(背景黒)
		m_pTexStyle = new CPulloutBlack;

		break;

	case CObject3DMultiTex::TYPE_BACKGROUND_BLACK:

		// 背景切り抜き(背景黒)
		m_pTexStyle = new CBackGroundBlack;

		break;

	case CObject3DMultiTex::TYPE_LIGHT_BLACK:

		// 輝くテクスチャ(黒背景)
		m_pTexStyle = new CLightBlack;

		break;

	case CObject3DMultiTex::TYPE_NEGA_BLACK:

		// ネガテクスチャ(黒背景)
		m_pTexStyle = new CNegaBlack;

		break;

	case CObject3DMultiTex::TYPE_MONO_TRANS:

		// 白黒背景(半透明)
		m_pTexStyle = new CMonoTrans;

		break;

	case CObject3DMultiTex::TYPE_PULL_OUT_SP:

		// 型抜き(完全透過)
		m_pTexStyle = new CPulloutSpecial;

		break;

	case CObject3DMultiTex::TYPE_LIGHT_PULL_OUT:

		// 型抜きしつつ輝くテクスチャ
		m_pTexStyle = new CLightPullout;

		break;

	case CObject3DMultiTex::TYPE_LIGHT_SP:

		// 輝くテクスチャ(完全透過)
		m_pTexStyle = new CLightSpecial;

		break;

	case CObject3DMultiTex::TYPE_BACKGROUND_SP:

		// 背景切り抜き(完全透過)
		m_pTexStyle = new CBackGroundSpecial;

		break;

	case CObject3DMultiTex::TYPE_NEGA_SP:

		// ネガテクスチャ(完全透過)
		m_pTexStyle = new CNegaSpecial;

		break;

	case CObject3DMultiTex::TYPE_BACKGROUND_NEGA:

		// ネガテクスチャ(背景切り抜き)
		m_pTexStyle = new CBackGroundNega;

		break;

	case CObject3DMultiTex::TYPE_FUSION:

		// 合成
		m_pTexStyle = new CFusion;

		break;

	default:

		// 停止
		assert(false);

		break;
	}
}

//===========================================
// テクスチャのインデックスの取得処理
//===========================================
int CObject3DMultiTex::GetTexIdx(const int nCnt) const
{
	// テクスチャのインデックスを返す
	return m_aTexIdx[nCnt];
}

//===========================================
// 頂点情報の設定
//===========================================
void CObject3DMultiTex::SetVertex(void)
{
	VERTEX_3D_MULTI * pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, +m_size.y, +m_size.z);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x, +m_size.y, +m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x, -m_size.y, -m_size.z);

	D3DXVECTOR3 nor[4] = {};

	// 法線の計算(正規化)
	useful::NormalizeVector(&nor[0], pVtx[2].pos, pVtx[1].pos, pVtx[0].pos);
	useful::NormalizeVector(&nor[1], pVtx[0].pos, pVtx[3].pos, pVtx[1].pos);
	useful::NormalizeVector(&nor[2], pVtx[3].pos, pVtx[0].pos, pVtx[2].pos);
	useful::NormalizeVector(&nor[3], pVtx[1].pos, pVtx[2].pos, pVtx[3].pos);

	// 法線座標の設定
	pVtx[0].nor = nor[0];
	pVtx[1].nor = nor[1];
	pVtx[2].nor = nor[2];
	pVtx[3].nor = nor[3];

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// 頂点カラーの設定処理
//===========================================
void CObject3DMultiTex::SetVtxColor(const D3DXCOLOR& col)
{
	VERTEX_3D_MULTI * pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// 頂点テクスチャの設定処理(縦の一定間隔のテクスチャ配分)
//===========================================
void CObject3DMultiTex::SetVtxTextureWidth(const D3DXVECTOR2& size, int nCnt)
{
	// ローカル変数宣言
	float fTexWidth = (float)(size.x / size.y);

	VERTEX_3D_MULTI * pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (nCnt == 0)
	{ // 0の場合

		// 頂点テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(fTexWidth, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(fTexWidth, 1.0f);
	}
	else
	{ // 上記以外

		// 頂点テクスチャの設定
		pVtx[0].texM = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].texM = D3DXVECTOR2(fTexWidth, 0.0f);
		pVtx[2].texM = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].texM = D3DXVECTOR2(fTexWidth, 1.0f);
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// 頂点テクスチャの設定処理(横の一定間隔のテクスチャ配分)
//===========================================
void CObject3DMultiTex::SetVtxTextureHeight(const D3DXVECTOR2& size, int nCnt)
{
	// ローカル変数宣言
	float fTexHeight = (float)(size.y / size.x);

	VERTEX_3D_MULTI * pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (nCnt == 0)
	{ // 0の場合

		// 頂点テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, fTexHeight);
		pVtx[3].tex = D3DXVECTOR2(1.0f, fTexHeight);
	}
	else
	{ // 上記以外

		// 頂点テクスチャの設定
		pVtx[0].texM = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].texM = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].texM = D3DXVECTOR2(0.0f, fTexHeight);
		pVtx[3].texM = D3DXVECTOR2(1.0f, fTexHeight);
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// 頂点テクスチャの設定処理(アニメーション版)
//===========================================
void CObject3DMultiTex::SetVtxTextureAnim(const D3DXVECTOR2& anim, int nCnt)
{
	VERTEX_3D_MULTI* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (nCnt == 0)
	{ // 0の場合

		// 頂点テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(anim.x, anim.y);
		pVtx[1].tex = D3DXVECTOR2(anim.x + 1.0f, anim.y);
		pVtx[2].tex = D3DXVECTOR2(anim.x, anim.y + 1.0f);
		pVtx[3].tex = D3DXVECTOR2(anim.x + 1.0f, anim.y + 1.0f);
	}
	else
	{ // 上記以外

		// 頂点テクスチャの設定
		pVtx[0].texM = D3DXVECTOR2(anim.x, anim.y);
		pVtx[1].texM = D3DXVECTOR2(anim.x + 1.0f, anim.y);
		pVtx[2].texM = D3DXVECTOR2(anim.x, anim.y + 1.0f);
		pVtx[3].texM = D3DXVECTOR2(anim.x + 1.0f, anim.y + 1.0f);
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// 生成処理
//===========================================
CObject3DMultiTex* CObject3DMultiTex::Create(const CObject::TYPE type, const PRIORITY priority, const TYPE typeTex)
{
	// ローカルオブジェクトを生成
	CObject3DMultiTex* pObject3D = nullptr;	// オブジェクト3Dのインスタンスを生成

	if (pObject3D == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pObject3D = new CObject3DMultiTex(type, priority);
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pObject3D != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pObject3D->Init()))
		{ // 失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 種類の設定処理
		pObject3D->SetType(typeTex);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// オブジェクト3Dのポインタを返す
	return pObject3D;
}