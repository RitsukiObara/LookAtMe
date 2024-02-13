//============================================
//
// 背景モデルのメイン処理[bgmodel.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "bgmodel.h"
#include "manager.h"
#include "renderer.h"
#include "useful.h"

//===========================================
// コンストラクタ
//===========================================
CBgModel::CBgModel() : CModel(TYPE_BGOBJECT, PRIORITY_BG)
{
	// 全ての値をクリアする
	m_type = CXFile::TYPE_ENERGYITEM;	// 種類

	for (int nCntMat = 0; nCntMat < MAX_MATERIAL; nCntMat++)
	{
		// マテリアルのデータ
		ZeroMemory(&m_aMat[nCntMat], sizeof(D3DXMATERIAL));
	}
}

//===========================================
// デストラクタ
//===========================================
CBgModel::~CBgModel()
{

}

//===========================================
// 初期化処理
//===========================================
HRESULT CBgModel::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値をクリアする
	m_type = CXFile::TYPE_ENERGYITEM;	// 種類

	for (int nCntMat = 0; nCntMat < MAX_MATERIAL; nCntMat++)
	{
		// マテリアルのデータ
		ZeroMemory(&m_aMat[nCntMat], sizeof(D3DXMATERIAL));
	}

	// 値を返す
	return S_OK;
}

//===========================================
// 終了処理
//===========================================
void CBgModel::Uninit(void)
{
	// 終了処理
	CModel::Uninit();
}

//===========================================
// 更新処理
//===========================================
void CBgModel::Update(void)
{

}

//===========================================
// 描画処理
//===========================================
void CBgModel::Draw(void)
{
	// 描画処理
	CModel::DrawBlock(m_aMat);
}

//===========================================
// 情報の設定処理
//===========================================
void CBgModel::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const D3DXMATERIAL* pMat, const CXFile::TYPE type)
{
	// 種類を設定する
	m_type = type;

	for (int nCntMat = 0; nCntMat < (int)CXFile::GetXFile(type).dwNumMat; nCntMat++)
	{
		// マテリアルへのポインタ
		m_aMat[nCntMat] = pMat[nCntMat];
	}

	// 初期化処理
	SetPos(pos);			// 位置設定
	SetPosOld(pos);			// 前回の位置設定処理
	SetRot(rot);			// 向き設定
	SetScale(scale);		// 拡大率設定
	SetFileData(type);		// データの設定処理
		
	CXFile::SXFile file = GetFileData();

	// 最大値を反映する
	file.vtxMax.x *= scale.x;
	file.vtxMax.y *= scale.y;
	file.vtxMax.z *= scale.z;

	// 最小値を反映する
	file.vtxMin.x *= scale.x;
	file.vtxMin.y *= scale.y;
	file.vtxMin.z *= scale.z;

	// データの設定処理
	SetFileData(file);
}

//===========================================
// 生成処理
//===========================================
CBgModel* CBgModel::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const D3DXMATERIAL* pMat, const CXFile::TYPE type)
{
	// ローカルオブジェクトを生成
	CBgModel* pBgModel = nullptr;	// プレイヤーのインスタンスを生成

	if (pBgModel == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pBgModel = new CBgModel;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pBgModel != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pBgModel->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "ブロックの初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pBgModel->SetData(pos, rot, scale, pMat, type);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// プレイヤーのポインタを返す
	return pBgModel;
}