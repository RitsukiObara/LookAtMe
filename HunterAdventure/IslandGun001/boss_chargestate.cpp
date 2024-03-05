//================================================================================================================
//
// ボスのチャージ状態処理 [boss_chargestate.cpp]
// Author：小原立暉
//
//================================================================================================================
//****************************************************************************************************************
//	インクルードファイル
//****************************************************************************************************************
#include "manager.h"
#include "boss.h"
#include "boss_chargestate.h"
#include "motion.h"
#include "sound.h"
#include "useful.h"

#include "game.h"
#include "player.h"

//----------------------------------------------------------------------------------------------------------------
// 定数定義
//----------------------------------------------------------------------------------------------------------------
namespace
{
	const int CHARGE_SOUND_COUNT = 110;		// サウンドのカウント
}

//==========================
// オーバーロードコンストラクタ
//==========================
CBossChargeState::CBossChargeState(CBossState* next, const int nCount)
{
	// 全ての値をクリアする
	m_pNext = next;				// 次の状態
	m_nCount = nCount;			// 経過カウント
}

//==========================
// デストラクタ
//==========================
CBossChargeState::~CBossChargeState()
{
	if (m_pNext != nullptr)
	{ // 次の状態が無かった場合

		// 終了処理
		m_pNext = nullptr;
	}
}

//==========================
// 完全消去処理
//==========================
void CBossChargeState::Delete(void)
{
	if (m_pNext != nullptr)
	{ // 次の状態が無かった場合

		// 終了処理
		m_pNext->Uninit();
		m_pNext = nullptr;
	}

	// 本体の消去処理
	CBossState::Delete();
}

//==========================
// 状態処理
//==========================
void CBossChargeState::Process(CBoss* pBoss)
{
	// 経過カウントを加算する
	m_nCount--;

	if (m_nCount % CHARGE_SOUND_COUNT == 0)
	{ // 一定カウント経過した場合

		// チャージ音を鳴らす
		CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_BOSSCHARGE);
	}

	if (m_nCount <= 0)
	{ // 一定カウント経過した場合

		// 経過カウントを初期化する
		m_nCount = 0;

		// 次の状態に変える
		pBoss->ChangeState(m_pNext);
	}
}

//==========================
// 情報の設定処理
//==========================
void CBossChargeState::SetData(CBoss* pBoss)
{
	if (pBoss->GetMotion()->GetType() != CBoss::MOTIONTYPE_CHARGE)
	{ // チャージモーション以外の場合

		// チャージモーションにする
		pBoss->GetMotion()->Set(CBoss::MOTIONTYPE_CHARGE);
	}

	// チャージ音を鳴らす
	CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_BOSSCHARGE);
}