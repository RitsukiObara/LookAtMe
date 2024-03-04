//================================================================================================================
//
// ボスの撃破状態処理 [boss_destroystate.cpp]
// Author：小原立暉
//
//================================================================================================================
//****************************************************************************************************************
//	インクルードファイル
//****************************************************************************************************************
#include "useful.h"
#include "boss.h"
#include "boss_destroystate.h"
#include "manager.h"
#include "motion.h"

#include "camera.h"
#include "game.h"
#include "anim_reaction.h"

//----------------------------------------------------------------------------------------------------------------
// 無名名前空間
//----------------------------------------------------------------------------------------------------------------
namespace
{
	const int FINISH_COUNT = 180;			// 終了までのカウント数
	const float INIT_ADD_HEIGHT = 30.0f;	// 高さの加算数の初期値
	const float DEC_HEIGHT = 0.1f;			// 高さの加算数の減速係数
}

//==========================
// コンストラクタ
//==========================
CBossDestroyState::CBossDestroyState()
{
	// 全ての値をクリアする
	m_nCount = 0;						// 経過カウント
	m_fAddHeight = INIT_ADD_HEIGHT;		// 高さの加算数
}

//==========================
// デストラクタ
//==========================
CBossDestroyState::~CBossDestroyState()
{

}

//==========================
// 状態の設定処理
//==========================
void CBossDestroyState::Process(CBoss* pBoss)
{
	// 経過カウントを加算する
	m_nCount++;

	// 移動処理
	Move(pBoss);

	if (m_nCount >= FINISH_COUNT)
	{ // 一定時間経過の場合

		// 終了状態にする
		CGame::SetState(CGame::STATE_FINISH);
	}
}

//==========================
// 情報の設定処理
//==========================
void CBossDestroyState::SetData(CBoss* pBoss)
{
	if (CManager::Get()->GetCamera()->GetType() != CCamera::TYPE_BOSSDESTROY)
	{ // ボスの死亡状態以外の場合

		// ボスの死亡カメラ状態にする
		CManager::Get()->GetCamera()->SetType(CCamera::TYPE_BOSSDESTROY);
	}
}

//==========================
// 移動処理
//==========================
void CBossDestroyState::Move(CBoss* pBoss)
{
	// 位置を取得する
	D3DXVECTOR3 pos = pBoss->GetPos();

	// 高さを設定する
	pos.y += m_fAddHeight;

	// 高さの加算数を減速する
	m_fAddHeight -= DEC_HEIGHT;

	// 位置を適用する
	pBoss->SetPos(pos);
}