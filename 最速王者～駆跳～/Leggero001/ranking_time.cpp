//============================================
//
// ランキングタイマー処理[ranking_time.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "ranking_time.h"
#include "manager.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define RANKINGTIME_WIDTH_SHIFT		(100.0f)								// ランキングタイマーの横のずらす幅
#define RANKINGTIME_SIZE			(D3DXVECTOR3(45.0f, 60.0f, 0.0f))		// ランキングタイマーのサイズ
#define RANKINGTIME_POS				(D3DXVECTOR3(700.0f, 80.0f, 0.0f))		// ランキングタイマーの位置
#define RANKINGTIME_DOT_SIZE		(D3DXVECTOR3(10.0f, 10.0f, 0.0f))		// ランキングタイマーの点のサイズ

//========================
// コンストラクタ
//========================
CRankingTime::CRankingTime() : CTime(CObject::TYPE_TIME, CObject::PRIORITY_UI)
{
	// 全ての値をクリアする
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
}

//========================
// デストラクタ
//========================
CRankingTime::~CRankingTime()
{

}

//========================
// 初期化処理
//========================
HRESULT CRankingTime::Init(void)
{
	if (FAILED(CTime::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	SetTotalTime(CManager::GetClearTime());		// クリアタイムを代入する
	m_pos = RANKINGTIME_POS;					// 位置を設定する

	// 計算処理
	Calculate();

	// 成功を返す
	return S_OK;
}

//========================
// 終了処理
//========================
void CRankingTime::Uninit(void)
{
	// 終了処理
	CTime::Uninit();

	// クリアタイムを初期化する
	CManager::SetClearTime(0);
}

//========================
// 更新処理
//========================
void CRankingTime::Update(void)
{
	// 計算処理
	Calculate();
}

//========================
// 描画処理
//========================
void CRankingTime::Draw(void)
{
	// 描画処理
	CTime::Draw();
}

//========================
// 計算処理
//========================
void CRankingTime::Calculate(void)
{
	// 数値の設定処理
	SetNumber();

	// 1桁ごとのテクスチャの設定処理
	SetTexture();
}

//========================
// 生成処理
//========================
CRankingTime* CRankingTime::Create(void)
{
	// ローカルオブジェクトを生成
	CRankingTime* pTime = nullptr;	// プレイヤーのインスタンスを生成

	if (pTime == nullptr)
	{ // オブジェクトが NULL の場合

	  // オブジェクトを生成
		pTime = new CRankingTime;
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
		pTime->SetData(RANKINGTIME_POS, D3DXVECTOR3(0.0f, 0.0f, 0.0f), RANKINGTIME_SIZE, RANKINGTIME_DOT_SIZE, RANKINGTIME_WIDTH_SHIFT);
	}
	else
	{ // オブジェクトが NULL の場合

	  // NULL を返す
		return nullptr;
	}

	// 時間のポインタを返す
	return pTime;
}