//================================================================================================================
//
// ボスの飛行状態処理 [boss_flystate.cpp]
// Author：小原立暉
//
//================================================================================================================
//****************************************************************************************************************
//	インクルードファイル
//****************************************************************************************************************
#include "useful.h"
#include "boss.h"
#include "boss_flystate.h"
#include "motion.h"
#include "collision.h"

#include "ripple.h"
#include "game.h"
#include "player.h"

#include "boss_nonestate.h"

//----------------------------------------------------------------------------------------------------------------
// 無名名前空間
//----------------------------------------------------------------------------------------------------------------
namespace
{
	const int FLYMOTION_COUNT = 50;		// 飛行モーションになるカウント数
	const int MOVE_COUNT = 115;			// 動くまでのカウント数
	const int NONESTATE_COUNT = 150;	// 通常状態に戻るカウント数
	const float ROT_CORRECT = 0.08f;	// 向きの補正係数
	const float STANDBY_MOVE_Y = 0.2f;	// スタンバイ時のY軸の移動量
	const float MOVE_CORRECT = 0.1f;	// 移動の補正係数
	const float MOVE_SPEED = 300.0f;	// 移動速度
	const int RIPPLE_COUNT = 4;			// 波紋のカウント
	const float RIPPLE_HEIGHT = 600.0f;	// 波紋の高度
	const D3DXVECTOR3 RIPPLE_SCALE = D3DXVECTOR3(100.0f, 100.0f, 100.0f);	// 波紋の拡大率
	const float STAGE_COLLISION_WIDTH = 300.0f;		// ステージの当たり判定の幅
}

//==========================
// コンストラクタ
//==========================
CBossFlyState::CBossFlyState()
{
	// 全ての値をクリアする
	m_moveDest = NONE_D3DXVECTOR3;	// 目的の移動量
	m_move = NONE_D3DXVECTOR3;		// 移動量
	m_nCount = 0;					// 経過カウント
}

//==========================
// デストラクタ
//==========================
CBossFlyState::~CBossFlyState()
{

}

//==========================
// 状態処理
//==========================
void CBossFlyState::Process(CBoss* pBoss)
{
	// 経過カウントを加算する
	m_nCount++;
	
	if (m_nCount >= NONESTATE_COUNT)
	{ // 通常状態に戻るようになった場合

		// ヒット状況を false にする
		pBoss->SetEnableHit(false);

		// 状態の切り替え処理
		pBoss->ChangeState(new CBossNoneState);

		// この先の処理を行わない
		return;
	}
	else if (m_nCount >= MOVE_COUNT)
	{ // 一定時間経過した場合

		// 飛行処理
		Fly(pBoss);

		// 波紋の生成処理
		Ripple(pBoss);

		// ヒット状況を true にする
		pBoss->SetEnableHit(true);
	}
	else if (m_nCount >= FLYMOTION_COUNT)
	{ // 一定時間経過した場合

		// スタンバイ処理
		Standby(pBoss);
	}
	else
	{ // 上記以外

		// 向きの移動処理
		RotMove(pBoss);
	}

	// ステージの当たり判定
	StageCollision(pBoss);
}

//==========================
// 情報の設定処理
//==========================
void CBossFlyState::SetData(CBoss* pBoss)
{
	// 全ての値を設定する
	m_moveDest = NONE_D3DXVECTOR3;	// 目的の移動量
	m_move = NONE_D3DXVECTOR3;		// 移動量
	m_nCount = 0;					// 経過カウント
}

//==========================
// 向きの移動処理
//==========================
void CBossFlyState::RotMove(CBoss* pBoss)
{
	CPlayer* pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 pos = pBoss->GetPos();
	D3DXVECTOR3 rot = pBoss->GetRot();

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		D3DXVECTOR3 posPlayer = pPlayer->GetPos();		// プレイヤーの位置を取得する
		float fRotDestY = 0.0f;							// 目的の向き

		// 目的の向きを算出する
		fRotDestY = atan2f(posPlayer.x - pos.x, posPlayer.z - pos.z);

		// 向きの補正処理
		useful::RotCorrect(fRotDestY, &rot.y, ROT_CORRECT);
	}

	// 向きを適用する
	pBoss->SetRot(rot);
}

//==========================
// スタンバイ処理 
//==========================
void CBossFlyState::Standby(CBoss* pBoss)
{
	// 位置を取得する
	D3DXVECTOR3 pos = pBoss->GetPos();

	if (pBoss->GetMotion()->GetType() != CBoss::MOTIONTYPE_FLYING)
	{ // 飛行モーションじゃない場合

		// 飛行モーションを設定する
		pBoss->GetMotion()->Set(CBoss::MOTIONTYPE_FLYING);
	}

	// 位置を加算する
	pos.y += STANDBY_MOVE_Y;

	// 位置を適用する
	pBoss->SetPos(pos);
}

//==========================
// 飛行処理
//==========================
void CBossFlyState::Fly(CBoss* pBoss)
{
	// 位置と向きを取得する
	D3DXVECTOR3 pos = pBoss->GetPos();
	D3DXVECTOR3 rot = pBoss->GetRot();

	// 目的の移動量を設定する
	m_moveDest.x = sinf(rot.y) * MOVE_SPEED;
	m_moveDest.z = cosf(rot.y) * MOVE_SPEED;

	// 移動量の補正処理
	useful::Correct(m_moveDest.x, &m_move.x, MOVE_CORRECT);
	useful::Correct(m_moveDest.z, &m_move.z, MOVE_CORRECT);

	// 位置を加算する
	pos.x += m_move.x;
	pos.z += m_move.z;

	// 位置を適用する
	pBoss->SetPos(pos);
}

//==========================
// 波紋の生成処理
//==========================
void CBossFlyState::Ripple(CBoss* pBoss)
{
	if (m_nCount % RIPPLE_COUNT == 0)
	{ // 一定周期で

		// 波紋の出る位置を定める
		D3DXVECTOR3 pos = pBoss->GetPos();
		pos.y += RIPPLE_HEIGHT;

		// 波紋の出る向きを定める
		D3DXVECTOR3 rot = pBoss->GetRot();
		rot.x = D3DX_PI * 0.5f;
		rot.z = D3DX_PI;

		// 波紋を生成する
		CRipple::Create
		(
			pos,
			rot,
			RIPPLE_SCALE
		);
	}
}

//==========================
// ステージの当たり判定
//==========================
void CBossFlyState::StageCollision(CBoss* pBoss)
{
	// 位置を取得
	D3DXVECTOR3 pos = pBoss->GetPos();

	// ステージの当たり判定
	collision::StageCollision(&pos, STAGE_COLLISION_WIDTH);

	// 位置を適用
	pBoss->SetPos(pos);
}