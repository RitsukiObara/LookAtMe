//============================================
//
// リザルトスコア処理[result_score.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "result.h"
#include "manager.h"
#include "result_score.h"
#include "result_rank.h"
#include "file.h"
#include "renderer.h"
#include "texture.h"
#include "sound.h"
#include "useful.h"

// マクロ定義
#define RESULTSCORE_TEXTURE			"data/TEXTURE/Number.png"				// リザルトスコアのテクスチャ
#define RESULTSCORE_WIDTH_SHIFT		(120.0f)								// リザルトスコアの横のずらす幅
#define RESULTSCORE_SIZE			(D3DXVECTOR3(60.0f, 80.0f, 0.0f))		// リザルトスコアのサイズ
#define RESULTSCORE_INIT_POS		(D3DXVECTOR3(-1000.0f, 500.0f, 0.0f))	// リザルトスコアの初期位置
#define RESULTSCORE_DEST_POS		(D3DXVECTOR3(100.0f, 500.0f, 0.0f))		// リザルトスコアの目的の位置
#define RESULTSCORE_RAND			(10)									// リザルトスコアのランダムで検出する幅
#define RESULTSCORE_MOVE			(70.0f)									// リザルトスコアの移動量
#define RESULTSCORE_RANK_POS		(D3DXVECTOR3(320.0f, -150.0f, 100.0f))	// リザルトスコアのランクの位置

//========================
// コンストラクタ
//========================
CResultScore::CResultScore() : CObject(TYPE_SCORE, PRIORITY_UI)
{
	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < MAX_RESULTSCORE_DIGIT; nCnt++)
	{
		// 数字の情報
		m_apNumber[nCnt] = nullptr;
	}

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_nScore = 0;		// スコア
}

//========================
// デストラクタ
//========================
CResultScore::~CResultScore()
{

}

//========================
// 初期化処理
//========================
HRESULT CResultScore::Init(void)
{
	//ローカル変数宣言
	int nTexIdx = NONE_TEXIDX;		// テクスチャのインデックス

	// テクスチャの読み込み処理
	nTexIdx = CManager::GetTexture()->Regist(RESULTSCORE_TEXTURE);

	// メモリを確保する
	for (int nCnt = 0; nCnt < MAX_RESULTSCORE_DIGIT; nCnt++)
	{
		if (m_apNumber[nCnt] == nullptr)
		{ // ポインタが NULL の場合

			// メモリを確保する
			m_apNumber[nCnt] = new CNumber(CObject::TYPE_NONE, CObject::PRIORITY_UI);
		}
		else
		{ // ポインタが NULL じゃない場合

			// 警告文
			MessageBox(NULL, "スコアのメモリが既に使われている！", "警告！", MB_ICONWARNING);

			// 失敗を返す
			return E_FAIL;
		}

		if (m_apNumber[nCnt] != nullptr)
		{ // ポインタが NULL じゃない場合

			// 初期化処理
			if (FAILED(m_apNumber[nCnt]->Init()))
			{ // 初期化処理に失敗した場合

				// 警告文
				MessageBox(NULL, "スコアの初期化に失敗！", "警告！", MB_ICONWARNING);

				// 失敗を返す
				return E_FAIL;
			}

			// テクスチャの割り当て処理
			m_apNumber[nCnt]->BindTexture(nTexIdx);
		}
		else
		{ // ポインタが　NULL の場合

			// 警告文
			MessageBox(NULL, "スコアのメモリの確保に失敗！", "警告！", MB_ICONWARNING);

			// 失敗を返す
			return E_FAIL;
		}
	}

	m_pos = RESULTSCORE_INIT_POS;		// 位置
	m_nScore = CManager::GetScore();	// スコア

	// 成功を返す
	return S_OK;
}

//========================
// 終了処理
//========================
void CResultScore::Uninit(void)
{
	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < MAX_RESULTSCORE_DIGIT; nCnt++)
	{
		// 終了処理
		m_apNumber[nCnt]->Uninit();
	}

	// 数字の終了処理
	Release();
}

//========================
// 更新処理
//========================
void CResultScore::Update(void)
{
	if (CResult::GetState() == CResult::STATE_SCOREDISP)
	{ // スコア表示状態の場合

		// 移動処理
		MoveProcess();

		// 計算処理
		Calculate();
	}
}

//========================
// 描画処理
//========================
void CResultScore::Draw(void)
{
	// アルファテストを有効にする
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			// アルファテストの有効/無効設定
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);		// アルファテストの設定
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHAREF, ALPHA_TEST_NUMBER);	// アルファテストの参照値設定

	for (int nCnt = 0; nCnt < MAX_RESULTSCORE_DIGIT; nCnt++)
	{
		// 描画処理
		m_apNumber[nCnt]->Draw();
	}

	// アルファテストを無効にする
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			// アルファテストの有効/無効設定
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);		// アルファテストの設定
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHAREF, 0);					// アルファテストの参照値設定
}

//========================
// 計算処理
//========================
void CResultScore::Calculate(void)
{
	// ローカル変数宣言
	int aNum[MAX_RESULTSCORE_DIGIT];		// 数値

	// 10進数の変換処理
	DecimalCalculation(MAX_RESULTSCORE_DIGIT, m_nScore, &aNum[0]);

	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < MAX_RESULTSCORE_DIGIT; nCnt++)
	{
		// 数字の設定処理
		m_apNumber[nCnt]->SetNumber(aNum[nCnt]);

		// テクスチャ座標の設定処理
		m_apNumber[nCnt]->SetVtxTextureAnim(NUMBER_TEXTURE_PATTERN, m_apNumber[nCnt]->GetNumber());
	}
}

//========================
// 移動処理
//========================
void CResultScore::MoveProcess(void)
{
	// 位置を更新する
	m_pos.x += RESULTSCORE_MOVE;

	if (m_pos.x >= RESULTSCORE_DEST_POS.x)
	{ // 位置が一定数以上になった場合

		// 位置を補正する
		m_pos.x = RESULTSCORE_DEST_POS.x;

		// 停止状態にする
		CResult::SetState(CResult::STATE_STOP);

		// ランクを設定する
		CResultRank::Create(RESULTSCORE_RANK_POS, DecisionRank());

		// ランクの出現音を鳴らす
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_RANKAPPEAR);
	}

	for (int nCnt = 0; nCnt < MAX_RESULTSCORE_DIGIT; nCnt++)
	{
		if (m_apNumber[nCnt] != nullptr)
		{ // ポインタが NULL じゃない場合

			// 設定処理
			m_apNumber[nCnt]->SetPos(D3DXVECTOR3(m_pos.x + (nCnt * RESULTSCORE_WIDTH_SHIFT), m_pos.y, 0.0f));				// 位置設定

			// 頂点情報の設定処理
			m_apNumber[nCnt]->SetVertex();
		}
	}
}

//========================
// ランクの決定処理
//========================
CResult::RANK CResultScore::DecisionRank(void)
{
	// ローカル変数宣言
	CFile::SResultFile result = CManager::GetFile()->GetResultInfo();		// リザルトの情報を取得する
	CResult::RANK rank = CResult::RANK_C;		// ランク

	if (CManager::GetScore() >= result.aRankScore[CResult::RANK_SS])
	{ // SSランクの基準よりも速かった場合

		// SSランクを設定する
		rank = CResult::RANK_SS;
	}
	else if (CManager::GetScore() >= result.aRankScore[CResult::RANK_S])
	{ // Sランクの基準よりも速かった場合

		// Sランクを設定する
		rank = CResult::RANK_S;
	}
	else if (CManager::GetScore() >= result.aRankScore[CResult::RANK_A])
	{ // Aランクの基準よりも速かった場合

		// Aランクを設定する
		rank = CResult::RANK_A;
	}
	else if (CManager::GetScore() >= result.aRankScore[CResult::RANK_B])
	{ // Bランクの基準よりも速かった場合

		// Bランクを設定する
		rank = CResult::RANK_B;
	}
	else if (CManager::GetScore() >= result.aRankScore[CResult::RANK_C])
	{ // Cランクの基準よりも速かった場合

		// Cランクを設定する
		rank = CResult::RANK_C;
	}

	// ランクを返す
	return rank;
}

//========================
// 生成処理
//========================
CResultScore* CResultScore::Create(void)
{
	// ローカルオブジェクトを生成
	CResultScore* pScore = nullptr;	// プレイヤーのインスタンスを生成

	if (pScore == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pScore = new CResultScore;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pScore != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pScore->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "スコアの初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		for (int nCnt = 0; nCnt < MAX_RESULTSCORE_DIGIT; nCnt++)
		{
			// 設定処理
			pScore->m_apNumber[nCnt]->SetPos(D3DXVECTOR3(pScore->m_pos.x + (nCnt * RESULTSCORE_WIDTH_SHIFT), pScore->m_pos.y, 0.0f));				// 位置設定
			pScore->m_apNumber[nCnt]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// 向き設定
			pScore->m_apNumber[nCnt]->SetSize(RESULTSCORE_SIZE);					// サイズ設定
			pScore->m_apNumber[nCnt]->SetLength();									// 長さ設定
			pScore->m_apNumber[nCnt]->SetAngle();									// 方向設定

			pScore->m_apNumber[nCnt]->SetType(CNumber::TYPE_DECIMAL);	// 番号の種類
			pScore->m_apNumber[nCnt]->SetNumber(0);						// 番号を設定する

			// 頂点情報の設定処理
			pScore->m_apNumber[nCnt]->SetVertex();
			
			// テクスチャの設定処理(アニメーションバージョン)
			pScore->m_apNumber[nCnt]->SetVtxTextureAnim(NUMBER_TEXTURE_PATTERN, 0);
		}
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pScore;
}