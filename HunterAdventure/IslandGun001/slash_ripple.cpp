//=======================================
//
// 斬撃の波紋のメイン処理[slash_ripple.cpp]
// Author：小原立暉
//
//=======================================
#include "manager.h"
#include "slash_ripple.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

//==========================================
// コンストラクタ
//==========================================
CSlashRipple::CSlashRipple() : CRipple(TYPE_SLASHRIPPLE, PRIORITY_PLAYER)
{

}

//==========================================
// デストラクタ
//==========================================
CSlashRipple::~CSlashRipple()
{

}

//==========================================
// 波紋の初期化処理
//==========================================
HRESULT CSlashRipple::Init(void)
{
	if (FAILED(CRipple::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//========================================
// 波紋の終了処理
//========================================
void CSlashRipple::Uninit(void)
{
	// 終了処理
	CRipple::Uninit();
}

//========================================
// 波紋の更新処理
//========================================
void CSlashRipple::Update(void)
{
	// 更新処理
	CRipple::Update();
}

//===========================================
// 波紋の描画処理
//===========================================
void CSlashRipple::Draw(void)
{
	// 描画処理
	CRipple::Draw();
}

//===========================================
// 情報の設定処理
//===========================================
void CSlashRipple::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// 情報の設定処理
	CRipple::SetData(pos, rot, NONE_SCALE);
}

//===========================================
// 生成処理
//===========================================
CSlashRipple* CSlashRipple::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// ローカルオブジェクトを生成
	CSlashRipple* pRipple = nullptr;	// 波紋のインスタンスを生成

	if (pRipple == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pRipple = new CSlashRipple;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pRipple != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pRipple->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pRipple->SetData(pos, rot);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// 波紋のポインタを返す
	return pRipple;
}