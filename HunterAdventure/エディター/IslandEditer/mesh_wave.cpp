//====================================================
//
// メッシュの波処理 [mesh_wave.cpp]
// Author：小原立暉
//
//====================================================
//****************************************************
//	インクルードファイル
//****************************************************
#include "manager.h"
#include "mesh_wave.h"
#include "texture.h"
#include "useful.h"

// 無名名前空間
namespace
{
	const int DIVI_WIDTH = 50;				// 横の分割数
	const int DIVI_HEIGHT = 50;				// 縦(奥行)の分割数
	const float SIZE_WIDTH = 1000.0f;		// 幅のサイズ
	const float SIZE_DEPTH = 1000.0f;		// 奥行のサイズ
}

//================================
// オーバーロードコンストラクタ
//================================
CMeshWave::CMeshWave(CObject::PRIORITY priority) : CMeshField(priority)
{
	// 全ての値をクリアする
	m_fWaveRot = 0.0f;		// 波の向き
	m_fWaveHeight = 0.0f;	// 波の高さ
	m_fCycle = 0.0f;		// 波の周期
	m_fSpeed = 0.0f;		// 波の速度
}

//================================
// デストラクタ
//================================
CMeshWave::~CMeshWave()
{

}

//================================
// 初期化処理
//================================
HRESULT CMeshWave::Init(void)
{
	if (FAILED(CMeshField::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//================================
// 終了処理
//================================
void CMeshWave::Uninit(void)
{
	// 終了処理
	CMeshField::Uninit();
}

//================================
// 更新処理
//================================
void CMeshWave::Update(void)
{
	// 波の設定処理
	SetWave();

	// 波の向きを加算する
	m_fWaveRot += m_fSpeed;

	// 波の向きを正規化する
	useful::RotNormalize(&m_fWaveRot);
}

//================================
// 描画処理
//================================
void CMeshWave::Draw(void)
{
	// 描画処理
	CMeshField::Draw();
}

//================================
// データの設定処理
//================================
void CMeshWave::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fDepth, const SGrid& Divi, const float fWaveHeight, const float fCycle, const float fSpeed)
{
	// 情報の設定処理
	CMeshField::SetData
	(
		pos,
		rot,
		fWidth,
		fDepth,
		Divi
	);

	// 全ての値を初期化する
	m_fWaveRot = 0.0f;		// 波の向き
	m_fWaveHeight = fWaveHeight;	// 波の高さ
	m_fCycle = fCycle;		// 波の周期
	m_fSpeed = fSpeed;		// 波の速度
}

//================================
// 生成処理
//================================
CMeshWave* CMeshWave::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fDepth, const SGrid& Divi, const float fWaveHeight, const float fCycle, const float fSpeed, const PRIORITY priority)
{
	// ローカルオブジェクトを生成
	CMeshWave* pWave = nullptr;			// メッシュの波のインスタンスを生成

	if (pWave == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pWave = new CMeshWave(priority);
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pWave != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 情報の設定処理
		pWave->SetData(pos, rot, fWidth, fDepth, Divi, fWaveHeight, fCycle, fSpeed);

		// 初期化処理
		if (FAILED(pWave->Init()))
		{ // 初期化処理に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// メッシュの波のポインタを返す
	return pWave;
}

//================================
// 波の設定処理
//================================
void CMeshWave::SetWave(void)
{
	VERTEX_3D *pVtx;									// 頂点情報へのポインタ
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();	// 頂点バッファ
	SGrid vtx = GetVtx();	// 頂点数
	float fRot;				// 波の向き

	if (pVtxBuff != nullptr)
	{ // 頂点バッファのポインタが NULL じゃない場合

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntDep = 0; nCntDep < vtx.nHeight; nCntDep++)
		{
			// 波の向きを代入する
			fRot = m_fWaveRot;

			for (int nCntWid = 0; nCntWid < vtx.nWidth; nCntWid++)
			{
				// 向きを加算する
				fRot += m_fCycle;

				// 向きの正規化
				useful::RotNormalize(&fRot);

				// 高さを設定する
				pVtx[0].pos.y = cosf(fRot) * m_fWaveHeight;

				pVtx++;				// 頂点データを進める
			}
		}

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}