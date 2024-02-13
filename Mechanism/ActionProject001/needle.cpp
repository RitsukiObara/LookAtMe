//===========================================
//
// 棘のメイン処理[needle.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "needle.h"
#include "needle_manager.h"
#include "useful.h"

//==============================
// コンストラクタ
//==============================
CNeedle::CNeedle() : CModel(CObject::TYPE_PORK, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_pPrev = nullptr;		// 前へのポインタ
	m_pNext = nullptr;		// 次へのポインタ

	if (CNeedleManager::Get() != nullptr)
	{ // マネージャーが存在していた場合

		// マネージャーへの登録処理
		CNeedleManager::Get()->Regist(this);
	}
}

//==============================
// デストラクタ
//==============================
CNeedle::~CNeedle()
{

}

//============================
// 前のポインタの設定処理
//============================
void CNeedle::SetPrev(CNeedle* pPrev)
{
	// 前のポインタを設定する
	m_pPrev = pPrev;
}

//============================
// 後のポインタの設定処理
//============================
void CNeedle::SetNext(CNeedle* pNext)
{
	// 次のポインタを設定する
	m_pNext = pNext;
}

//============================
// 前のポインタの設定処理
//============================
CNeedle* CNeedle::GetPrev(void) const
{
	// 前のポインタを返す
	return m_pPrev;
}

//============================
// 次のポインタの設定処理
//============================
CNeedle* CNeedle::GetNext(void) const
{
	// 次のポインタを返す
	return m_pNext;
}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CNeedle::Init(void)
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
void CNeedle::Uninit(void)
{
	// 終了処理
	CModel::Uninit();

	if (CNeedleManager::Get() != nullptr)
	{ // マネージャーが存在していた場合

		// リスト構造の引き抜き処理
		CNeedleManager::Get()->Pull(this);
	}

	// リスト構造関係のポインタを NULL にする
	m_pPrev = nullptr;
	m_pNext = nullptr;
}

//========================================
//ブロックの更新処理
//========================================
void CNeedle::Update(void)
{

}

//=====================================
//ブロックの描画処理
//=====================================
void CNeedle::Draw(void)
{
	// 描画処理
	CModel::Draw();
}

//=====================================
// 情報の設定処理
//=====================================
void CNeedle::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3 rot)
{
	// 情報の設定処理
	SetPos(pos);							// 位置
	SetPosOld(pos);							// 前回の位置
	SetRot(rot);							// 向き
	SetScale(NONE_SCALE);					// 拡大率
	SetFileData(CXFile::TYPE_NEEDLE);		// モデルの情報
}

//=======================================
// 生成処理
//=======================================
CNeedle* CNeedle::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3 rot)
{
	// ローカルオブジェクトを生成
	CNeedle* pPork = nullptr;	// インスタンスを生成

	if (pPork == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		pPork = new CNeedle;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pPork != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pPork->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pPork->SetData(pos, rot);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// ポークのポインタを返す
	return pPork;
}