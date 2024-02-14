//======================================================================================================================
//
//	メッシュウォール処理 [mesh_wall.cpp]
//	Author：小原立暉
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "manager.h"
#include "texture.h"
#include "mesh_wall.h"

//================================
// オーバーロードコンストラクタ
//================================
CMeshWall::CMeshWall(CObject::PRIORITY priority) : CMesh(CObject::TYPE_MESH, priority)
{
	// 全ての値をクリアする
	m_fWidthSize = 0.0f;		// 幅のサイズ
	m_fHeightSize = 0.0f;		// 縦のサイズ
	m_fWidthSizeDivi = 0.0f;	// 幅1面ずつのサイズ
	m_fHeightSizeDivi = 0.0f;	// 縦1面ずつのサイズ
}

//================================
// デストラクタ
//================================
CMeshWall::~CMeshWall()
{

}

//================================
// 初期化処理
//================================
HRESULT CMeshWall::Init(void)
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
void CMeshWall::Uninit(void)
{
	// 終了処理
	CMesh::Uninit();
}

//================================
// 更新処理
//================================
void CMeshWall::Update(void)
{

}

//================================
// 描画処理
//================================
void CMeshWall::Draw(void)
{
	// 描画処理
	CMesh::Draw();
}

//================================
// 情報の設定処理
//================================
void CMeshWall::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fHeight, const SGrid& Divi)
{
	// 頂点数を宣言する
	SGrid vtx;

	// 分割数を設定する
	vtx.nWidth = Divi.nWidth + 1;
	vtx.nHeight = Divi.nHeight + 1;

	// 全ての値を初期化する
	SetPos(pos);				// 位置
	SetRot(rot);				// 向き
	SetDivi(Divi);				// 分割数
	SetVtx(vtx);				// 頂点数
	SetType(TYPE_WALL);			// 種類
	SetNumVtx();				// 総頂点数
	SetNumIdx();				// 総インデックス数

	// 全ての値をクリアする
	m_fWidthSize = fWidth;		// 幅のサイズ
	m_fHeightSize = fHeight;	// 縦のサイズ
	m_fWidthSizeDivi = fWidth / (float)Divi.nWidth;		// 幅1面ずつのサイズ
	m_fHeightSizeDivi = fHeight / (float)Divi.nHeight;	// 縦1面ずつのサイズ
}

//================================
// 頂点の設定処理
//================================
void CMeshWall::SetVertex(void)
{
	VERTEX_3D *pVtx;									// 頂点情報へのポインタ
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();	// 頂点バッファ
	SGrid Divi = GetDivi();			// 分割数
	SGrid vtx = GetVtx();			// 頂点数

	if (pVtxBuff != nullptr)
	{ // 頂点バッファのポインタが NULL じゃない場合

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntDep = 0; nCntDep < vtx.nHeight; nCntDep++)
		{
			for (int nCntWid = 0; nCntWid < vtx.nWidth; nCntWid++)
			{
				// 頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3
				( // 引数
					(nCntWid * m_fWidthSizeDivi) - (m_fWidthSize / 2.0f),	// X座標
					m_fHeightSize - (nCntDep * m_fHeightSizeDivi),			// Y座標
					0.0f													// Z座標
				);

				// 法線ベクトルの設定
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// 頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(nCntWid * (1.0f / 5.0f), nCntDep * (1.0f / 5.0f));

				pVtx++;				// 頂点データを進める
			}
		}

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//================================
// 生成処理
//================================
CMeshWall* CMeshWall::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fHeight, const SGrid& Divi, const PRIORITY priority)
{
	// ローカルオブジェクトを生成
	CMeshWall* pMesh = nullptr;	// フィールドのインスタンスを生成

	if (pMesh == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pMesh = new CMeshWall(priority);
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
		pMesh->SetData(pos, rot, fWidth, fHeight, Divi);

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