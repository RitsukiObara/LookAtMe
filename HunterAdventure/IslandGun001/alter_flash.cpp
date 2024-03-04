//=======================================
//
// 祭壇閃光のメイン処理[alter_flash.cpp]
// Author 小原立暉
//
//=======================================
#include "manager.h"
#include "alter_flash.h"
#include "renderer.h"
#include "useful.h"

//---------------------------------------
// 定数定義
//---------------------------------------
namespace
{
	const D3DXVECTOR3 FLASH_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);		// 位置
	const D3DXVECTOR3 FLASH_SIZE = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);	// 位置
	const float DEST_ALPHA = 0.0f;			// 目的の透明度
	const float ALPHA_CORRECT = 0.1f;		// 透明度の補正係数
	const float DEATH_ALPHA = 0.0001f;		// 死ぬときの透明度
}

//=========================
// コンストラクタ
//=========================
CAlterFlash::CAlterFlash() : CObject2D(CObject2D::TYPE_ALTERFLASH, CObject::PRIORITY_BG)
{
	// 全ての値をクリアする
	m_fAlpha = 1.0f;			// 透明度
}

//=========================
// デストラクタ
//=========================
CAlterFlash::~CAlterFlash()
{

}

//=========================
// 初期化処理
//=========================
HRESULT CAlterFlash::Init(void)
{
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する

	// 成功を返す
	return S_OK;
}

//=========================
// 終了処理
//=========================
void CAlterFlash::Uninit(void)
{
	// 終了
	CObject2D::Uninit();
}

//=========================
// 更新処理
//=========================
void CAlterFlash::Update(void)
{
	// 透明に近づけていく
	useful::Correct(DEST_ALPHA, &m_fAlpha, ALPHA_CORRECT);

	if (m_fAlpha <= DEATH_ALPHA)
	{ // 透明度がほぼ0になった場合

		// 終了処理
		Uninit();

		// この先の処理を行わない
		return;
	}

	// 頂点カラーの設定処理
	SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));
}

//=========================
// 描画処理
//=========================
void CAlterFlash::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}
//=========================
// 情報の設定処理
//=========================
void CAlterFlash::SetData(void)
{
	// スクロールの設定処理
	SetPos(FLASH_POS);			// 位置設定
	SetRot(NONE_D3DXVECTOR3);	// 向き設定
	SetSize(FLASH_SIZE);		// サイズ設定
	SetLength();				// 長さ設定
	SetAngle();					// 方向設定

	// 頂点情報の初期化
	SetVertex();

	// 全ての値を設定する
	m_fAlpha = 1.0f;			// 透明度

	// 頂点カラーの設定処理
	SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));
}

//=========================
// 生成処理
//=========================
CAlterFlash* CAlterFlash::Create(void)
{
	// ローカルオブジェクトを生成
	CAlterFlash* pFlash = nullptr;	// 閃光のインスタンスを生成

	if (pFlash == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pFlash = new CAlterFlash;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pFlash != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pFlash->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pFlash->SetData();
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pFlash;
}