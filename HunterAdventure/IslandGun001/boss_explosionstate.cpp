//==================================================================
//
// ボスの爆破状態処理 [boss_explosionstate.cpp]
// Author：小原立暉
//
//==================================================================
//******************************************************************
//	インクルードファイル
//******************************************************************
#include "useful.h"
#include "boss.h"
#include "boss_explosionstate.h"
#include "manager.h"
#include "motion.h"
#include "sound.h"

#include "game.h"
#include "alter_flash.h"
#include "boss_ripple.h"
#include "boss_explosion.h"

//------------------------------------------------------------------
// 定数定義
//------------------------------------------------------------------
namespace
{
	const int FINISH_COUNT = 100;		// 終了カウント
	const float RIPPLE_HEIGHT = 600.0f;	// 波紋を出す高さ
}

//==========================
// コンストラクタ
//==========================
CBossExplosionState::CBossExplosionState()
{
	// 全ての値をクリアする
	m_nCount = 0;		// カウント
}

//==========================
// デストラクタ
//==========================
CBossExplosionState::~CBossExplosionState()
{

}

//==========================
// 状態の設定処理
//==========================
void CBossExplosionState::Process(CBoss* pBoss)
{
	// カウントを加算する
	m_nCount++;

	if (m_nCount == FINISH_COUNT)
	{ // 一定時間経過の場合

		// 閃光を生成
		CAlterFlash::Create();

		// ボスの爆破波紋を生成
		CBossRipple::Create(D3DXVECTOR3(pBoss->GetPos().x, pBoss->GetPos().y + RIPPLE_HEIGHT, pBoss->GetPos().z));

		// ボスの爆発を生成
		CBossExplosion::Create(D3DXVECTOR3(pBoss->GetPos().x, pBoss->GetPos().y + RIPPLE_HEIGHT, pBoss->GetPos().z), pBoss->GetRot());

		// ボスの爆発音を鳴らす
		CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_BOSS_EXPLOSION);

		// 終了状態にする
		CGame::SetState(CGame::STATE_FINISH);

		pBoss->Uninit();

		return;
	}
}

//==========================
// 情報の設定処理
//==========================
void CBossExplosionState::SetData(CBoss* pBoss)
{
	// 全ての値を設定する
	m_nCount = 0;		// カウント
}