//============================================
//
// チュートリアルのプレイヤーのメイン処理[tutorial_player.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "tutorial_player.h"
#include "tutorial.h"
#include "motion.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "debugproc.h"
#include "collision.h"

#include "balloon.h"
#include "balloonUI.h"
#include "push_timing.h"
#include "speedUI.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define POLEJUMP_SPEED			(11.0f)			// 街灯ジャンプのスピード
#define PUSH_TIMING_ADD_POS		(D3DXVECTOR3(-90.0f, 80.0f, 0.0f))		// 押しタイミングの追加位置
#define PUSH_CLIFFCLIMB_COUNT	(13)			// 崖登り時の押しタイミングの表示するカウント
#define PUSH_CLIFFLAND_COUNT	(5)				// 崖着地時の押しタイミングの表示するカウント
#define PUSH_HANGING_ROT		(0.9f)			// ぶら下がり時の押しタイミングの表示する向き
#define STAGE_LEFT				(-700.0f)		// ステージの左端

//=========================================
// コンストラクタ
//=========================================
CTutorialPlayer::CTutorialPlayer() : CPlayer(CObject::TYPE_PLAYER, CObject::PRIORITY_PLAYER)
{
	// 全ての値をクリアする
	m_pPushTiming = nullptr;		// 押しタイミングのポインタ
}

//=========================================
// デストラクタ
//=========================================
CTutorialPlayer::~CTutorialPlayer()
{

}

//===========================================
// プレイヤーの初期化処理
//===========================================
HRESULT CTutorialPlayer::Init(void)
{
	if (FAILED(CPlayer::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値をクリアする
	m_pPushTiming = nullptr;		// 押しタイミングのポインタ

	// 値を返す
	return S_OK;
}

//===========================================
// プレイヤーの終了処理
//===========================================
void CTutorialPlayer::Uninit(void)
{
	if (m_pPushTiming != nullptr)
	{ // 押しタイミングが NULL じゃない場合

		// 終了処理
		m_pPushTiming->Uninit();
		m_pPushTiming = nullptr;
	}

	// 終了
	CPlayer::Uninit();
}

//===========================================
// プレイヤーの更新処理
//===========================================
void CTutorialPlayer::Update(void)
{
	// プレイヤーの吹き出しと当たり判定
	collision::PlayerBalloonHit(this);

	if (CTutorial::GetState() != CTutorial::STATE_STAMINA)
	{ // スタミナモード以外の場合

		// スタミナを全回復させる
		CalcStamina(PLAYER_MAX_STAMINA);
	}

	if (CTutorial::GetState() == CTutorial::STATE_BOOST)
	{ // ブーストモードの場合

		if (GetSpeed() < PLAYER_MAX_SPEED)
		{ // 速度が最大値未満の場合

			// 速度を最大にする
			SetSpeed(PLAYER_MAX_SPEED);
		}
	}
	else if (CTutorial::GetState() == CTutorial::STATE_POLE)
	{ // 街灯ジャンプの場合

		if (GetSpeed() < POLEJUMP_SPEED)
		{ // 速度が一定の速度以下になった場合

			// 速度を設定する
			SetSpeed(POLEJUMP_SPEED);
		}

		if (GetState() == STATE_BOOST)
		{ // ブースト状態の場合

			// 通常状態にする
			SetState(STATE_NONE);
		}
	}
	else
	{ // 上記以外

		if (GetState() == STATE_BOOST)
		{ // ブースト状態の場合

			// 通常状態にする
			SetState(STATE_NONE);
		}
	}

	if (CTutorial::GetState() == CTutorial::STATE_END)
	{ // 終了状態の場合

		// 右を向かせる
		SetRight(true);

		// 終了時の更新処理
		FinishUpdate();
	}
	else
	{ // 上記以外

		// 通常状態の更新処理
		MainUpdate();
	}

	// 押しタイミングの位置設定処理
	m_pPushTiming->SetPos(D3DXVECTOR3(GetPos().x + PUSH_TIMING_ADD_POS.x, GetPos().y + PUSH_TIMING_ADD_POS.y, GetPos().z));

	if (GetAction()->GetAct() == CPlayerAct::ACT_CLIFFCLIMB &&
		GetAction()->GetCliffCatch() == true &&
		GetAction()->GetActCount() >= PUSH_CLIFFCLIMB_COUNT)
	{ // 崖登りの場合
		
		// 描画状況を設定する
		m_pPushTiming->SetDisp(true);
	}
	else if (GetAction()->GetAct() == CPlayerAct::ACT_CLIFFLAND &&
		GetAction()->GetCliffCatch() == true &&
		GetAction()->GetActCount() <= PUSH_CLIFFLAND_COUNT)
	{ // 崖着地の場合

		// 描画状況を設定する
		m_pPushTiming->SetDisp(true);
	}
	else if (GetAction()->GetAct() == CPlayerAct::ACT_HANGING &&
		GetRot().x >= PUSH_HANGING_ROT)
	{ // ぶら下がりの場合

		// 描画状況を設定する
		m_pPushTiming->SetDisp(true);
	}
	else if (GetAction()->GetAct() == CPlayerAct::ACT_WALLFRIC &&
		GetStamina() >= PLAYER_WALLKICK_STAMINA)
	{ // 壁擦り状態の場合

		// 描画状況を設定する
		m_pPushTiming->SetDisp(true);
	}
	else
	{ // 上記以外

		// 描画状況を設定する
		m_pPushTiming->SetDisp(false);
	}

	// ステージの左端の当たり判定
	CollisionStageLeft();

	// プレイヤーのデバッグ・操作方法表示
	CManager::GetDebugProc()->Print
	(
		"位置[%f %f %f]\n\n移動[A/D]\nジャンプ[SPACE]\n\n向き：%f\nジャンプ状況：%d\n行動状況：%d\n速度：%f\nモーションの種類：%d\n"
		, GetPos().x, GetPos().y, GetPos().z, GetRot().y, GetJump(), GetAction()->GetAct(), GetSpeed(), GetMotion()->GetType()
	);
}

//===========================================
// プレイヤーの描画処理
//===========================================
void CTutorialPlayer::Draw(void)
{
	// 描画処理
	CPlayer::Draw();
}

//===========================================
// 情報の設定処理
//===========================================
void CTutorialPlayer::SetData(const D3DXVECTOR3& pos, const CScene::MODE mode)
{
	// ローカル変数宣言
	D3DXVECTOR3 PushPos;		// 押しタイミングの位置

	// 情報の設定処理
	CPlayer::SetData(pos, mode);

	// 押しタイミングの位置を設定する
	PushPos = D3DXVECTOR3(pos.x + PUSH_TIMING_ADD_POS.x, pos.y + PUSH_TIMING_ADD_POS.y, pos.z);

	// 押しタイミングを生成
	m_pPushTiming = CPushTiming::Create(PushPos);
}

//===========================================
// ステージの左端との当たり判定
//===========================================
void CTutorialPlayer::CollisionStageLeft(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();

	if (pos.x <= STAGE_LEFT)
	{ // ステージの左端より左にいた場合

		// プレイヤーの位置を動かす
		pos.x = STAGE_LEFT;
	}

	// 位置を更新する
	SetPos(pos);
}