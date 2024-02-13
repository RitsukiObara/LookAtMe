//============================================
//
// ランキングスコア処理[ranking_score.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "ranking_score.h"
#include "manager.h"
#include "time.h"
#include "sound.h"

#include "file.h"
#include "new_record.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define SCORE_WIDTH_SHIFT		(70.0f)									// スコアの横のずらす幅
#define SCORE_HEIGHT_SHIFT		(110.0f)								// スコアの縦のずらす幅
#define SCORE_SIZE				(D3DXVECTOR3(30.0f, 40.0f, 0.0f))		// スコアのサイズ
#define SCORE_POS				(D3DXVECTOR3(SCREEN_WIDTH * 0.5f + 150.0f, 220.0f, 0.0f))		// スコアの位置
#define SCORE_TRANS_POS			(D3DXVECTOR3(500.0f, 220.0f, 0.0f))		// 自動遷移後のスコアの位置
#define NO_NEW_RECORD			(0)										// 新記録じゃないときの数値
#define SCOREDOT_SIZE			(D3DXVECTOR3(8.0f, 8.0f, 0.0f))			// スコアの点のサイズ
#define SOUND_COUNT				(70)									// 音が鳴るカウント数

//========================
// コンストラクタ
//========================
CRankingScore::CRankingScore() : CObject(CObject::TYPE_RANKINGSCORE, CObject::PRIORITY_UI)
{
	// 全ての値をクリアする
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		m_apTime[nCnt] = nullptr;				// 時間のポインタ
	}

	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_nNewRecordRank = NO_NEW_RECORD;			// 新記録の順位
	m_nSoundCount = 0;							// 音を鳴らすカウント
}

//========================
// デストラクタ
//========================
CRankingScore::~CRankingScore()
{

}

//========================
// 初期化処理
//========================
HRESULT CRankingScore::Init(void)
{
	// メモリを確保する
	for (int nCnt = 0; nCnt < MAX_TIME_DIGIT; nCnt++)
	{
		if (m_apTime[nCnt] == nullptr)
		{ // ポインタが NULL の場合

			// メモリを確保する
			m_apTime[nCnt] = new CTime(CObject::TYPE_NONE, CObject::PRIORITY_UI);
		}
		else
		{ // ポインタが NULL じゃない場合

			// 警告文
			MessageBox(NULL, "時間のメモリが既に使われている！", "警告！", MB_ICONWARNING);

			// 失敗を返す
			return E_FAIL;
		}

		if (m_apTime[nCnt] != nullptr)
		{ // ポインタが NULL じゃない場合

			// 初期化処理
			if (FAILED(m_apTime[nCnt]->Init()))
			{ // 初期化処理に失敗した場合

				// 警告文
				MessageBox(NULL, "時間の初期化に失敗！", "警告！", MB_ICONWARNING);

				// 失敗を返す
				return E_FAIL;
			}
		}
		else
		{ // ポインタが　NULL の場合

			// 警告文
			MessageBox(NULL, "時間のメモリの確保に失敗！", "警告！", MB_ICONWARNING);

			// 失敗を返す
			return E_FAIL;
		}
	}

	if (CManager::GetClear() == true)
	{ // クリア後の場合

		// 位置
		m_pos = SCORE_POS;
	}
	else
	{ // 上記以外

		// 位置
		m_pos = SCORE_TRANS_POS;
	}
	// 全ての値を初期化する
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
	m_nNewRecordRank = NO_NEW_RECORD;			// 新記録の順位
	m_nSoundCount = 0;							// 音を鳴らすカウント

	// 成功を返す
	return S_OK;
}

//========================
// 終了処理
//========================
void CRankingScore::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		if (m_apTime[nCnt] != nullptr)
		{ // タイマーの情報が NULL じゃない場合

			// 終了処理
			m_apTime[nCnt]->Uninit();
			m_apTime[nCnt] = nullptr;
		}
	}

	// 破棄処理
	Release();
}

//========================
// 更新処理
//========================
void CRankingScore::Update(void)
{
	if (m_nNewRecordRank > NO_NEW_RECORD && 
		m_nNewRecordRank <= MAX_RANKING &&
		m_apTime[m_nNewRecordRank - 1] != nullptr)
	{ // 新記録を取ったかつ、数字のポインタがあった場合

		for (int nCnt = 0; nCnt < MAX_TIME_DIGIT; nCnt++)
		{
			// 色を設定する
			m_apTime[m_nNewRecordRank - 1]->GetNumber(nCnt)->SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		}
	}

	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		// 1桁ごとの数字設定処理
		m_apTime[nCnt]->SetNumber();

		// 1桁ごとのテクスチャ設定処理
		m_apTime[nCnt]->SetTexture();
	}

	if (m_nNewRecordRank != NO_NEW_RECORD)
	{ // 新記録を取っていた場合

		// 音を鳴らすカウントを加算する
		m_nSoundCount++;

		if (m_nSoundCount % SOUND_COUNT == 0)
		{ // カウント数が一定数に達した場合

			// 新記録の音を鳴らす
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_NEWRECORD);
		}
	}
}

//========================
// 描画処理
//========================
void CRankingScore::Draw(void)
{
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		// 描画処理
		m_apTime[nCnt]->Draw();
	}
}

//========================
// 情報の設定処理
//========================
void CRankingScore::SetData(void)
{
	// ランキングのロード処理
	CManager::GetFile()->Load(CFile::TYPE_RANKING);

	if (CManager::GetClear() == true)
	{ // クリア状況が true だった場合

		// ランキングの入れ替え処理
		RankSwap(CManager::GetClearTime());

		// 順位のセーブ処理
		CManager::GetFile()->Save(CFile::TYPE_RANKING);
	}

	// 位置を設定する
	D3DXVECTOR3 pos;

	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		// 位置を設定する
		pos.x = m_pos.x;
		pos.y = m_pos.y + (nCntRank * SCORE_HEIGHT_SHIFT);
		pos.z = 0.0f;

		m_apTime[nCntRank]->SetData(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCORE_SIZE, SCOREDOT_SIZE, SCORE_WIDTH_SHIFT);

		// 結果を代入する
		m_apTime[nCntRank]->SetTotalTime(CManager::GetFile()->GetRankingInfo().aRank[nCntRank]);

		// 数値の設定処理
		m_apTime[nCntRank]->SetNumber();

		// 1桁ごとのテクスチャの設定処理
		m_apTime[nCntRank]->SetTexture();

		if (nCntRank == m_nNewRecordRank - 1)
		{ // 新記録だった場合

			// 新記録表示の生成
			CNewRecord::Create(m_apTime[nCntRank]->GetNumber(0)->GetPos());
		}
	}
}

//========================================
// ランキングの入れ替え処理
//========================================
void CRankingScore::RankSwap(int nScore)
{
	// ローカル変数宣言
	int aScore[MAX_RANKING];		// スコア
	int nTemp;						// 一時的な変数
	bool bUpdate = false;			// 新記録が更新されたかどうか

	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		aScore[nCnt] = CManager::GetFile()->GetRankingInfo().aRank[nCnt];
	}

	for (int nCount = 0; nCount < MAX_RANKING; nCount++)
	{ // ランキングを比較する

		if (aScore[nCount] >= nScore)
		{ // クリアタイムの数値の方が小さかった場合

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
	CManager::GetFile()->SetRankingInfo(aScore);
}

//========================
// 生成処理
//========================
CRankingScore* CRankingScore::Create(void)
{
	// ローカルオブジェクトを生成
	CRankingScore* pTime = nullptr;	// プレイヤーのインスタンスを生成

	if (pTime == nullptr)
	{ // オブジェクトが NULL の場合

	  // オブジェクトを生成
		pTime = new CRankingScore;
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