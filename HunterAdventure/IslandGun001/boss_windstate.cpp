//================================================================================================================
//
// ボスのかまいたち状態処理 [boss_windstate.cpp]
// Author：小原立暉
//
//================================================================================================================
//****************************************************************************************************************
//	インクルードファイル
//****************************************************************************************************************
#include "useful.h"
#include "manager.h"
#include "boss.h"
#include "boss_windstate.h"
#include "motion.h"
#include "sound.h"

#include "game.h"
#include "player.h"
#include "wind_shot.h"

#include "boss_nonestate.h"

//----------------------------------------------------------------------------------------------------------------
// 無名名前空間
//----------------------------------------------------------------------------------------------------------------
namespace
{
	const float CHASE_CORRECT = 0.01f;			// 追跡の補正数
	const int WIND_CREATE_COUNT = 70;			// 風攻撃発生カウント
	const int FINISH_COUNT = 400;				// 終了カウント
}

//==========================
// コンストラクタ
//==========================
CBossWindState::CBossWindState()
{
	// 全ての値をクリアする
	m_pWindShot = nullptr;			// 風攻撃の情報
	m_nCount = 0;					// 経過カウント
}

//==========================
// デストラクタ
//==========================
CBossWindState::~CBossWindState()
{
	if (m_pWindShot != nullptr)
	{ // 風攻撃が NULL じゃない場合

		// 風攻撃を NULL にする
		m_pWindShot->SetState(CWindShot::STATE_DELETE);
		m_pWindShot = nullptr;
	}
}

//==========================
// 状態処理
//==========================
void CBossWindState::Process(CBoss* pBoss)
{
	// カウントを加算する
	m_nCount++;

	if (m_nCount == WIND_CREATE_COUNT)
	{ // 風攻撃発生カウントになった場合

		// 風攻撃を生成
		m_pWindShot = CWindShot::Create(pBoss->GetPos());

		// 風攻撃音を鳴らす
		CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_WINDSHOT);
		CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_BOSSHOWLING);
	}

	if (m_nCount >= FINISH_COUNT)
	{ // 一定カウント経過した場合

		// 通常状態にする
		pBoss->ChangeState(new CBossNoneState);

		// この先の処理を行わない
		return;
	}
}

//==========================
// 情報の設定処理
//==========================
void CBossWindState::SetData(CBoss* pBoss)
{
	if (pBoss->GetMotion()->GetType() != CBoss::MOTIONTYPE_HOWLING)
	{ // 遠吠えモーション以外の場合

		// 遠吠えモーションにする
		pBoss->GetMotion()->Set(CBoss::MOTIONTYPE_HOWLING);
	}
}