//===========================================
//
// イワカリの殻のメイン処理[iwakari_shell.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "iwakari_shell.h"
#include "renderer.h"
#include "useful.h"

//-------------------------------------------
// 無名名前空間
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\IwakariShell.x";	// イワカリの殻の名前
	const int LIFE = 80;								// 殻の寿命
}

//==============================
// コンストラクタ
//==============================
CIwakariShell::CIwakariShell() : CModel(CObject::TYPE_NONE, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_nLife = 0;		// 体力
}

//==============================
// デストラクタ
//==============================
CIwakariShell::~CIwakariShell()
{

}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CIwakariShell::Init(void)
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
void CIwakariShell::Uninit(void)
{
	// 終了処理
	CModel::Uninit();
}

//========================================
//ブロックの更新処理
//========================================
void CIwakariShell::Update(void)
{

}

//=====================================
//ブロックの描画処理
//=====================================
void CIwakariShell::Draw(void)
{
	// 描画処理
	CModel::Draw();
}

//=====================================
// 情報の設定処理
//=====================================
void CIwakariShell::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// 情報の設定処理
	SetPos(pos);						// 位置
	SetPosOld(pos);						// 前回の位置
	SetRot(rot);						// 向き
	SetScale(NONE_SCALE);				// 拡大率
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));		// モデルの情報

	// 全ての値を設定する
	m_nLife = LIFE;			// 体力
}

//=======================================
// 生成処理
//=======================================
CIwakariShell* CIwakariShell::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// ローカルオブジェクトを生成
	CIwakariShell* pShell = nullptr;	// インスタンスを生成

	if (pShell == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		pShell = new CIwakariShell;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pShell != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pShell->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pShell->SetData(pos, rot);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 殻のポインタを返す
	return pShell;
}

//=======================================
// 体力の設定処理
//=======================================
void CIwakariShell::SetLife(const int nLife)
{
	// 体力を設定する
	m_nLife = nLife;
}

//=======================================
// 体力の取得処理
//=======================================
int CIwakariShell::GetLife(void) const
{
	// 体力を返す
	return m_nLife;
}