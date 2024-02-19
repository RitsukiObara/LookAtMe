//================================================================================================================
//
// ボスの炎攻撃状態処理 [boss_firestate.cpp]
// Author：小原立暉
//
//================================================================================================================
//****************************************************************************************************************
//	インクルードファイル
//****************************************************************************************************************
#include "useful.h"
#include "boss.h"
#include "boss_firestate.h"
#include "motion.h"

#include "game.h"
#include "player.h"
#include "fire_warning.h"

#include "boss_nonestate.h"

//----------------------------------------------------------------------------------------------------------------
// 無名名前空間
//----------------------------------------------------------------------------------------------------------------
namespace
{
	const float CHASE_CORRECT = 0.01f;		// 追跡の補正数
	const int FIRE_CREATE_COUNT = 30;		// 炎を出すカウント数
	const int FINISH_COUNT = 180;			// 終了カウント
}

//==========================
// コンストラクタ
//==========================
CBossFireState::CBossFireState()
{
	// 全ての値をクリアする
	m_nCount = 0;					// 経過カウント
}

//==========================
// デストラクタ
//==========================
CBossFireState::~CBossFireState()
{

}

//==========================
// 状態処理
//==========================
void CBossFireState::Process(CBoss* pBoss)
{
	// 追跡処理
	Chase(pBoss);

	// カウントを加算する
	m_nCount++;

	CPlayer* pPlayer = CGame::GetPlayer();

	if (m_nCount % FIRE_CREATE_COUNT == 0 &&
		pPlayer != nullptr)
	{ // 経過カウントが一定数経過した場合

		// 炎注意の生成
		CFireWarning::Create(pPlayer->GetPos());
	}

	if (m_nCount >= FINISH_COUNT)
	{ // 終了カウント経過した場合

		// 状態切り替え処理
		pBoss->ChangeState(new CBossNoneState);
	}
}

//==========================
// 情報の設定処理
//==========================
void CBossFireState::SetData(CBoss* pBoss)
{
	// 待機モーションにする
	pBoss->GetMotion()->Set(CBoss::MOTIONTYPE_NEUTRAL);
}

//==========================
// 追跡処理
//==========================
void CBossFireState::Chase(CBoss* pBoss)
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