//===========================================
//
// ブロックのメイン処理[block.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "block.h"
#include "useful.h"
#include "shadowCircle.h"

#include "dirtblock.h"
#include "rockblock.h"
#include "iceblock.h"
#include "magmablock.h"

#include "cliffchecker.h"

//==============================
// コンストラクタ
//==============================
CBlock::CBlock() : CModel(CObject::TYPE_BLOCK, CObject::PRIORITY_BLOCK)
{
	// コンストラクタの箱
	Box();
}

//==============================
// コンストラクタ
//==============================
CBlock::CBlock(CObject::TYPE type, CObject::PRIORITY priority) : CModel(type, priority)
{
	// コンストラクタの箱
	Box();
}

//==============================
// デストラクタ
//==============================
CBlock::~CBlock()
{

}

//==============================
// コンストラクタの箱
//==============================
void CBlock::Box(void)
{
	// 全ての値をクリアする
	m_type = CBlock::TYPE_DIRT;				// 種類

	for (int nCntMat = 0; nCntMat < MAX_MATERIAL; nCntMat++)
	{
		// マテリアルへのポインタ
		ZeroMemory(&m_aMat[nCntMat], sizeof(D3DXMATERIAL));
	}

	m_bCliffRight = false;						// 右側が崖つかまり出来るか
	m_bCliffLeft = false;						// 左側が崖つかまり出来るか

// デバッグ処理
#ifdef _DEBUG

	m_bDelete = false;							// 消去状況
#endif 
}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CBlock::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_type = TYPE_DIRT;		// 種類

	for (int nCntMat = 0; nCntMat < MAX_MATERIAL; nCntMat++)
	{
		// マテリアルへのポインタ
		ZeroMemory(&m_aMat[nCntMat], sizeof(D3DXMATERIAL));
	}

	m_bCliffRight = false;		// 右側が崖つかまり出来るか
	m_bCliffLeft = false;		// 左側が崖つかまり出来るか

// デバッグ処理
#ifdef _DEBUG

	m_bDelete = false;			// 消去状況
#endif 

	// 値を返す
	return S_OK;
}

//========================================
//ブロックの終了処理
//========================================
void CBlock::Uninit(void)
{
	// 終了処理
	CModel::Uninit();
}

//========================================
//ブロックの更新処理
//========================================
void CBlock::Update(void)
{

}

//=====================================
//ブロックの描画処理
//=====================================
void CBlock::Draw(void)
{
	// 描画処理
	CModel::DrawBlock(&m_aMat[0]);
}

//=====================================
// 情報の設定処理
//=====================================
void CBlock::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const D3DXMATERIAL* pMat, const TYPE type, const bool bCliffR, const bool bCliffL)
{
	for (int nCntMat = 0; nCntMat < (int)GetFileData().dwNumMat; nCntMat++)
	{
		// マテリアルへのポインタ
		m_aMat[nCntMat] = pMat[nCntMat];
	}

	// 崖つかまり状態を設定する
	m_bCliffRight = bCliffR;		// 右崖
	m_bCliffLeft = bCliffL;			// 左崖
	m_type = type;					// 種類

	// デバッグ処理
#ifdef _DEBUG

	m_bDelete = false;								// 消去状況
#endif 

	// 初期化処理
	SetPos(pos);			// 位置設定
	SetPosOld(pos);			// 前回の位置設定処理
	SetRot(rot);			// 向き設定
	SetScale(scale);		// 拡大率設定

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

	// ローカル変数宣言
	D3DXVECTOR3 Blockpos;		// 位置

	if (GetCliffLeft() == true)
	{ // 左の崖に掴まれる場合

		// 位置を取得する
		Blockpos = GetPos();

		// 位置を設定する
		Blockpos.x += GetFileData().vtxMin.x;
		Blockpos.y += GetFileData().vtxMax.y;
		Blockpos.z += GetFileData().vtxMin.z;

		// 崖捕まりチェッカーの生成処理
		CCliffChecker::Create(Blockpos);
	}

	if (GetCliffRight() == true)
	{ // 右の崖に掴まれる場合

		// 位置を取得する
		Blockpos = GetPos();

		// 位置を設定する
		Blockpos.x += GetFileData().vtxMax.x;
		Blockpos.y += GetFileData().vtxMax.y;
		Blockpos.z += GetFileData().vtxMin.z;

		// 崖捕まりチェッカーの生成処理
		CCliffChecker::Create(Blockpos);
	}
}

//=======================================
// 種類の設定処理
//=======================================
void CBlock::SetType(const TYPE type)
{
	// 種類を設定する
	m_type = type;
}

//=======================================
// 種類の取得処理
//=======================================
CBlock::TYPE CBlock::GetType(void) const
{
	// 種類を返す
	return m_type;
}

//=======================================
// 右崖の設定処理
//=======================================
void CBlock::SetCliffRight(const bool bCliff)
{
	// 右崖の判定を設定する
	m_bCliffRight = bCliff;
}

//=======================================
// 右崖の取得処理
//=======================================
bool CBlock::GetCliffRight(void) const
{
	// 右崖の判定を返す
	return m_bCliffRight;
}

//=======================================
// 左崖の設定処理
//=======================================
void CBlock::SetCliffLeft(const bool bCliff)
{
	// 左崖の判定を設定する
	m_bCliffLeft = bCliff;
}

//=======================================
// 左崖の取得処理
//=======================================
bool CBlock::GetCliffLeft(void) const
{
	// 左崖の判定を返す
	return m_bCliffLeft;
}

//=======================================
// マテリアルの取得処理
//=======================================
D3DXMATERIAL CBlock::GetMaterial(int nIdx) const
{
	// マテリアルの情報を返す
	return m_aMat[nIdx];
}

//=======================================
// 生成処理
//=======================================
CBlock* CBlock::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const D3DXMATERIAL* pMat, const TYPE type, const bool bCliffR, const bool bCliffL)
{
	// ローカルオブジェクトを生成
	CBlock* pBlock = nullptr;	// プレイヤーのインスタンスを生成

	if (pBlock == nullptr)
	{ // オブジェクトが NULL の場合

		switch (type)
		{
		case TYPE_DIRT:			// 地面ブロック

			// 地面ブロックを生成
			pBlock = new CDirtBlock;

			break;

		case TYPE_ROCK:			// 岩ブロック

			// 岩ブロックを生成
			pBlock = new CRockBlock;

			break;

		case TYPE_ICE:			// 氷ブロック

			// 氷ブロックを生成
			pBlock = new CIceBlock;

			break;

		case TYPE_MAGMA:		// マグマブロック

			// マグマブロックを生成
			pBlock = new CMagmaBlock;

			break;
		}
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pBlock != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pBlock->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "ブロックの初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pBlock->SetData(pos, rot, scale, pMat, type, bCliffR, bCliffL);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// プレイヤーのポインタを返す
	return pBlock;
}

// デバッグ処理
#ifdef _DEBUG

//=======================================
// 消去状況の設定処理
//=======================================
void CBlock::SetDelete(const bool bDelete)
{
	// 消去状況を設定する
	m_bDelete = bDelete;
}

//=======================================
// 消去状況の取得処理
//=======================================
bool CBlock::GetDelete(void) const
{
	// 消去状況を返す
	return m_bDelete;
}

#endif