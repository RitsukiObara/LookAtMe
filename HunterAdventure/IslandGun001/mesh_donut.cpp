//======================================================================================================================
//
//	メッシュドーナツ処理 [mesh_donut.cpp]
//	Author：小原立暉
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "manager.h"
#include "texture.h"
#include "mesh_donut.h"
#include "useful.h"

//================================
// オーバーロードコンストラクタ
//================================
CMeshDonut::CMeshDonut(CObject::PRIORITY priority) : CMesh(TYPE_MESH, priority)
{
	// 全ての値をクリアする
	m_fCircumSize = 0.0f;		// 円周のサイズ
	m_fWidthSize = 0.0f;		// 幅のサイズ
	m_fWidthDiviSize = 0.0f;	// 幅1面ずつのサイズ
}

//================================
// デストラクタ
//================================
CMeshDonut::~CMeshDonut()
{

}

//================================
// 初期化処理
//================================
HRESULT CMeshDonut::Init(void)
{
	if (FAILED(CMesh::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全頂点情報の設定処理
	SetVertexAll();

	// インデックスの設定処理
	SetIndex(GetVtx().nWidth, GetVtx().nHeight);

	// 成功を返す
	return S_OK;
}

//================================
// 終了処理
//================================
void CMeshDonut::Uninit(void)
{
	// 終了処理
	CMesh::Uninit();
}

//================================
// 更新処理
//================================
void CMeshDonut::Update(void)
{

}

//================================
// 描画処理
//================================
void CMeshDonut::Draw(void)
{
	// 描画処理
	CMesh::Draw();
}

//================================
// 情報の設定処理
//================================
void CMeshDonut::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fCircum, const SGrid& Divi)
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
	SetType(TYPE_DOME);			// 種類
	SetNumVtx();				// 総頂点数
	SetNumIdx();				// 総インデックス数

	// 全ての値を設定する
	m_fCircumSize = fCircum;		// 円周のサイズ
	m_fWidthSize = fWidth;			// 幅のサイズ
	m_fWidthDiviSize = fWidth / (float)Divi.nWidth;	// 幅1面ずつのサイズ
}

//================================
// 全頂点の設定処理
//================================
void CMeshDonut::SetVertexAll(void)
{
	// ローカル変数宣言
	VERTEX_3D* pVtx;		// 頂点情報へのポインタ
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();	// 頂点バッファ
	SGrid Divi = GetDivi();	// 分割数
	SGrid vtx = GetVtx();	// 頂点数
	float fAngle;			// 方向
	float fLength;			// 高さの方向

	if (pVtxBuff != nullptr)
	{ // 頂点バッファが NULL じゃない場合

		// 頂点バッファをロック
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntWidth = 0; nCntWidth < vtx.nWidth; nCntWidth++)
		{
			for (int nCntCircum = 0; nCntCircum < vtx.nHeight; nCntCircum++)
			{
				// 角度を算出する
				fAngle = D3DX_PI * (nCntCircum * (float)(1.0f / (Divi.nHeight / 2)));

				// 幅を算出する
				fLength = m_fCircumSize + (m_fWidthDiviSize * (Divi.nWidth - nCntWidth));

				// 角度の正規化
				useful::RotNormalize(&fAngle);

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3
				(
					(sinf(fAngle) * fLength),
					(cosf(fAngle) * fLength),
					0.0f
				);

				//法線ベクトルの設定
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(((float)(1.0f / Divi.nHeight) * nCntCircum), (nCntWidth * 1.0f));

				pVtx++;			// 頂点データを進める
			}
		}

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//================================
// 頂点の設定処理
//================================
void CMeshDonut::SetVertex(void)
{
	// ローカル変数宣言
	VERTEX_3D *pVtx;		// 頂点情報へのポインタ
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();	// 頂点バッファ
	SGrid Divi = GetDivi();	// 分割数
	SGrid vtx = GetVtx();	// 頂点数
	float fAngle;			// 方向
	float fLength;			// 高さの方向

	if (pVtxBuff != nullptr)
	{ // 頂点バッファが NULL じゃない場合

		// 頂点バッファをロック
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntWidth = 0; nCntWidth < vtx.nWidth; nCntWidth++)
		{
			for (int nCntCircum = 0; nCntCircum < vtx.nHeight; nCntCircum++)
			{
				// 角度を算出する
				fAngle = D3DX_PI * (nCntCircum * (float)(1.0f / (Divi.nHeight / 2)));

				// 幅を算出する
				fLength = m_fCircumSize + (m_fWidthDiviSize * (Divi.nWidth - nCntWidth));

				// 角度の正規化
				useful::RotNormalize(&fAngle);

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3
				(
					(sinf(fAngle) * fLength),
					(cosf(fAngle) * fLength),
					0.0f
				);

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
CMeshDonut* CMeshDonut::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fCircum, const SGrid& Divi, const PRIORITY priority)
{
	// ローカルオブジェクトを生成
	CMeshDonut* pMesh = nullptr;		// メッシュのインスタンスを生成

	if (pMesh == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pMesh = new CMeshDonut(priority);
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
		pMesh->SetData(pos, rot, fCircum, fWidth, Divi);

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

//================================
// 円周の設定処理
//================================
void CMeshDonut::SetCircum(const float fCircum)
{
	// 円周を設定する
	m_fCircumSize = fCircum;
}

//================================
// 円周の取得処理
//================================
float CMeshDonut::GetCircum(void) const
{
	// 円周を返す
	return m_fCircumSize;
}

//================================
// 幅の設定処理
//================================
void CMeshDonut::SetWidth(const float fWidth)
{
	// 幅を設定する
	m_fWidthSize = fWidth;
	m_fWidthDiviSize = fWidth / (float)GetDivi().nWidth;
}

//================================
// 幅の取得処理
//================================
float CMeshDonut::GetWidth(void) const
{
	// 幅を返す
	return m_fWidthSize;
}