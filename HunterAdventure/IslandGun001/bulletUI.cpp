//============================================
//
// 残弾UI処理[bulletUI.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "bulletUI.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

#include "object2D.h"

//--------------------------------------------
// 無名名前空間
//--------------------------------------------
namespace
{
	const D3DXVECTOR3 INIT_BULLET_POS = D3DXVECTOR3(80.0f, 17.0f, 0.0f);		// 最初の弾丸の位置
	const D3DXVECTOR3 BULLET_SIZE = D3DXVECTOR3(7.0f, 14.0f, 0.0f);				// 弾丸のサイズ
	const float BULLET_SHIFT_X = 15.0f;											// 弾丸のずらす幅(X座標)
	const char* BULLET_TEXTURE = "data\\TEXTURE\\RestBullet.png";				// 弾丸のテクスチャ
}

//========================
// コンストラクタ
//========================
CBulletUI::CBulletUI() : CObject(TYPE_BULLETUI, DIM_2D, PRIORITY_UI)
{
	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < MAX_REMAINING_BULLET; nCnt++)
	{
		m_aBullet[nCnt].pBullet = nullptr;		// 残弾の情報
		m_aBullet[nCnt].bDisp = true;			// 描画状況
	}

	m_pReload = nullptr;						// リロードの文字
	m_nNumBullet = MAX_REMAINING_BULLET;		// 残弾数
}

//========================
// デストラクタ
//========================
CBulletUI::~CBulletUI()
{

}

//========================
// 初期化処理
//========================
HRESULT CBulletUI::Init(void)
{
	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < MAX_REMAINING_BULLET; nCnt++)
	{
		if (m_aBullet[nCnt].pBullet == nullptr)
		{ // 残弾が NULL の場合

			// 弾丸の情報を生成する
			m_aBullet[nCnt].pBullet = CObject2D::Create(CObject2D::TYPE_NONE, TYPE_NONE, PRIORITY_UI);
		}

		m_aBullet[nCnt].bDisp = true;			// 描画状況
	}

	// リロードの文字を生成する
	m_pReload = CObject2D::Create(CObject2D::TYPE_NONE, TYPE_NONE, PRIORITY_UI);

	// 成功を返す
	return S_OK;
}

//========================
// 終了処理
//========================
void CBulletUI::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_REMAINING_BULLET; nCnt++)
	{
		if (m_aBullet[nCnt].pBullet != nullptr)
		{ // 残弾の情報が NULL じゃない場合

			// 残弾の終了処理
			m_aBullet[nCnt].pBullet->Uninit();
			m_aBullet[nCnt].pBullet = nullptr;
		}
	}

	if (m_pReload != nullptr)
	{ // リロードの文字が NULL じゃない場合

		// リロードの文字の終了処理
		m_pReload->Uninit();
		m_pReload = nullptr;
	}

	// 数字の終了処理
	Release();
}

//========================
// 更新処理
//========================
void CBulletUI::Update(void)
{

}

//========================
// 描画処理
//========================
void CBulletUI::Draw(void)
{
	for (int nCnt = 0; nCnt < m_nNumBullet; nCnt++)
	{
		if (m_aBullet[nCnt].pBullet != nullptr &&
			m_aBullet[nCnt].bDisp == true)
		{ // 残弾が描画できる状態の場合

			// 描画処理
			m_aBullet[nCnt].pBullet->Draw();
		}
	}

	if (m_pReload != nullptr)
	{ // リロードの文字が NULL じゃない場合

		// 描画処理
		//m_pReload->Draw();
	}
}

//========================
// 情報の設定処理
//========================
void CBulletUI::SetData(void)
{
	D3DXVECTOR3 posBullet;			// 弾丸の位置

	for (int nCnt = 0; nCnt < MAX_REMAINING_BULLET; nCnt++)
	{
		if (m_aBullet[nCnt].pBullet != nullptr)
		{
			// 弾丸の位置を設定する
			posBullet = D3DXVECTOR3(INIT_BULLET_POS.x + (BULLET_SHIFT_X * nCnt), INIT_BULLET_POS.y, 0.0f);

			// 設定処理
			m_aBullet[nCnt].pBullet->SetPos(posBullet);			// 位置
			m_aBullet[nCnt].pBullet->SetPosOld(posBullet);		// 前回の位置
			m_aBullet[nCnt].pBullet->SetRot(NONE_D3DXVECTOR3);	// 向き
			m_aBullet[nCnt].pBullet->SetSize(BULLET_SIZE);		// サイズ
			m_aBullet[nCnt].pBullet->SetAngle();				// 方向
			m_aBullet[nCnt].pBullet->SetLength();				// 長さ

			// テクスチャの割り当て処理
			m_aBullet[nCnt].pBullet->BindTexture(CManager::Get()->GetTexture()->Regist(BULLET_TEXTURE));

			// 頂点座標の設定処理
			m_aBullet[nCnt].pBullet->SetVertex();
		}
	}
}

//========================
// 生成処理
//========================
CBulletUI* CBulletUI::Create(void)
{
	// ローカルオブジェクトを生成
	CBulletUI* pUI = nullptr;	// UIのインスタンスを生成

	if (pUI == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pUI = new CBulletUI;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pUI != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pUI->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pUI->SetData();
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// UIのポインタを返す
	return pUI;
}

//========================
// 残弾数の設定処理
//========================
void CBulletUI::SetNumBullet(const int nNum)
{
	// 残弾数を設定する
	m_nNumBullet = nNum;

	if (m_nNumBullet < 0)
	{ // 残弾数が0未満になった場合

		// 残弾数を0にする
		m_nNumBullet = 0;
	}
}

//========================
// 残弾数の取得処理
//========================
int CBulletUI::GetNumBullet(void) const
{
	// 残弾数を返す
	return m_nNumBullet;
}