//=================================================================================
//
//	軌跡処理 [orbit.cpp]
//	Author：小原立暉
//
//=================================================================================
//*********************************************************************************
//	インクルードファイル
//*********************************************************************************
#include "main.h"
#include "manager.h"
#include "orbit.h"
#include "renderer.h"
#include "texture.h"

//---------------------------------------------------------------------------------
// 無名名前空間
//---------------------------------------------------------------------------------
namespace
{
	const int ORBIT_HEIGHT_VTX = 2;			// 軌跡の縦幅
	const float ORBIT_ALPHA_SUB = 0.02f;	// 透明度の減算量
}

//=================================
// コンストラクタ
//=================================
COrbit::COrbit() : CObject(CObject::TYPE_ORBIT, DIM_3D, CObject::PRIORITY_UI)
{
	// 全ての値をクリアする
	m_pVtxBuff = nullptr;				// 頂点バッファへのポインタ
	m_pMtxParent = nullptr;				// 親のマトリックスへのポインタ
	m_nTexIdx = NONE_TEXIDX;			// テクスチャのインデックス

	for (int nCnt = 0; nCnt < MATRIXPLACE_MAX; nCnt++)
	{
		m_aOffset[nCnt] = NONE_D3DXVECTOR3;			// 両端のオフセット
		m_aCol[nCnt] = NONE_D3DXCOLOR;				// 両端の基準の色
	}

	for (int nCntVtx = 0; nCntVtx < MAX_ORBIT_VTX; nCntVtx++)
	{
		m_aPosPoint[nCntVtx] = NONE_D3DXVECTOR3;	// 計算後の各頂点座標
		m_aColPoint[nCntVtx] = NONE_D3DXCOLOR;		// 計算後の各頂点カラー
	}
}

//=================================
// デストラクタ
//=================================
COrbit::~COrbit()
{

}

//=================================
//	軌跡の初期化処理
//=================================
HRESULT COrbit::Init(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// デバイスへのポインタ
	VERTEX_3D *pVtx;							// 頂点情報へのポインタ

	// 全ての値を初期化する
	m_pVtxBuff = nullptr;				// 頂点バッファへのポインタ
	m_pMtxParent = nullptr;				// 親のマトリックスへのポインタ
	m_nTexIdx = NONE_TEXIDX;			// テクスチャのインデックス

	for (int nCnt = 0; nCnt < MATRIXPLACE_MAX; nCnt++)
	{
		m_aOffset[nCnt] = NONE_D3DXVECTOR3;			// 両端のオフセット
		m_aCol[nCnt] = NONE_D3DXCOLOR;				// 両端の基準の色
	}

	for (int nCntVtx = 0; nCntVtx < MAX_ORBIT_VTX; nCntVtx++)
	{
		m_aPosPoint[nCntVtx] = NONE_D3DXVECTOR3;	// 計算後の各頂点座標
		m_aColPoint[nCntVtx] = NONE_D3DXCOLOR;		// 計算後の各頂点カラー
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_3D) * MAX_ORBIT_VTX,	// 必要頂点数
		D3DUSAGE_WRITEONLY,					// 使用方法
		FVF_VERTEX_3D,						// 頂点フォーマット
		D3DPOOL_MANAGED,					// メモリの指定
		&m_pVtxBuff,						// 頂点バッファへのポインタ
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWid = 0; nCntWid < (int)(MAX_ORBIT_VTX * 0.5f); nCntWid++)
	{
		for (int nCntDep = 1; nCntDep >= 0; nCntDep--)
		{
			// 頂点座標の設定
			pVtx[0].pos = NONE_D3DXVECTOR3;

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = NONE_D3DXCOLOR;

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2
			(
				(float)(nCntWid * 0.05f),
				(float)((nCntDep) % 2)
			);

			pVtx++;				// 頂点データを進める
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// 成功を返す
	return S_OK;
}

//=================================
//	軌跡の終了処理
//=================================
void COrbit::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{ // 変数 (g_pVtxBuffOrbit) がNULLではない場合

		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// 破棄処理
	Release();
}

//=================================
//	軌跡の更新処理
//=================================
void COrbit::Update(void)
{

}

//=================================
//	軌跡の描画処理
//=================================
void COrbit::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();
	VERTEX_3D *pVtx;				// 頂点情報へのポインタ

	D3DXMATRIX mtxTrans;			// 計算用マトリックス
	D3DXMATRIX mtxUnit;				// 原点のマトリックス

	// ライティングをOFFにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// カリングをOFFにする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// マトリックスを作成し	親のマトリックスと掛け合わせる
	for (int nCntmtx = 0; nCntmtx < MATRIXPLACE_MAX; nCntmtx++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_aMtxWorldPoint[nCntmtx]);

		switch (nCntmtx)
		{
		case MATRIXPLACE_BOTTOM:		// 下のオフセット

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, m_aOffset[nCntmtx].x, m_aOffset[nCntmtx].y, m_aOffset[nCntmtx].z);
			D3DXMatrixMultiply(&m_aMtxWorldPoint[nCntmtx], &m_aMtxWorldPoint[nCntmtx], &mtxTrans);

			break;							// 抜け出す

		case MATRIXPLACE_TOP:			// 上のオフセット

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, m_aOffset[nCntmtx].x, m_aOffset[nCntmtx].y, m_aOffset[nCntmtx].z);
			D3DXMatrixMultiply(&m_aMtxWorldPoint[nCntmtx], &m_aMtxWorldPoint[nCntmtx], &mtxTrans);

			break;							// 抜け出す
		}

		// 親のマトリックスを掛け合わせる
		D3DXMatrixMultiply
		(
			&m_aMtxWorldPoint[nCntmtx],
			&m_aMtxWorldPoint[nCntmtx],
			m_pMtxParent
		);
	}

	// 頂点座標と頂点カラーをずらす
	for (int nCnt = MAX_ORBIT_VTX - (ORBIT_HEIGHT_VTX + 1); nCnt >= 0; nCnt--)
	{ // 頂点数分ずらす

		// 頂点座標のデータを2個分ずらす
		m_aPosPoint[nCnt + ORBIT_HEIGHT_VTX] = m_aPosPoint[nCnt];

		// 頂点カラーのデータを2個分ずらす
		m_aColPoint[nCnt + ORBIT_HEIGHT_VTX] = m_aColPoint[nCnt];
	}

	// 最初の頂点座標と頂点カラーを代入する
	for (int nCntNew = 0; nCntNew < MATRIXPLACE_MAX; nCntNew++)
	{ // マトリックスの数繰り返す

		// 頂点座標を代入する
		m_aPosPoint[nCntNew].x = m_aMtxWorldPoint[nCntNew]._41;
		m_aPosPoint[nCntNew].y = m_aMtxWorldPoint[nCntNew]._42;
		m_aPosPoint[nCntNew].z = m_aMtxWorldPoint[nCntNew]._43;

		// 色を代入する
		m_aColPoint[nCntNew] = m_aCol[nCntNew];
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0; nCntVtx < MAX_ORBIT_VTX; nCntVtx++)
	{
		// 頂点座標の設定
		pVtx[nCntVtx].pos = m_aPosPoint[nCntVtx];
		pVtx[nCntVtx].col = m_aColPoint[nCntVtx];
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxUnit);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxUnit);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		m_pVtxBuff,						//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_3D));				//頂点情報構造体のサイズ

	//テクスチャの設定
	pDevice->SetTexture(0, CManager::Get()->GetTexture()->GetAddress(m_nTexIdx));

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//プリミティブの種類
		0,													//描画する最初の頂点インデックス
		MAX_ORBIT_VTX - 2);									//描画するプリミティブ数

	// カリングをONにする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//ライティングをONにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//================================
// テクスチャの割り当て処理
//================================
void COrbit::BindTexture(int nIdx)
{
	// テクスチャを割り当てる
	m_nTexIdx = nIdx;
}

//================================
// 情報の設定処理
//================================
void COrbit::SetData(D3DXMATRIX *mtx, const D3DXVECTOR3 &pos, const D3DXVECTOR3& OffSet1, const D3DXVECTOR3& OffSet2, const int nTexIdx, const TYPE type)
{
	// 全ての値を設定する
	m_pMtxParent = mtx;			// 親のマトリックスへのポインタ
	m_nTexIdx = nTexIdx;		// テクスチャのインデックス

	m_aOffset[MATRIXPLACE_BOTTOM] = OffSet1;	// 両端のオフセット
	m_aOffset[MATRIXPLACE_TOP] = OffSet2;		// 両端のオフセット

	for (int nCntSet = 0; nCntSet < MATRIXPLACE_MAX; nCntSet++)
	{
		// 情報の初期化
		m_aCol[nCntSet] = NONE_D3DXCOLOR;								// 両端の標準の色
		ZeroMemory(&m_aMtxWorldPoint[nCntSet], sizeof(D3DXMATRIX));		// 両端のワールドマトリックス
	}

	for (int nCntVtx = 0; nCntVtx < MAX_ORBIT_VTX; nCntVtx++)
	{
		// 情報の設定
		m_aColPoint[nCntVtx] = NONE_D3DXCOLOR;		// 計算後の各頂点カラー
		m_aPosPoint[nCntVtx] = pos;					// 計算後の各頂点座標
	}

	// 種類を設定する
	SetType(type);
}

//================================
// 情報の設定処理
//================================
void COrbit::SetData(D3DXMATRIX *mtx, const D3DXVECTOR3& pos, const D3DXVECTOR3& OffSet1, const D3DXVECTOR3& OffSet2, const char* pTextureName, const TYPE type)
{
	// 全ての値を設定する
	m_pMtxParent = mtx;			// 親のマトリックスへのポインタ
	m_nTexIdx = CManager::Get()->GetTexture()->Regist(pTextureName);	// テクスチャのインデックス

	m_aOffset[MATRIXPLACE_BOTTOM] = OffSet1;	// 両端のオフセット
	m_aOffset[MATRIXPLACE_TOP] = OffSet2;		// 両端のオフセット

	for (int nCntSet = 0; nCntSet < MATRIXPLACE_MAX; nCntSet++)
	{
		// 情報の初期化
		m_aCol[nCntSet] = NONE_D3DXCOLOR;								// 両端の標準の色
		ZeroMemory(&m_aMtxWorldPoint[nCntSet], sizeof(D3DXMATRIX));		// 両端のワールドマトリックス
	}

	for (int nCntVtx = 0; nCntVtx < MAX_ORBIT_VTX; nCntVtx++)
	{
		// 情報の設定
		m_aColPoint[nCntVtx] = NONE_D3DXCOLOR;		// 計算後の各頂点カラー
		m_aPosPoint[nCntVtx] = pos;					// 計算後の各頂点座標
	}

	// 種類を設定する
	SetType(type);
}

//=================================
// 生成処理(テクスチャ番号)
//=================================
COrbit* COrbit::Create(D3DXMATRIX *mtx, const D3DXVECTOR3& pos, const D3DXVECTOR3& OffSet1, const D3DXVECTOR3& OffSet2, const int nTexIdx, const TYPE type)
{
	// ローカルオブジェクトを生成
	COrbit* pOrbit = nullptr;	// 軌跡のインスタンスを生成

	if (pOrbit == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pOrbit = new COrbit;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pOrbit != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pOrbit->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 全頂点の位置の設定処理
		pOrbit->SetData(mtx, pos, OffSet1, OffSet2, nTexIdx, type);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// 軌跡のポインタを返す
	return pOrbit;
}

//=================================
// 生成処理(テクスチャの名前)
//=================================
COrbit* COrbit::Create(D3DXMATRIX *mtx, const D3DXVECTOR3& pos, const D3DXVECTOR3& OffSet1, const D3DXVECTOR3& OffSet2, const char* pTextureName, const TYPE type)
{
	// ローカルオブジェクトを生成
	COrbit* pOrbit = nullptr;	// 軌跡のインスタンスを生成

	if (pOrbit == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pOrbit = new COrbit;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pOrbit != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pOrbit->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 全頂点の位置の設定処理
		pOrbit->SetData(mtx, pos, OffSet1, OffSet2, pTextureName, type);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// 軌跡のポインタを返す
	return pOrbit;
}

//=================================
// 位置のリセット処理
//=================================
void COrbit::PosReset(void)
{
	VERTEX_3D* pVtx;				// 頂点情報へのポインタ

	D3DXMATRIX mtxTrans;			// 計算用マトリックス
	D3DXMATRIX mtxUnit;				// 原点のマトリックス

	// マトリックスを作成し	親のマトリックスと掛け合わせる
	for (int nCntmtx = 0; nCntmtx < MATRIXPLACE_MAX; nCntmtx++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_aMtxWorldPoint[nCntmtx]);

		switch (nCntmtx)
		{
		case MATRIXPLACE_BOTTOM:		// 下のオフセット

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, m_aOffset[nCntmtx].x, m_aOffset[nCntmtx].y, m_aOffset[nCntmtx].z);
			D3DXMatrixMultiply(&m_aMtxWorldPoint[nCntmtx], &m_aMtxWorldPoint[nCntmtx], &mtxTrans);

			break;							// 抜け出す

		case MATRIXPLACE_TOP:			// 上のオフセット

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, m_aOffset[nCntmtx].x, m_aOffset[nCntmtx].y, m_aOffset[nCntmtx].z);
			D3DXMatrixMultiply(&m_aMtxWorldPoint[nCntmtx], &m_aMtxWorldPoint[nCntmtx], &mtxTrans);

			break;							// 抜け出す
		}

		// 親のマトリックスを掛け合わせる
		D3DXMatrixMultiply
		(
			&m_aMtxWorldPoint[nCntmtx],
			&m_aMtxWorldPoint[nCntmtx],
			m_pMtxParent
		);
	}

	// 最初の頂点座標と頂点カラーを代入する
	for (int nCntNew = 0; nCntNew < MAX_ORBIT_VTX; nCntNew++)
	{ // マトリックスの数繰り返す

		// 頂点座標を代入する
		m_aPosPoint[nCntNew].x = m_aMtxWorldPoint[nCntNew % 2]._41;
		m_aPosPoint[nCntNew].y = m_aMtxWorldPoint[nCntNew % 2]._42;
		m_aPosPoint[nCntNew].z = m_aMtxWorldPoint[nCntNew % 2]._43;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0; nCntVtx < MAX_ORBIT_VTX; nCntVtx++)
	{
		// 頂点座標の設定
		pVtx[nCntVtx].pos = m_aPosPoint[nCntVtx];
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}