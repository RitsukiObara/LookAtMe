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
#include "block_manager.h"
#include "useful.h"

#include "screw.h"
#include "airplane.h"

// マクロ定義
#define SCREW_MOVE				(D3DXVECTOR3(0.0f, 20.0f, 0.0f))		// ネジの移動量
#define AIRPLANE_SPAWN_CHECK	(500.0f)								// 飛行機のチェック

//==============================
// コンストラクタ
//==============================
CBlock::CBlock() : CModel(CObject::TYPE_BLOCK, CObject::PRIORITY_BLOCK)
{
	// 全ての値をクリアする
	m_type = TYPE_IRON;			// 種類
	m_dropType = DROPTYPE_NONE;	// ドロップの種類
	m_nBreakCount = 0;			// 破壊カウント
	m_bBreak = false;			// 破壊状況
	m_pPrev = nullptr;			// 前へのポインタ
	m_pNext = nullptr;			// 次へのポインタ

	if (CBlockManager::Get() != nullptr)
	{ // マネージャーが存在していた場合

		// マネージャーへの登録処理
		CBlockManager::Get()->Regist(this);
	}
}

//==============================
// デストラクタ
//==============================
CBlock::~CBlock()
{

}

//============================
// 前のポインタの設定処理
//============================
void CBlock::SetPrev(CBlock* pPrev)
{
	// 前のポインタを設定する
	m_pPrev = pPrev;
}

//============================
// 後のポインタの設定処理
//============================
void CBlock::SetNext(CBlock* pNext)
{
	// 次のポインタを設定する
	m_pNext = pNext;
}

//============================
// 前のポインタの設定処理
//============================
CBlock* CBlock::GetPrev(void) const
{
	// 前のポインタを返す
	return m_pPrev;
}

//============================
// 次のポインタの設定処理
//============================
CBlock* CBlock::GetNext(void) const
{
	// 次のポインタを返す
	return m_pNext;
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
	m_type = TYPE_IRON;			// 種類
	m_dropType = DROPTYPE_NONE;	// ドロップの種類
	m_nBreakCount = 0;			// 破壊カウント
	m_bBreak = false;			// 破壊状況

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

	if (CBlockManager::Get() != nullptr)
	{ // マネージャーが存在していた場合

		// リスト構造の引き抜き処理
		CBlockManager::Get()->Pull(this);
	}

	// リスト構造関係のポインタを NULL にする
	m_pPrev = nullptr;
	m_pNext = nullptr;
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
	CModel::Draw();
}

//=====================================
// ドロップ処理
//=====================================
void CBlock::Drop(void)
{
	switch (m_dropType)
	{
	case DROPTYPE_NONE:			// ドロップ無し

		// ドロップしない

		break;

	case DROPTYPE_ITEM:			// アイテムドロップ

		// ネジの生成
		CScrew::Create(D3DXVECTOR3(GetPos().x, GetPos().y + GetFileData().vtxMax.y, GetPos().z), SCREW_MOVE, true);

		break;

	case DROPTYPE_AIRPLANE:		// 飛行機ドロップ

		if (GetPos().z >= AIRPLANE_SPAWN_CHECK)
		{ // 飛行機が奥にある場合

			// 飛行機の生成
			CAirplane::Create(D3DXVECTOR3(GetPos().x, GetPos().y + GetFileData().vtxMax.y, GetPos().z), false, CAirplane::STATE_APPEAR);
		}
		else
		{ // 飛行機が手前にある場合

			// 飛行機の生成
			CAirplane::Create(D3DXVECTOR3(GetPos().x, GetPos().y + GetFileData().vtxMax.y, GetPos().z), true, CAirplane::STATE_APPEAR);
		}

		break;

	default:

		// 停止
		assert(false);

		break;
	}
}

//=====================================
// 情報の設定処理
//=====================================
void CBlock::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const TYPE type, const DROPTYPE dropType)
{
	// 情報の設定処理
	SetPos(pos);						// 位置
	SetPosOld(pos);						// 前回の位置
	SetRot(rot);						// 向き
	SetScale(scale);					// 拡大率

	// 全ての値を設定する
	m_type = type;			// 種類
	m_dropType = dropType;	// ドロップの種類
	m_nBreakCount = 0;		// 破壊カウント

	switch (type)
	{
	case TYPE_IRON:

		SetFileData(CXFile::TYPE_IRONBLOCK);	// モデルの情報
		m_bBreak = false;		// 破壊状況

		break;

	case TYPE_WOOD:

		SetFileData(CXFile::TYPE_WOODBLOCK);	// モデルの情報
		m_bBreak = true;		// 破壊状況

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	// ファイルの情報を取得する
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

//=======================================
// 種類の設定処理
//=======================================
CBlock::TYPE CBlock::GetType(void) const
{
	// 種類を返す
	return m_type;
}

//=======================================
// ドロップの種類の設定処理
//=======================================
CBlock::DROPTYPE CBlock::GetDropType(void) const
{
	// ドロップの種類を返す
	return m_dropType;
}

//=======================================
// 破壊カウントの設定処理
//=======================================
void CBlock::SetBreakCount(const int nCount)
{
	// 破壊カウントを設定する
	m_nBreakCount = nCount;
}

//=======================================
// 破壊カウントの取得処理
//=======================================
int CBlock::GetBreakCount(void) const
{
	// 破壊カウントを返す
	return m_nBreakCount;
}

//=======================================
// 破壊状況の設定処理
//=======================================
void CBlock::SetEnableBreak(const bool bBreak)
{
	// 破壊状況を設定する
	m_bBreak = bBreak;
}

//=======================================
// 破壊状況の取得処理
//=======================================
bool CBlock::IsBreak(void)
{
	// 破壊状況を返す
	return m_bBreak;
}

//=======================================
// 生成処理
//=======================================
CBlock* CBlock::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const TYPE type, const DROPTYPE dropType)
{
	// ローカルオブジェクトを生成
	CBlock* pBlock = nullptr;	// プレイヤーのインスタンスを生成

	if (pBlock == nullptr)
	{ // オブジェクトが NULL の場合

		// ブロックを生成
		pBlock = new CBlock;
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
		pBlock->SetData(pos, rot, scale, type, dropType);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// プレイヤーのポインタを返す
	return pBlock;
}