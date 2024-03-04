//===========================================
//
// 壁のメイン処理[wall.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "wall.h"
#include "texture.h"
#include "useful.h"

//-------------------------------------------
// 無名名前空間
//-------------------------------------------
namespace
{
	const char* MODEL[CWall::TYPE_MAX] =		// モデルの名前
	{
		"data\\MODEL\\Wall001.x",		// 通常種類
		"data\\MODEL\\Wall002.x",		// 細い種類
	};
}

//-------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------
CListManager<CWall*> CWall::m_aList[area::NUM_AREA] = {};			// リスト

//==============================
// コンストラクタ
//==============================
CWall::CWall() : CModel(TYPE_WALL, PRIORITY_BLOCK)
{
	// 全ての値をクリアする
	m_vtxMax = NONE_D3DXVECTOR3;		// 頂点の最大値
	m_vtxMin = NONE_D3DXVECTOR3;		// 頂点の最小値
	m_type = TYPE_NORMAL;				// 種類
	m_rottype = ROTTYPE_FRONT;			// 向きの種類
	m_nFieldIdx = 0;					// 区分の番号
}

//==============================
// デストラクタ
//==============================
CWall::~CWall()
{

}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CWall::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 値を返す
	return S_OK;
}

//========================================
//ブロックの終了処理
//========================================
void CWall::Uninit(void)
{
	// 引き抜き処理
	m_aList[m_nFieldIdx].Pull(this);

	// 終了処理
	CModel::Uninit();
}

//========================================
//ブロックの更新処理
//========================================
void CWall::Update(void)
{

}

//=====================================
//ブロックの描画処理
//=====================================
void CWall::Draw(void)
{
	// 描画処理
	CModel::Draw();
}

//=====================================
// 情報の設定処理
//=====================================
void CWall::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& scale, const TYPE type, const ROTTYPE rottype)
{
	// 情報の設定処理
	SetPos(pos);					// 位置
	SetPosOld(pos);					// 前回の位置
	SetScale(scale);				// 拡大率
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL[type]));	// モデルの情報

	// 種類を設定する
	m_type = type;				// 種類
	m_rottype = rottype;		// 向きの種類

	// 頂点の設定処理
	SetVertex();

	// 区分の番号の設定処理
	m_nFieldIdx = area::SetFieldIdx(GetPos());

	// リストに追加する
	m_aList[m_nFieldIdx].Regist(this);
}

//=======================================
// 生成処理
//=======================================
CWall* CWall::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& scale, const TYPE type, const ROTTYPE rottype)
{
	// ローカルオブジェクトを生成
	CWall* pBlock = nullptr;		// インスタンスを生成

	if (pBlock == nullptr)
	{ // オブジェクトが NULL の場合

		// ブロックを生成する
		pBlock = new CWall;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pBlock != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pBlock->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pBlock->SetData(pos, scale, type, rottype);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// ブロックのポインタを返す
	return pBlock;
}

//=======================================
// 頂点の最大値の取得処理
//=======================================
D3DXVECTOR3 CWall::GetVtxMax(void) const
{
	// 頂点の最大値を返す
	return m_vtxMax;
}

//=======================================
// 頂点の最小値の取得処理
//=======================================
D3DXVECTOR3 CWall::GetVtxMin(void) const
{
	// 頂点の最小値を返す
	return m_vtxMin;
}

//=======================================
// 種類の取得処理
//=======================================
CWall::TYPE CWall::GetType(void) const
{
	// 種類を返す
	return m_type;
}

//=======================================
// 向きの種類の取得処理
//=======================================
CWall::ROTTYPE CWall::GetRotType(void) const
{
	// 向きの種類を返す
	return m_rottype;
}

//=======================================
// リストの取得処理
//=======================================
CListManager<CWall*> CWall::GetList(const int nIdx)
{
	// リストマネージャーを返す
	return m_aList[nIdx];
}

//=======================================
// 頂点の設定処理
//=======================================
void CWall::SetVertex(void)
{
	CXFile::SXFile filedata = GetFileData();	// Xファイルデータ
	D3DXVECTOR3 scale = GetScale();				// 拡大率
	D3DXVECTOR3 vtxMax = NONE_D3DXVECTOR3;		// 頂点の最大値
	D3DXVECTOR3 vtxMin = NONE_D3DXVECTOR3;		// 頂点の最小値

	// 頂点を一旦算出する
	vtxMax.x = filedata.vtxMax.x * scale.x;
	vtxMax.y = filedata.vtxMax.y * scale.y;
	vtxMax.z = filedata.vtxMax.z * scale.z;
	vtxMin.x = filedata.vtxMin.x * scale.x;
	vtxMin.y = filedata.vtxMin.y * scale.y;
	vtxMin.z = filedata.vtxMin.z * scale.z;

	switch (m_rottype)
	{
	case CWall::ROTTYPE_FRONT:		// 前向き

		// 向きを設定する
		SetRot(NONE_D3DXVECTOR3);

		// 最大値と最小値を設定する
		m_vtxMax = vtxMax;			// 最大値
		m_vtxMin = vtxMin;			// 最小値

		break;

	case CWall::ROTTYPE_RIGHT:		// 右向き

		// 向きを設定する
		SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));

		// 最大値と最小値を設定する
		m_vtxMax = D3DXVECTOR3(-vtxMin.z, vtxMax.y, vtxMax.x);
		m_vtxMin = D3DXVECTOR3(-vtxMax.z, vtxMin.y, vtxMin.x);

		break;

	case CWall::ROTTYPE_BACK:		// 後ろ向き

		// 向きを設定する
		SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));

		// 最大値と最小値を設定する
		m_vtxMax = D3DXVECTOR3(-vtxMin.x, vtxMax.y, -vtxMin.z);
		m_vtxMin = D3DXVECTOR3(-vtxMax.x, vtxMin.y, -vtxMax.z);

		break;

	case CWall::ROTTYPE_LEFT:		// 左向き

		// 向きを設定する
		SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));

		// 最大値と最小値を設定する
		m_vtxMax = D3DXVECTOR3(vtxMax.z, vtxMax.y, -vtxMin.x);
		m_vtxMin = D3DXVECTOR3(vtxMin.z, vtxMin.y, -vtxMax.x);

		break;

	default:

		// 停止
		assert(false);

		break;
	}
}