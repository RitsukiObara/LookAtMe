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
#include "boss.h"
#include "boss_windstate.h"
#include "motion.h"

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
	// 追跡処理
	Chase(pBoss);

	// カウントを加算する
	m_nCount++;

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
	// 待機モーションにする
	pBoss->GetMotion()->Set(CBoss::MOTIONTYPE_NEUTRAL);

	// 風攻撃を生成
	m_pWindShot = CWindShot::Create(pBoss->GetPos());
}

//==========================
// 追跡処理
//==========================
void CBossWindState::Chase(CBoss* pBoss)
{
	// プレイヤーの情報を取得する
	CPlayer* pPlayer = CGame::GetPlayer();

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		// 位置と向きを宣言
		D3DXVECTOR3 posPlayer = pPlayer->GetPos();
		D3DXVECTOR3 posBoss = pBoss->GetPos();
		D3DXVECTOR3 rotBoss = pBoss->GetRot();
		float fRotDest = atan2f(posPlayer.x - posBoss.x, posPlayer.z - posBoss.z);

		// 向きの補正処理
		useful::RotCorrect(fRotDest, &rotBoss.y, CHASE_CORRECT);

		// 向きを適用する
		pBoss->SetRot(rotBoss);
	}
}