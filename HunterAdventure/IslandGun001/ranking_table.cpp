//============================================
//
// ランキングスコア処理[ranking_score.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "ranking_table.h"
#include "file.h"
#include "game.h"
#include "sound.h"

#include "rank_score.h"

//--------------------------------------------
// 定数定義
//--------------------------------------------
namespace
{
	const float SCORE_HEIGHT_SHIFT = 120.0f;		// スコアの縦のずらす幅
	const int NO_NEW_RECORD = 0;					// 新記録じゃないときの数値
	const int SOUND_COUNT = 70;						// 音が鳴るカウント数
	const D3DXVECTOR3 SCORE_POS = D3DXVECTOR3(350.0f, 140.0f, 0.0f);			// スコアの位置
	const D3DXVECTOR3 SCORE_TRANS_POS = D3DXVECTOR3(500.0f, 220.0f, 0.0f);		// 自動遷移後のスコアの位置
}


//========================
// コンストラクタ
//========================
CRankingTable::CRankingTable() : CObject(CObject::TYPE_RANKINGTABLE, DIM_2D, CObject::PRIORITY_UI)
{
	// 全ての値をクリアする
	for (int nCnt = 0; nCnt < CRanking::MAX_RANKING; nCnt++)
	{
		m_apScore[nCnt] = nullptr;				// 時間のポインタ
	}

	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
	m_nNewRecordRank = NO_NEW_RECORD;			// 新記録の順位
	m_nSoundCount = 0;							// 音を鳴らすカウント
}

//========================
// デストラクタ
//========================
CRankingTable::~CRankingTable()
{

}

//========================
// 初期化処理
//========================
HRESULT CRankingTable::Init(void)
{
	// 成功を返す
	return S_OK;
}

//========================
// 終了処理
//========================
void CRankingTable::Uninit(void)
{
	for (int nCnt = 0; nCnt < CRanking::MAX_RANKING; nCnt++)
	{
		if (m_apScore[nCnt] != nullptr)
		{ // タイマーの情報が NULL じゃない場合

			// 終了処理
			m_apScore[nCnt]->Uninit();
			m_apScore[nCnt] = nullptr;
		}
	}

	// 破棄処理
	Release();
}

//========================
// 更新処理
//========================
void CRankingTable::Update(void)
{
	if (m_nNewRecordRank > NO_NEW_RECORD && 
		m_nNewRecordRank <= CRanking::MAX_RANKING &&
		m_apScore[m_nNewRecordRank - 1] != nullptr)
	{ // 新記録を取ったかつ、数字のポインタがあった場合

		for (int nCnt = 0; nCnt < MAX_SCORE_DIGIT; nCnt++)
		{
			// 色を設定する
			m_apScore[m_nNewRecordRank - 1]->GetNumber(nCnt)->SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		}
	}

	if (m_nNewRecordRank != NO_NEW_RECORD)
	{ // 新記録を取っていた場合

		// 音を鳴らすカウントを加算する
		m_nSoundCount++;

		if (m_nSoundCount % SOUND_COUNT == 0)
		{ // カウント数が一定数に達した場合

			// 新記録の音を鳴らす
			//CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_NEWRECORD);
		}
	}
}

//========================
// 描画処理
//========================
void CRankingTable::Draw(void)
{
	for (int nCnt = 0; nCnt < CRanking::MAX_RANKING; nCnt++)
	{
		// 描画処理
		m_apScore[nCnt]->Draw();
	}
}

//========================
// 情報の設定処理
//========================
void CRankingTable::SetData(void)
{
	// 得点と位置
	D3DXVECTOR3 pos;
	int aScore[CRanking::MAX_RANKING];

	// ランキングのロード処理
	CManager::Get()->GetFile()->Load(CFile::TYPE_RANKING);

	// ランキングの入れ替え処理
	RankSwap(CGame::GetScore(), aScore);

	// 順位のセーブ処理
	CManager::Get()->GetFile()->Save(CFile::TYPE_RANKING);

	for (int nCntRank = 0; nCntRank < CRanking::MAX_RANKING; nCntRank++)
	{
		// 位置を設定する
		pos = D3DXVECTOR3(SCORE_POS.x, SCORE_POS.y + (SCORE_HEIGHT_SHIFT * nCntRank), 0.0f);

		if (m_apScore[nCntRank] == nullptr)
		{ // スコアが NULL の場合

			// スコアを生成する
			m_apScore[nCntRank] = CRankScore::Create(pos, aScore[nCntRank]);
		}

		//if (nCntRank == m_nNewRecordRank - 1)
		//{ // 新記録だった場合

		//	// 新記録表示の生成
		//	CNewRecord::Create(m_apScore[nCntRank]->GetNumber(0)->GetPos());
		//}
	}
}

//========================================
// ランキングの入れ替え処理
//========================================
void CRankingTable::RankSwap(int nScore, int* aSave)
{
	// ローカル変数宣言
	int nTemp;							// 一時的な変数
	bool bUpdate = false;				// 新記録が更新されたかどうか

	// ランキングを取得する
	for (int nCnt = 0; nCnt < CRanking::MAX_RANKING; nCnt++)
	{
		aSave[nCnt] = CManager::Get()->GetFile()->GetRankingInfo().aRank[nCnt];
	}

	for (int nCount = 0; nCount < CRanking::MAX_RANKING; nCount++)
	{ // ランキングを比較する

		if (aSave[nCount] <= nScore)
		{ // 今回のスコアの数値の方が大きかった場合

			// 一時的な変数にランキングの数値を代入する
			nTemp = aSave[nCount];

			// 結果をランキングの中に代入する
			aSave[nCount] = nScore;

			// 結果を一時的な変数に変える
			nScore = nTemp;

			if (bUpdate == false)
			{ // 新記録が更新されていなかった場合

				// 今のカウントを新記録の番号とする
				m_nNewRecordRank = nCount + 1;

				// 新記録更新
				bUpdate = true;
			}
		}
	}

	// ランキングの設定処理
	CManager::Get()->GetFile()->SetRankingInfo(aSave);
}

//========================
// 生成処理
//========================
CRankingTable* CRankingTable::Create(void)
{
	// ローカルオブジェクトを生成
	CRankingTable* pTime = nullptr;	// プレイヤーのインスタンスを生成

	if (pTime == nullptr)
	{ // オブジェクトが NULL の場合

	  // オブジェクトを生成
		pTime = new CRankingTable;
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

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pTime->SetData();
	}
	else
	{ // オブジェクトが NULL の場合

	  // NULL を返す
		return nullptr;
	}

	// 時間のポインタを返す
	return pTime;
}