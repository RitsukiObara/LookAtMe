//============================================
//
// リザルト表示タイマー処理[resulttime.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "result_time.h"
#include "result_score.h"
#include "result_rank.h"
#include "file.h"
#include "manager.h"
#include "sound.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define RESULTTIME_WIDTH_SHIFT		(120.0f)								// リザルトタイマーの横のずらす幅
#define RESULTTIME_SIZE				(D3DXVECTOR3(60.0f, 80.0f, 0.0f))		// リザルトタイマーのサイズ
#define RESULTTIME_INIT_POS			(D3DXVECTOR3(-1000.0f, 200.0f, 0.0f))	// リザルトタイマーの位置
#define RESULTTIME_DEST_POS			(D3DXVECTOR3(100.0f, 200.0f, 0.0f))		// リザルトタイマーの位置
#define RESULTTIME_RAND				(10)									// リザルトタイマーのランダムで検出する幅
#define RESULTTIME_MOVE				(70.0f)									// リザルトタイマーの移動量
#define RESULTTIME_RANK_POS			(D3DXVECTOR3(320.0f, 150.0f, 100.0f))	// リザルトタイマーのランクを出す位置
#define RESULTTIME_DOT_SIZE			(D3DXVECTOR3(12.0f, 12.0f, 0.0f))		// リザルトタイマーの点のサイズ

//========================
// コンストラクタ
//========================
CResultTime::CResultTime() : CTime(CObject::TYPE_TIME, CObject::PRIORITY_UI)
{
	// 全ての値をクリアする
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
}

//========================
// デストラクタ
//========================
CResultTime::~CResultTime()
{

}

//========================
// 初期化処理
//========================
HRESULT CResultTime::Init(void)
{
	if (FAILED(CTime::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	SetTotalTime(CManager::GetClearTime());			// クリアタイムを代入する
	m_pos = RESULTTIME_INIT_POS;					// 位置を設定する

	// 計算処理
	Calculate();

	// 成功を返す
	return S_OK;
}

//========================
// 終了処理
//========================
void CResultTime::Uninit(void)
{
	// 終了処理
	CTime::Uninit();
}

//========================
// 更新処理
//========================
void CResultTime::Update(void)
{
	if (CResult::GetState() == CResult::STATE_TIMEDISP)
	{ // タイマー表示状態の場合

		// 移動処理
		MoveProcess();
	}
	else
	{ // 上記以外の場合

		// 計算処理
		Calculate();
	}
}

//========================
// 描画処理
//========================
void CResultTime::Draw(void)
{
	// 描画処理
	CTime::Draw();
}

//========================
// 計算処理
//========================
void CResultTime::Calculate(void)
{
	// 数値の設定処理
	SetNumber();

	// 1桁ごとのテクスチャの設定処理
	SetTexture();
}

//========================
// 移動処理
//========================
void CResultTime::MoveProcess(void)
{
	// 位置を更新する
	m_pos.x += RESULTTIME_MOVE;

	if (m_pos.x >= RESULTTIME_DEST_POS.x)
	{ // 位置が一定数以上になった場合

		// 位置を補正する
		m_pos.x = RESULTTIME_DEST_POS.x;

		// リザルトの状態を設定する
		CResult::SetState(CResult::STATE_SCOREDISP);

		// ランクを設定する
		CResultRank::Create(RESULTTIME_RANK_POS, DecisionRank());

		// スコアを生成する
		CResultScore::Create();

		// ランクの出現音を鳴らす
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_RANKAPPEAR);
	}

	for (int nCnt = 0; nCnt < MAX_TIME_DIGIT; nCnt++)
	{
		if (GetNumber(nCnt) != nullptr)
		{ // ポインタが NULL じゃない場合

			// 設定処理
			GetNumber(nCnt)->SetPos(D3DXVECTOR3(m_pos.x + (nCnt * RESULTTIME_WIDTH_SHIFT), m_pos.y, 0.0f));				// 位置設定

			// 頂点情報の設定処理
			GetNumber(nCnt)->SetVertex();
		}
	}

	// 点の移動処理
	DotMove();
}

//========================
// 点の移動処理
//========================
void CResultTime::DotMove(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetDot()->GetPos();		//位置を取得する

	// 位置を動かす
	pos.x += RESULTTIME_MOVE;

	if (pos.x >= RESULTTIME_DEST_POS.x + (3 * RESULTTIME_WIDTH_SHIFT) - RESULTTIME_SIZE.x - (RESULTTIME_DOT_SIZE.x * 0.5f))
	{ // 位置が一定以上になった場合

		// 位置を補正する
		pos.x = RESULTTIME_DEST_POS.x + (3 * RESULTTIME_WIDTH_SHIFT) - RESULTTIME_SIZE.x - (RESULTTIME_DOT_SIZE.x * 0.5f);
	}

	// 位置を更新する
	GetDot()->SetPos(pos);

	// 頂点座標の設定処理
	GetDot()->SetVertex();
}

//========================
// ランクの決定処理
//========================
CResult::RANK CResultTime::DecisionRank(void)
{
	// ローカル変数宣言
	CFile::SResultFile result = CManager::GetFile()->GetResultInfo();		// リザルトの情報を取得する
	CResult::RANK rank = CResult::RANK_C;		// ランク

	if (CManager::GetClearTime() <= (DWORD)result.aRankTime[CResult::RANK_SS])
	{ // SSランクの基準よりも速かった場合

		// SSランクを設定する
		rank = CResult::RANK_SS;
	}
	else if (CManager::GetClearTime() <= (DWORD)result.aRankTime[CResult::RANK_S])
	{ // Sランクの基準よりも速かった場合

		// Sランクを設定する
		rank = CResult::RANK_S;
	}
	else if (CManager::GetClearTime() <= (DWORD)result.aRankTime[CResult::RANK_A])
	{ // Aランクの基準よりも速かった場合

		// Aランクを設定する
		rank = CResult::RANK_A;
	}
	else if (CManager::GetClearTime() <= (DWORD)result.aRankTime[CResult::RANK_B])
	{ // Bランクの基準よりも速かった場合

		// Bランクを設定する
		rank = CResult::RANK_B;
	}
	else if (CManager::GetClearTime() <= (DWORD)result.aRankTime[CResult::RANK_C])
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
CResultTime* CResultTime::Create(void)
{
	// ローカルオブジェクトを生成
	CResultTime* pTime = nullptr;	// プレイヤーのインスタンスを生成

	if (pTime == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pTime = new CResultTime;
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
		pTime->SetData(RESULTTIME_INIT_POS, D3DXVECTOR3(0.0f, 0.0f, 0.0f), RESULTTIME_SIZE, RESULTTIME_DOT_SIZE, RESULTTIME_WIDTH_SHIFT);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// 時間のポインタを返す
	return pTime;
}