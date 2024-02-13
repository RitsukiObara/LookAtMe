//===========================================
//
// 台のメイン処理[table.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "table.h"
#include "table_manager.h"
#include "useful.h"

//==============================
// コンストラクタ
//==============================
CTable::CTable() : CModel(CObject::TYPE_AIRPLANE, CObject::PRIORITY_BLOCK)
{
	// 全ての値をクリアする
	m_pPrev = nullptr;		// 前へのポインタ
	m_pNext = nullptr;		// 次へのポインタ

	if (CTableManager::Get() != nullptr)
	{ // マネージャーが存在していた場合

		// マネージャーへの登録処理
		CTableManager::Get()->Regist(this);
	}
}

//==============================
// デストラクタ
//==============================
CTable::~CTable()
{

}

//============================
// 前のポインタの設定処理
//============================
void CTable::SetPrev(CTable* pPrev)
{
	// 前のポインタを設定する
	m_pPrev = pPrev;
}

//============================
// 後のポインタの設定処理
//============================
void CTable::SetNext(CTable* pNext)
{
	// 次のポインタを設定する
	m_pNext = pNext;
}

//============================
// 前のポインタの設定処理
//============================
CTable* CTable::GetPrev(void) const
{
	// 前のポインタを返す
	return m_pPrev;
}

//============================
// 次のポインタの設定処理
//============================
CTable* CTable::GetNext(void) const
{
	// 次のポインタを返す
	return m_pNext;
}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CTable::Init(void)
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
void CTable::Uninit(void)
{
	// 終了処理
	CModel::Uninit();

	if (CTableManager::Get() != nullptr)
	{ // マネージャーが存在していた場合

		// リスト構造の引き抜き処理
		CTableManager::Get()->Pull(this);
	}

	// リスト構造関係のポインタを NULL にする
	m_pPrev = nullptr;
	m_pNext = nullptr;
}

//========================================
//ブロックの更新処理
//========================================
void CTable::Update(void)
{
	// 前回の位置を設定する
	SetPosOld(GetPos());
}

//=====================================
//ブロックの描画処理
//=====================================
void CTable::Draw(void)
{
	// 描画処理
	CModel::Draw();
}

//=====================================
// 情報の設定処理
//=====================================
void CTable::SetData(const D3DXVECTOR3& pos)
{
	// 情報の設定処理
	SetPos(pos);							// 位置
	SetPosOld(pos);							// 前回の位置
	SetRot(NONE_D3DXVECTOR3);				// 向き
	SetScale(NONE_SCALE);					// 拡大率
	SetFileData(CXFile::TYPE_TABLE);		// モデルの情報
}

//=======================================
// 生成処理
//=======================================
CTable* CTable::Create(const D3DXVECTOR3& pos)
{
	// ローカルオブジェクトを生成
	CTable* pTable = nullptr;	// インスタンスを生成

	if (pTable == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		pTable = new CTable;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pTable != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pTable->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pTable->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 台のポインタを返す
	return pTable;
}