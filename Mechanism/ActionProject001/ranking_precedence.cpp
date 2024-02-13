//============================================
//
// ランキングの順位表示処理[ranking_precedence.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "ranking_precedence.h"
#include "object.h"
#include "object2D.h"
#include "texture.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define RANK_HEIGHT_SHIFT		(110.0f)								// スコアの縦のずらす幅
#define RANK_POS				(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 220.0f, 0.0f))		// 順位の位置
#define RANK_TRANS_POS			(D3DXVECTOR3(300.0f, 220.0f, 0.0f))		// 順位の自動遷移の位置
#define RANK_SIZE				(D3DXVECTOR3(80.0f, 40.0f, 0.0f))		// 順位のサイズ
#define RANK_TEXTURE_PATTERN	(0.2f)									// 順位のテクスチャのパターン
#define RANK_TEXTURE			"data/TEXTURE/Rank.png"					// 順位表示のテクスチャ

//========================
// コンストラクタ
//========================
CRankingPrece::CRankingPrece() : CObject(CObject::TYPE_RANKINGPRECE, CObject::PRIORITY_UI)
{
	// 全ての値をクリアする
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		m_apRank[nCnt] = nullptr;				// 時間のポインタ
	}
}

//========================
// デストラクタ
//========================
CRankingPrece::~CRankingPrece()
{

}

//========================
// 初期化処理
//========================
HRESULT CRankingPrece::Init(void)
{
	// ローカル変数宣言
	int nTexIdx = 0;		// テクスチャのインデックス

	// テクスチャの設定処理
	nTexIdx = CManager::Get()->GetTexture()->Regist(RANK_TEXTURE);

	// メモリを確保する
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		// 生成する
		m_apRank[nCnt] = CObject2D::Create(CObject2D::TYPE_NONE, CObject::TYPE_NONE, CObject::PRIORITY_UI);

		if (m_apRank[nCnt] != nullptr)
		{ // ポインタが NULL じゃない場合

			//// 情報の設定
			//if (CManager::GetClear() == true)
			//{ // クリア後の場合

				m_apRank[nCnt]->SetPos(D3DXVECTOR3(RANK_POS.x, RANK_POS.y + (RANK_HEIGHT_SHIFT * nCnt), 0.0f));		// 位置
			//}
			//else
			//{ // 上記以外

			//	m_apRank[nCnt]->SetPos(D3DXVECTOR3(RANK_TRANS_POS.x, RANK_TRANS_POS.y + (RANK_HEIGHT_SHIFT * nCnt), 0.0f));		// 位置
			//}

			m_apRank[nCnt]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));				// 向き
			m_apRank[nCnt]->SetSize(RANK_SIZE);									// サイズ
			m_apRank[nCnt]->SetAngle();											// 方向
			m_apRank[nCnt]->SetLength();										// 長さ

			m_apRank[nCnt]->BindTexture(nTexIdx);								// テクスチャの割り当て処理

			m_apRank[nCnt]->SetVertex();										// 頂点情報の設定処理
			m_apRank[nCnt]->SetVtxTextureAnim(RANK_TEXTURE_PATTERN, nCnt);		// テクスチャ座標の設定処理
		}
	}

	// 成功を返す
	return S_OK;
}

//========================
// 終了処理
//========================
void CRankingPrece::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		// 終了処理
		m_apRank[nCnt]->Uninit();
	}

	// 破棄処理
	Release();
}

//========================
// 更新処理
//========================
void CRankingPrece::Update(void)
{

}

//========================
// 描画処理
//========================
void CRankingPrece::Draw(void)
{
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		// 描画処理
		m_apRank[nCnt]->Draw();
	}
}

//========================
// 生成処理
//========================
CRankingPrece* CRankingPrece::Create(void)
{
	// ローカルオブジェクトを生成
	CRankingPrece* pTime = nullptr;	// プレイヤーのインスタンスを生成

	if (pTime == nullptr)
	{ // オブジェクトが NULL の場合

	  // オブジェクトを生成
		pTime = new CRankingPrece;
	}
	else
	{ // オブジェクトが NULL じゃない場合

	  // NULL を返す
		return nullptr;
	}

	if (pTime != nullptr)
	{ // オブジェクトが NULL じゃない場合

	  // 初期化処理
		if (FAILED(pTime->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "時間の初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}
	}
	else
	{ // オブジェクトが NULL の場合

	  // NULL を返す
		return nullptr;
	}

	// 時間のポインタを返す
	return pTime;
}