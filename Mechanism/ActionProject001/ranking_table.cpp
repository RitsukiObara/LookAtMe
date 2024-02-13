//============================================
//
// ランキングスコア処理[ranking_score.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "ranking_table.h"
#include "manager.h"
#include "sound.h"

#include "file.h"
#include "score.h"
#include "game.h"
#include "new_record.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define SCORE_WIDTH_SHIFT		(70.0f)									// スコアの横のずらす幅
#define SCORE_HEIGHT_SHIFT		(110.0f)								// スコアの縦のずらす幅
#define SCORE_SIZE				(D3DXVECTOR3(30.0f, 40.0f, 0.0f))		// スコアのサイズ
#define SCORE_SHIFT				(60.0f)									// スコアのサイズ
#define SCORE_POS				(D3DXVECTOR3(SCREEN_WIDTH * 0.5f + 150.0f, 220.0f, 0.0f))		// スコアの位置
#define SCORE_TRANS_POS			(D3DXVECTOR3(500.0f, 220.0f, 0.0f))		// 自動遷移後のスコアの位置
#define NO_NEW_RECORD			(0)										// 新記録じゃないときの数値
#define SCOREDOT_SIZE			(D3DXVECTOR3(8.0f, 8.0f, 0.0f))			// スコアの点のサイズ
#define SOUND_COUNT				(70)									// 音が鳴るカウント数

//========================
// コンストラクタ
//========================
CRankingTable::CRankingTable() : CObject(CObject::TYPE_RANKINGTABLE, CObject::PRIORITY_UI)
{
	// 全ての値をクリアする
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		m_apScore[nCnt] = nullptr;				// 時間のポインタ
	}

	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
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
	// メモリを確保する
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		if (m_apScore[nCnt] == nullptr)
		{ // ポインタが NULL の場合

			// メモリを確保する
			m_apScore[nCnt] = new CScore(CObject::TYPE_NONE, CObject::PRIORITY_UI);
		}
		else
		{ // ポインタが NULL じゃない場合

			// 停止
			assert(false);

			// 失敗を返す
			return E_FAIL;
		}

		if (m_apScore[nCnt] != nullptr)
		{ // ポインタが NULL じゃない場合

			// 初期化処理
			if (FAILED(m_apScore[nCnt]->Init()))
			{ // 初期化処理に失敗した場合

				// 停止
				assert(false);

				// 失敗を返す
				return E_FAIL;
			}
		}
		else
		{ // ポインタが　NULL の場合

			// 停止
			assert(false);

			// 失敗を返す
			return E_FAIL;
		}
	}

	// 全ての値を初期化する
	m_pos = SCORE_POS;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
	m_nNewRecordRank = NO_NEW_RECORD;			// 新記録の順位
	m_nSoundCount = 0;							// 音を鳴らすカウント

	// 成功を返す
	return S_OK;
}

//========================
// 終了処理
//========================
void CRankingTable::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
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
		m_nNewRecordRank <= MAX_RANKING &&
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
			CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_NEWRECORD);
		}
	}
}

//========================
// 描画処理
//========================
void CRankingTable::Draw(void)
{
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
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
	// ランキングのロード処理
	CManager::Get()->GetFile()->Load(CFile::TYPE_RANKING);

	// ランキングの入れ替え処理
	RankSwap(CGame::GetScore());

	// 順位のセーブ処理
	CManager::Get()->GetFile()->Save(CFile::TYPE_RANKING);

	// 位置を設定する
	D3DXVECTOR3 pos;

	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		// 位置を設定する
		pos.x = m_pos.x;
		pos.y = m_pos.y + (nCntRank * SCORE_HEIGHT_SHIFT);
		pos.z = 0.0f;

		// スコアの情報の設定処理
		m_apScore[nCntRank]->SetData(pos, NONE_D3DXVECTOR3, SCORE_SIZE, SCORE_SHIFT);

		// スコアを設定する
		m_apScore[nCntRank]->SetScore(CManager::Get()->GetFile()->GetRankingInfo().aRank[nCntRank]);
		m_apScore[nCntRank]->SetScoreDisp(CManager::Get()->GetFile()->GetRankingInfo().aRank[nCntRank]);

		// テクスチャの設定処理
		m_apScore[nCntRank]->Update();

		if (nCntRank == m_nNewRecordRank - 1)
		{ // 新記録だった場合

			// 新記録表示の生成
			CNewRecord::Create(m_apScore[nCntRank]->GetNumber(0)->GetPos());
		}
	}
}

//========================================
// ランキングの入れ替え処理
//========================================
void CRankingTable::RankSwap(int nScore)
{
	// ローカル変数宣言
	int aScore[MAX_RANKING];		// スコア
	int nTemp;						// 一時的な変数
	bool bUpdate = false;			// 新記録が更新されたかどうか

	// ランキングを取得する
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		aScore[nCnt] = CManager::Get()->GetFile()->GetRankingInfo().aRank[nCnt];
	}

	for (int nCount = 0; nCount < MAX_RANKING; nCount++)
	{ // ランキングを比較する

		if (aScore[nCount] <= nScore)
		{ // 今回のスコアの数値の方が大きかった場合

			// 一時的な変数にランキングの数値を代入する
			nTemp = aScore[nCount];

			// 結果をランキングの中に代入する
			aScore[nCount] = nScore;

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
	CManager::Get()->GetFile()->SetRankingInfo(aScore);
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

			// 警告文
			MessageBox(NULL, "時間の初期化に失敗！", "警告！", MB_ICONWARNING);

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