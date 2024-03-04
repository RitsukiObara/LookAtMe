//===========================================
//
// 風船の紐のメイン処理[balloon_rope.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "balloon_rope.h"
#include "useful.h"

//-------------------------------------------
// 定数定義
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\Balloon_rope.x";			// 紐モデルの名前
	const D3DXVECTOR3 POS = D3DXVECTOR3(0.0f, -75.0f, 0.0f);	// 位置
}

//==============================
// コンストラクタ
//==============================
CBalloonRope::CBalloonRope() : CModel(CObject::TYPE_NONE, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_pMtxParent = nullptr;			// 親のマトリックス
}

//==============================
// デストラクタ
//==============================
CBalloonRope::~CBalloonRope()
{

}

//==============================
// 破片の初期化処理
//==============================
HRESULT CBalloonRope::Init(void)
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
// 破片の終了処理
//========================================
void CBalloonRope::Uninit(void)
{
	// 親のマトリックスを NULL にする
	m_pMtxParent = nullptr;

	// 終了処理
	CModel::Uninit();
}

//=====================================
// 破片の更新処理
//=====================================
void CBalloonRope::Update(void)
{

}

//=====================================
// 破片の描画処理
//=====================================
void CBalloonRope::Draw(void)
{
	// 描画処理
	CModel::DrawMatrix(*m_pMtxParent);
}

//=====================================
// 情報の設定処理
//=====================================
void CBalloonRope::SetData(D3DXMATRIX* mtx)
{
	// 情報の設定処理
	SetPos(POS);					// 位置
	SetPosOld(POS);					// 前回の位置
	SetRot(NONE_D3DXVECTOR3);		// 向き
	SetScale(NONE_SCALE);			// 拡大率
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));	// モデル情報

	// 全ての値をクリアする
	m_pMtxParent = mtx;				// 親のマトリックス
}

//=======================================
// 生成処理
//=======================================
CBalloonRope* CBalloonRope::Create(D3DXMATRIX* mtx)
{
	// ローカルオブジェクトを生成
	CBalloonRope* pRope = nullptr;		// インスタンスを生成

	if (pRope == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		pRope = new CBalloonRope;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pRope != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pRope->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pRope->SetData(mtx);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 紐のポインタを返す
	return pRope;
}