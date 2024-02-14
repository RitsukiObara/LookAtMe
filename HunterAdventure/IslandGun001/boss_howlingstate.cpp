//================================================================================================================
//
// ボスの雄たけび状態処理 [boss_howlingstate.cpp]
// Author：小原立暉
//
//================================================================================================================
//****************************************************************************************************************
//	インクルードファイル
//****************************************************************************************************************
#include "useful.h"
#include "boss.h"
#include "boss_howlingstate.h"
#include "motion.h"

#include "manager.h"
#include "camera.h"

#include "boss_nonestate.h"

//----------------------------------------------------------------------------------------------------------------
// 無名名前空間
//----------------------------------------------------------------------------------------------------------------
namespace
{
	const int HOWLING_COUNT = 50;				// 雄たけびまでのカウント
	const int HOWLING_CAMERA_COUNT = 63;		// 雄たけびカメラまでのカウント
	const int VIBRATE_CAMERA_COUNT = 120;		// 振動カメラまでのカウント
	const int NONESTATE_COUNT = 240;			// 通常状態までのカウント
	const CCamera::SVibrate LAND_VIB_INFO =		// 着地時の振動情報
	{
		CCamera::TYPE_BOSSCLOSER,	// 次の種類
		0,							// 経過カウント
		5,							// 揺れのカウント
		90,							// 揺れの範囲
		50,							// 終了カウント
		0.3f,						// 補正係数
		false						// 下状況
	};
	const CCamera::SVibrate HOWLING_VIB_INFO =	// 雄たけび時の振動情報
	{
		CCamera::TYPE_NONE,		// 次の種類
		0,						// 経過カウント
		3,						// 揺れのカウント
		400,					// 揺れの範囲
		90,						// 終了カウント
		0.4f,					// 補正係数
		false					// 下状況
	};
}

//==========================
// コンストラクタ
//==========================
CBossHowlingState::CBossHowlingState()
{
	// 全ての値をクリアする
	m_nCount = 0;		// 経過カウント
}

//==========================
// デストラクタ
//==========================
CBossHowlingState::~CBossHowlingState()
{

}

//==========================
// 状態処理
//==========================
void CBossHowlingState::Process(CBoss* pBoss)
{
	// 経過カウントを加算する
	m_nCount++;

	switch (m_nCount)
	{
	case HOWLING_COUNT:				// 雄たけびモーション

		if (pBoss->GetMotion()->GetType() != CBoss::MOTIONTYPE_HOWLING)
		{ // 雄たけびモーションじゃない場合

			// 雄たけびモーションを設定する
			pBoss->GetMotion()->Set(CBoss::MOTIONTYPE_HOWLING);
		}

		break;

	case HOWLING_CAMERA_COUNT:		// 雄たけびカメラ

		if (CManager::Get()->GetCamera()->GetType() != CCamera::TYPE_BOSSHOWLING)
		{ // 雄たけび状態以外の場合

			// 雄たけびカメラにする
			CManager::Get()->GetCamera()->SetType(CCamera::TYPE_BOSSHOWLING);
		}

		break;

	case VIBRATE_CAMERA_COUNT:		// 振動カメラ

		if (CManager::Get()->GetCamera()->GetType() != CCamera::TYPE_VIBRATE)
		{ // 雄たけび状態以外の場合

			// 雄たけびカメラにする
			CManager::Get()->GetCamera()->SetType(CCamera::TYPE_VIBRATE);

			{// 振動の情報を設定する

				// 振動情報を適用する
				CManager::Get()->GetCamera()->SetVibrate(HOWLING_VIB_INFO);
			}
		}

		break;

	case NONESTATE_COUNT:			// 通常状態

		// 通常状態にする
		pBoss->ChangeState(new CBossNoneState);

		// この先の処理を行わない
		return;

		break;
	}
}

//==========================
// 情報の設定処理
//==========================
void CBossHowlingState::SetData(CBoss* pBoss)
{
	// 全ての値を設定する
	m_nCount = 0;		// 経過カウント

	// ホバリングモーションを設定する
	pBoss->GetMotion()->Set(CBoss::MOTIONTYPE_LANDING);

	// 振動カメラにする
	CManager::Get()->GetCamera()->SetType(CCamera::TYPE_VIBRATE);

	{// 振動の情報を設定する

		// 振動情報を適用する
		CManager::Get()->GetCamera()->SetVibrate(LAND_VIB_INFO);
	}
}