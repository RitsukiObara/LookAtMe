//============================================
//
// 順位UI処理[ranking_rank.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "ranking_rank.h"
#include "texture.h"

#include "object2D.h"

//--------------------------------------------
// 無名名前空間
//--------------------------------------------
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\Rank.png";				// テクスチャ
	const D3DXVECTOR3 INIT_POS = D3DXVECTOR3(200.0f, 140.0f, 0.0f);	// 初期位置
	const D3DXVECTOR3 UI_SIZE = D3DXVECTOR3(80.0f, 40.0f, 0.0f);	// サイズ
	const float SHIFT = 120.0f;					// ずらす高さ
	const float TEXTURE_PATTERN = 0.2f;			// テクスチャのパターン
}

//========================
// コンストラクタ
//========================
CRankUI::CRankUI() : CObject(TYPE_RANKUI, DIM_2D, PRIORITY_UI)
{
	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		m_apRankUI[nCnt] = nullptr;
	}
}

//========================
// デストラクタ
//========================
CRankUI::~CRankUI()
{

}

//========================
// 初期化処理
//========================
HRESULT CRankUI::Init(void)
{
	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		if (m_apRankUI[nCnt] == nullptr)
		{ // 順位UIが NULL の場合

			// 順位UIを生成
			m_apRankUI[nCnt] = CObject2D::Create(CObject2D::TYPE_NONE, TYPE_NONE, PRIORITY_UI);
		}
	}

	// 成功を返す
	return S_OK;
}

//========================
// 終了処理
//========================
void CRankUI::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		if (m_apRankUI[nCnt] != nullptr)
		{ // 順位UIが NULL じゃない場合

			// 終了処理
			m_apRankUI[nCnt]->Uninit();
			m_apRankUI[nCnt] = nullptr;
		}
	}

	// 数字の終了処理
	Release();
}

//========================
// 更新処理
//========================
void CRankUI::Update(void)
{

}

//========================
// 描画処理
//========================
void CRankUI::Draw(void)
{
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		if (m_apRankUI[nCnt] != nullptr)
		{ // 順位UIが NULL じゃない場合
			 
			// 描画処理
			m_apRankUI[nCnt]->Draw();
		}
	}
}

//========================
// 情報の設定処理
//========================
void CRankUI::SetData(void)
{
	// UIの位置
	D3DXVECTOR3 posUI = NONE_D3DXVECTOR3;

	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		if (m_apRankUI[nCnt] != nullptr)
		{ // 順位UIが NULL じゃない場合

			// 位置を設定する
			posUI.x = INIT_POS.x;
			posUI.y = INIT_POS.y + (SHIFT * nCnt);
			posUI.z = INIT_POS.z;

			// 設定処理
			m_apRankUI[nCnt]->SetPos(posUI);				// 位置
			m_apRankUI[nCnt]->SetPosOld(posUI);				// 前回の位置
			m_apRankUI[nCnt]->SetRot(NONE_D3DXVECTOR3);		// 向き
			m_apRankUI[nCnt]->SetSize(UI_SIZE);				// サイズ
			m_apRankUI[nCnt]->SetAngle();					// 方向
			m_apRankUI[nCnt]->SetLength();					// 長さ

			// テクスチャの割り当て処理
			m_apRankUI[nCnt]->BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE));

			// 頂点座標の設定処理
			m_apRankUI[nCnt]->SetVertex();

			// テクスチャ座標の設定処理
			m_apRankUI[nCnt]->SetVtxTextureAnim(TEXTURE_PATTERN, nCnt);
		}
	}
}

//========================
// 生成処理
//========================
CRankUI* CRankUI::Create(void)
{
	// ローカルオブジェクトを生成
	CRankUI* pRank = nullptr;	// UIのインスタンスを生成

	if (pRank == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pRank = new CRankUI;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pRank != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pRank->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pRank->SetData();
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// UIのポインタを返す
	return pRank;
}