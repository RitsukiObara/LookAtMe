//================================================================================================================
//
// ボスの向き直り状態処理 [boss_turnstate.cpp]
// Author：小原立暉
//
//================================================================================================================
//****************************************************************************************************************
//	インクルードファイル
//****************************************************************************************************************
#include "useful.h"
#include "boss.h"
#include "boss_turnstate.h"
#include "motion.h"

#include "game.h"
#include "player.h"
#include "manager.h"
#include "camera.h"

#include "boss_howlingstate.h"

//----------------------------------------------------------------------------------------------------------------
// 無名名前空間
//----------------------------------------------------------------------------------------------------------------
namespace
{
	const float ROT_CORRECT = 0.08f;		// 向きの補正倍率
	const int COUNT = 100;					// 向き直り状態のカウント数
	const float SUB_HEIGHT = 10.0f;			// 高度の減算量
}
//==========================
// コンストラクタ
//==========================
CBossTurnState::CBossTurnState()
{
	// 全ての値をクリアする
	m_nCount = 0;		// 経過カウント
}

//==========================
// デストラクタ
//==========================
CBossTurnState::~CBossTurnState()
{

}

//==========================
// 状態処理
//==========================
void CBossTurnState::Process(CBoss* pBoss)
{
	// プレイヤーの情報を取得する
	CPlayer* pPlayer = CGame::GetPlayer();

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		// カウントを加算する
		m_nCount++;

		// 向きを取得する
		D3DXVECTOR3 rot = pBoss->GetRot();
		D3DXVECTOR3 pos = pBoss->GetPos();
		D3DXVECTOR3 posPlayer = pPlayer->GetPos();
		float fRotDest = atan2f(posPlayer.x - pos.x, posPlayer.z - pos.z);

		// 向きの補正処理
		useful::RotCorrect(fRotDest, &rot.y, ROT_CORRECT);

		// 向きの正規化
		useful::RotNormalize(&rot.y);

		// 向きを適用する
		pBoss->SetRot(rot);

		if (m_nCount >= COUNT)
		{ // 一定数経過した場合

			// ボス出現状態を設定する
			CGame::SetState(CGame::STATE_BOSSMOVIE);

			if (CManager::Get()->GetCamera()->GetType() != CCamera::TYPE_BOSSCLOSER)
			{ // ボス近づき状態以外の場合

				// ボス近づきカメラ状態にする
				CManager::Get()->GetCamera()->SetType(CCamera::TYPE_BOSSCLOSER);
			}

			// 高度を減算する
			pos.y -= SUB_HEIGHT;

			// 位置の設定処理
			pBoss->SetPos(pos);

			// 起伏地面との当たり判定
			if (pBoss->ElevationCollision() == true)
			{ // 地面に着地した場合

				// 向きを補正する
				rot.y = fRotDest;

				// 向きを適用する
				pBoss->SetRot(rot);

				// 遠吠え状態にする
				pBoss->ChangeState(new CBossHowlingState());
			}

			// この先の処理を行わない
			return;
		}
	}
}

//==========================
// 情報の設定処理
//==========================
void CBossTurnState::SetData(CBoss* pBoss)
{
	// 全ての値を設定する
	m_nCount = 0;		// 経過カウント

	// ホバリングモーションを設定する
	pBoss->GetMotion()->Set(CBoss::MOTIONTYPE_HOVERING);
}