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
#include "fraction.h"
#include "fire_warning.h"

#include "boss_nonestate.h"

//----------------------------------------------------------------------------------------------------------------
// 無名名前空間
//----------------------------------------------------------------------------------------------------------------
namespace
{
	const float CHASE_CORRECT = 0.01f;		// 追跡の補正数
	const int FIRE_CREATE_COUNT = 30;		// 炎を出すカウント数
	const int FIRST_FRACTION_COUNT = 20;	// 1回目の破片を出すカウント数
	const int SECOND_FRACTION_COUNT = 112;	// 2回目の破片を出すカウント数
	const int THIRD_FRACTION_COUNT = 187;	// 3回目の破片を出すカウント数
	const int FIRE_CREATE_RANGE = 180;		// 炎を出している間のカウント数
	const int FINISH_COUNT = 240;			// 終了カウント
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
	// カウントを加算する
	m_nCount++;

	CPlayer* pPlayer = CGame::GetPlayer();

	if (m_nCount % FIRE_CREATE_COUNT == 0 &&
		m_nCount <= FIRE_CREATE_RANGE &&
		pPlayer != nullptr)
	{ // 経過カウントが一定数経過した場合

		// 炎注意の生成
		CFireWarning::Create(pPlayer->GetPos());
	}
	
	if (m_nCount == FIRST_FRACTION_COUNT ||
		m_nCount == SECOND_FRACTION_COUNT ||
		m_nCount == THIRD_FRACTION_COUNT)
	{ // 破片を出すカウント数だった場合

		D3DXMATRIX mtx;
		D3DXVECTOR3 pos = NONE_D3DXVECTOR3;

		pBoss->GetHierarchy(7)->MatrixCalc(&mtx, pBoss->GetMatrix());

		pos = D3DXVECTOR3(mtx._41, mtx._42, mtx._43);

		for (int nCnt = 0; nCnt < 15; nCnt++)
		{
			CFraction::Create(pos, CFraction::TYPE::TYPE_DIRT, 30, 4, 45);
		}

		pBoss->GetHierarchy(8)->MatrixCalc(&mtx, pBoss->GetMatrix());

		pos = D3DXVECTOR3(mtx._41, mtx._42, mtx._43);

		for (int nCnt = 0; nCnt < 15; nCnt++)
		{
			CFraction::Create(pos, CFraction::TYPE::TYPE_DIRT, 30, 4, 45);
		}
	}

	if (m_nCount == FIRE_CREATE_RANGE)
	{ // カウントが一定数の場合

		if (pBoss->GetMotion()->GetType() != CBoss::MOTIONTYPE_NEUTRAL)
		{ // 通常モーション以外の場合

			// 通常モーションにする
			pBoss->GetMotion()->Set(CBoss::MOTIONTYPE_NEUTRAL);
		}
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
	if (pBoss->GetMotion()->GetType() != CBoss::MOTIONTYPE_STOMP)
	{ // 足踏みモーション以外の場合

		// 足踏みモーションにする
		pBoss->GetMotion()->Set(CBoss::MOTIONTYPE_STOMP);
	}
}