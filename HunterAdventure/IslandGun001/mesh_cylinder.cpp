//======================================================================================================================
//
//	メッシュシリンダー処理 [mesh_cylinder.cpp]
//	Author：小原立暉
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "manager.h"
#include "texture.h"
#include "mesh_cylinder.h"
#include "useful.h"

//================================
// オーバーロードコンストラクタ
//================================
CMeshCylinder::CMeshCylinder(CObject::PRIORITY priority) : CMesh(CObject::TYPE_MESH, priority)
{
	// 全ての値をクリアする
	m_fCircumSize = 0.0f;		// 幅のサイズ
	m_fHeightSize = 0.0f;		// 縦のサイズ
	m_fHeightSizeDivi = 0.0f;	// 縦1面ずつのサイズ
}

//================================
// デストラクタ
//================================
CMeshCylinder::~CMeshCylinder()
{

}

//================================
// 初期化処理
//================================
HRESULT CMeshCylinder::Init(void)
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
void CMeshCylinder::Uninit(void)
{
	// 終了処理
	CMesh::Uninit();
}

//================================
// 更新処理
//================================
void CMeshCylinder::Update(void)
{

}

//================================
// 描画処理
//================================
void CMeshCylinder::Draw(void)
{
	// 描画処理
	CMesh::Draw();
}

//================================
// 情報の設定処理
//================================
void CMeshCylinder::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fHeight, const SGrid& Divi)
{
	// 分割数と頂点数を宣言する
	SGrid vtx;

	// 分割数を設定する
	vtx.nWidth = Divi.nWidth + 1;
	vtx.nHeight = Divi.nHeight + 1;

	// 全ての値を初期化する
	SetPos(pos);				// 位置
	SetRot(rot);				// 向き
	SetDivi(Divi);				// 分割数
	SetVtx(vtx);				// 頂点数
	SetType(TYPE_CYLINDER);		// 種類
	SetNumVtx();				// 総頂点数
	SetNumIdx();				// 総インデックス数

	// 全ての値をクリアする
	m_fCircumSize = fWidth;		// 円周のサイズ
	m_fHeightSize = fHeight;	// 縦のサイズ
	m_fHeightSizeDivi = fHeight / (float)Divi.nHeight;	// 縦1面ずつのサイズ
}

//================================
// 頂点の設定処理
//================================
void CMeshCylinder::SetVertex(void)
{
	VERTEX_3D *pVtx;		// 頂点情報へのポインタ
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();	// 頂点バッファ
	SGrid Divi = GetDivi();	// 分割数
	SGrid vtx = GetVtx();	// 頂点数
	float fAngle;			// 角度算出用変数

	if (pVtxBuff != nullptr)
	{ // 頂点バッファが NULL じゃない場合

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntHeight = 0; nCntHeight < vtx.nHeight; nCntHeight++)
		{
			for (int nCntCircum = 0; nCntCircum < vtx.nWidth; nCntCircum++)
			{
				// 角度を算出する
				fAngle = D3DX_PI * (nCntCircum * (float)(1.0f / (Divi.nWidth / 2)));

				// 角度の正規化
				useful::RotNormalize(&fAngle);

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3
				(
					(-sinf(fAngle) * m_fCircumSize),
					m_fHeightSize - (m_fHeightSizeDivi * nCntHeight),
					(-cosf(fAngle) * m_fCircumSize)
				);

				//法線ベクトルの設定
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(((float)(1.0f / Divi.nWidth) * nCntCircum), ((float)(1.0f / Divi.nHeight) * nCntHeight));

				pVtx++;			// 頂点データを進める
			}
		}

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//================================
// 生成処理
//================================
CMeshCylinder* CMeshCylinder::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fHeight, const SGrid& Divi, const PRIORITY priority)
{
	// ローカルオブジェクトを生成
	CMeshCylinder* pMesh = nullptr;	// メッシュのインスタンスを生成

	if (pMesh == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pMesh = new CMeshCylinder(priority);
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

	// メッシュのポインタを返す
	return pMesh;
}