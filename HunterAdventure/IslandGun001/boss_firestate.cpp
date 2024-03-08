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
	const int FIRE_CREATE_RANGE = 180;		// 炎を出している間のカウント数
	const int FINISH_COUNT = 240;			// 終了カウント

	const int FRACTION_COUNT = 3;			// 破片を出す回数
	const int FRACTION_TIMING[FRACTION_COUNT] =		// 破片を出すタイミング
	{
		17,
		104,
		190
	};
	const int NUM_FRACTION = 15;			// 出す破片の総数
	const int RAND_FRACTION_LIFE = 30;		// 破片の寿命のランダム値
	const int MIN_FRACTION_LIFE = 15;		// 破片の寿命の最小値
	const int FRACTION_WIDTH = 4;			// 破片の幅
	const int FRACTION_HEIGHT = 45;			// 破片の高さ
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

	// 炎生成処理
	FireCreate(pBoss);
	
	if (m_nCount == FRACTION_TIMING[0] ||
		m_nCount == FRACTION_TIMING[1] ||
		m_nCount == FRACTION_TIMING[2])
	{ // 規定のカウント数になった場合

		// 破片を出す処理
		Fraction(pBoss);
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

//==========================
// 炎を出す処理
//==========================
void CBossFireState::FireCreate(CBoss* pBoss)
{
	CPlayer* pPlayer = CGame::GetPlayer();

	if (m_nCount % FIRE_CREATE_COUNT == 0 &&
		m_nCount <= FIRE_CREATE_RANGE &&
		pPlayer != nullptr)
	{ // 経過カウントが一定数経過した場合

		// 炎注意の生成
		CFireWarning::Create(pPlayer->GetPos());
	}
}

//==========================
// 破片出す処理
//==========================
void CBossFireState::Fraction(CBoss* pBoss)
{
	D3DXMATRIX mtx;
	D3DXVECTOR3 pos[2] = {};
	int nLife = 0;

	for (int nCntFrac = 0; nCntFrac < 2; nCntFrac++)
	{
		// 位置を設定
		pBoss->GetHierarchy(nCntFrac + 7)->MatrixCalc(&mtx, pBoss->GetMatrix());
		pos[nCntFrac] = D3DXVECTOR3(mtx._41, mtx._42, mtx._43);

		for (int nCnt = 0; nCnt < NUM_FRACTION; nCnt++)
		{
			// 破片を生成
			nLife = rand() % RAND_FRACTION_LIFE + MIN_FRACTION_LIFE;
			CFraction::Create(pos[nCntFrac], CFraction::TYPE::TYPE_DIRT, nLife, FRACTION_WIDTH, FRACTION_HEIGHT);
		}
	}
}