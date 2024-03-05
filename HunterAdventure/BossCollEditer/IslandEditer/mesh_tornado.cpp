//======================================================================================================================
//
// メッシュトルネード処理 [mesh_tornado.cpp]
// Author：小原立暉
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "manager.h"
#include "mesh_tornado.h"
#include "texture.h"
#include "useful.h"

//================================
// オーバーロードコンストラクタ
//================================
CMeshTornado::CMeshTornado(CObject::PRIORITY priority) : CMesh(CObject::TYPE_MESH, priority)
{
	// 全ての値をクリアする
	m_nVortex = 0;			// 渦の数
	m_fWidth = 0.0f;		// 幅
	m_fHeight = 0.0f;		// 高さ
	m_fShift = 0.0f;		// ずらす幅
	m_fCircum = 0.0f;		// 円周
}

//================================
// デストラクタ
//================================
CMeshTornado::~CMeshTornado()
{

}

//================================
// 初期化処理
//================================
HRESULT CMeshTornado::Init(void)
{
	if (FAILED(CMesh::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 頂点情報の設定処理
	SetVertex();

	// インデックスの設定処理
	SetIndex(GetVtx().nHeight, GetVtx().nWidth);

	// 成功を返す
	return S_OK;
}

//================================
// 終了処理
//================================
void CMeshTornado::Uninit(void)
{
	// 終了処理
	CMesh::Uninit();
}

//================================
// 更新処理
//================================
void CMeshTornado::Update(void)
{

}

//================================
// 描画処理
//================================
void CMeshTornado::Draw(void)
{
	// 描画処理
	CMesh::Draw();
}

//================================
// 情報の設定処理
//================================
void CMeshTornado::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fHeight, const int nVortex, const float fShift, const float fCircum, const SGrid& Divi)
{
	// 頂点数を宣言する
	SGrid vtx;

	// 頂点数を設定する
	vtx.nWidth = Divi.nWidth * nVortex;
	vtx.nHeight = Divi.nHeight + 1;

	// 全ての値を初期化する
	SetPos(pos);				// 位置
	SetRot(rot);				// 向き
	SetDivi(Divi);				// 分割数
	SetVtx(vtx);				// 頂点数
	SetType(TYPE_FIELD);		// 種類
	SetNumVtx();				// 総頂点数
	SetNumIdx();				// 総インデックス数

	// 全ての値をクリアする
	m_nVortex = nVortex;		// 渦の数
	m_fWidth = fWidth;			// 幅
	m_fHeight = fHeight;		// 高さ
	m_fShift = fShift;			// ずらす幅
	m_fCircum = fCircum;		// 円周
}

//================================
// 頂点の設定処理
//================================
void CMeshTornado::SetVertex(void)
{
	VERTEX_3D *pVtx;									// 頂点情報へのポインタ
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();	// 頂点バッファ
	SGrid Divi = GetDivi();			// 分割数
	SGrid vtx = GetVtx();			// 頂点数
	float fAngle;					// 向き
	float fLength;					// 長さ
	float fHeight;					// 高さ
	int nCount = 0;					// カウント

	if (pVtxBuff != nullptr)
	{ // 頂点バッファのポインタが NULL じゃない場合

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntWid = 0; nCntWid < vtx.nHeight; nCntWid++)
		{
			for (int nCntHei = 0; nCntHei < m_nVortex; nCntHei++)
			{
				fLength = ((m_fWidth / Divi.nHeight) * (Divi.nHeight - nCntWid)) + (m_fShift * nCntHei) + m_fCircum;

				for (int nCntCir = 0; nCntCir < Divi.nWidth; nCntCir++)
				{
					fHeight = (nCntHei * m_fHeight) + ((m_fHeight / Divi.nWidth) * nCntCir);

					fAngle = ((D3DX_PI * 2) / Divi.nWidth) * nCntCir;

					// 向きを正規化する
					useful::RotNormalize(&fAngle);

					// 頂点座標の設定
					pVtx[nCount].pos = D3DXVECTOR3
					( // 引数
						sinf(fAngle) * fLength,		// X座標
						fHeight,					// Y座標
						cosf(fAngle) * fLength		// Z座標
					);

					// 法線ベクトルの設定
					pVtx[nCount].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

					// 頂点カラーの設定
					pVtx[nCount].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					// テクスチャ座標の設定
					pVtx[nCount].tex = D3DXVECTOR2(((nCntHei * Divi.nWidth) + nCntCir) * (1.0f / (float)(Divi.nWidth)), nCntWid * (1.0f / (float)(Divi.nHeight)));

					// カウントを加算する
					nCount++;
				}
			}
		}

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//================================
// 生成処理
//================================
CMeshTornado* CMeshTornado::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fHeight, const int nVortex, const float fShift, const float fCircum, const SGrid& Divi, const PRIORITY priority)
{
	// ローカルオブジェクトを生成
	CMeshTornado* pMesh = nullptr;	// フィールドのインスタンスを生成

	if (pMesh == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pMesh = new CMeshTornado(priority);
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pMesh != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 情報の設定処理
		pMesh->SetData(pos, rot, fWidth, fHeight, nVortex, fShift, fCircum, Divi);

		// 初期化処理
		if (FAILED(pMesh->Init()))
		{ // 初期化処理に失敗した場合

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

	// フィールドのポインタを返す
	return pMesh;
}

//================================
// 円周の設定処理
//================================
void CMeshTornado::SetWidth(const float fWidth)
{
	// 幅を設定する
	m_fWidth = fWidth;
}

//================================
// 円周の取得処理
//================================
float CMeshTornado::GetWidth(void) const
{
	// 幅を返す
	return m_fWidth;
}

//================================
// 高さの設定処理
//================================
void CMeshTornado::SetHeight(const float fHeight)
{
	// 高さを設定する
	m_fHeight = fHeight;
}

//================================
// 高さの取得処理
//================================
float CMeshTornado::GetHeight(void) const
{
	// 高さを返す
	return m_fHeight;
}