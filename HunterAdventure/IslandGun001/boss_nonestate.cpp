//================================================================================================================
//
// ボスの通常状態処理 [boss_nonestate.cpp]
// Author：小原立暉
//
//================================================================================================================
//****************************************************************************************************************
//	インクルードファイル
//****************************************************************************************************************
#include "useful.h"
#include "boss.h"
#include "boss_nonestate.h"
#include "motion.h"

#include "game.h"
#include "player.h"

#include "boss_chargestate.h"
#include "boss_flystate.h"
#include "boss_windstate.h"
#include "boss_firestate.h"

//----------------------------------------------------------------------------------------------------------------
// 無名名前空間
//----------------------------------------------------------------------------------------------------------------
namespace
{
	const int STATECHANGE_COUNT = 90;				// 状態が変化するカウント数
	const float GRAVITY = 0.1f;						// 重力

	const int WIND_STATE_CHARGE = 100;				// 風状態のチャージカウント
	const float FLYSTATE_DISTANCE = 4800.0f;		// 飛行状態にする距離
	const int NUM_RAND_STATE = 2;					// ランダムでなる状態の総数
}

//==========================
// コンストラクタ
//==========================
CBossNoneState::CBossNoneState()
{
	// 全ての値をクリアする
	m_fGravity = 0.0f;		// 重力
	m_nCount = 0;			// 経過カウント
}

//==========================
// デストラクタ
//==========================
CBossNoneState::~CBossNoneState()
{

}

//==========================
// 状態処理
//==========================
void CBossNoneState::Process(CBoss* pBoss)
{
	{ // 重力処理

		// 位置を取得する
		D3DXVECTOR3 pos = pBoss->GetPos();

		// 重力処理
		useful::Gravity(&m_fGravity, &pos.y, GRAVITY);

		// 位置を適用する
		pBoss->SetPos(pos);
	}

	// 経過カウントを加算する
	m_nCount++;

	// 起伏地面の当たり判定
	pBoss->ElevationCollision();

	if (m_nCount >= STATECHANGE_COUNT)
	{ // 一定時間経過した場合

		// 状態の選択処理
		StateSelect(pBoss);

		// この先の処理を行わない
		return;
	}
}

//==========================
// 情報の設定処理
//==========================
void CBossNoneState::SetData(CBoss* pBoss)
{
	// 全ての値を設定する
	m_fGravity = 0.0f;		// 重力
	m_nCount = 0;			// 経過カウント

	if (pBoss->GetMotion()->GetType() != CBoss::MOTIONTYPE_NEUTRAL)
	{ // 通常モーション以外の場合

		// 待機モーションを設定する
		pBoss->GetMotion()->Set(CBoss::MOTIONTYPE_NEUTRAL);
	}
}

//==========================
// 状態の選択処理
//==========================
void CBossNoneState::StateSelect(CBoss* pBoss)
{
	// プレイヤーを取得する
	CPlayer* pPlayer = CGame::GetPlayer();

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		// 位置を取得する
		D3DXVECTOR3 posPlayer = pPlayer->GetPos();
		D3DXVECTOR3 posBoss = pBoss->GetPos();
		float fLength = sqrtf((posPlayer.x - posBoss.x) * (posPlayer.x - posBoss.x) + (posPlayer.z - posBoss.z) * (posPlayer.z - posBoss.z));

		if (fLength >= FLYSTATE_DISTANCE)
		{ // プレイヤーが遠かった場合

			// 飛行状態にする
			pBoss->ChangeState(new CBossFlyState);
		}
		else
		{ // 上記以外

			// 次の状態をランダムで算出する
			int nState = rand() % NUM_RAND_STATE;

			switch (nState)
			{
			case 0:

				// チャージ状態(風攻撃状態)にする
				pBoss->ChangeState(new CBossChargeState(new CBossWindState, WIND_STATE_CHARGE));

				break;

			case 1:

				// 炎攻撃状態にする
				pBoss->ChangeState(new CBossFireState);

				break;

			default:

				// 停止
				assert(false);

				break;
			}
		}
	}
}