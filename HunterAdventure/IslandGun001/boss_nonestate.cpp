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

#include "boss_flystate.h"
#include "boss_windstate.h"

//----------------------------------------------------------------------------------------------------------------
// 無名名前空間
//----------------------------------------------------------------------------------------------------------------
namespace
{
	const int STATECHANGE_COUNT = 90;			// 状態が変化するカウント数
	const float GRAVITY = 0.1f;					// 重力
	const float WIND_STATE_LENGTH = 5000.0f;	// 風攻撃状態になる距離
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

	// 待機モーションを設定する
	pBoss->GetMotion()->Set(CBoss::MOTIONTYPE_NEUTRAL);
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

		if (fLength <= WIND_STATE_LENGTH)
		{ // 長さが近い場合

			// かまいたち状態にする
			pBoss->ChangeState(new CBossWindState);
		}
		else
		{ // 上記以外

			// 飛行状態にする
			pBoss->ChangeState(new CBossFlyState);
		}
	}
}