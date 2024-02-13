//============================================
//
// ゲームのプレイヤーのメイン処理[game_player.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "game_player.h"
#include "motion.h"
#include "manager.h"
#include "file.h"
#include "game.h"
#include "renderer.h"
#include "debugproc.h"

// マクロ定義
#define EMERGENCY_STAGELEFT		(D3DXVECTOR3(-1000.0f, 0.0f, 0.0f))		// 緊急用のステージの左端

//=========================================
// コンストラクタ
//=========================================
CGamePlayer::CGamePlayer() : CPlayer(CObject::TYPE_PLAYER, CObject::PRIORITY_PLAYER)
{
	// 全ての値をクリアする
	m_stageLeft = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ステージの左端
}

//=========================================
// デストラクタ
//=========================================
CGamePlayer::~CGamePlayer()
{

}

//===========================================
// プレイヤーの初期化処理
//===========================================
HRESULT CGamePlayer::Init(void)
{
	if (FAILED(CPlayer::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	if (CManager::GetFile() != nullptr)
	{ // ファイルの情報が NULL じゃない場合

		// ステージの左端を設定する
		m_stageLeft = CManager::GetFile()->GetGameInfo().StageLeftPos;
	}
	else
	{ // 上記以外

		// ステージの左端を設定する
		m_stageLeft = EMERGENCY_STAGELEFT;
	}

	// 値を返す
	return S_OK;
}

//===========================================
// プレイヤーの終了処理
//===========================================
void CGamePlayer::Uninit(void)
{
	// 終了
	CPlayer::Uninit();
}

//===========================================
// プレイヤーの更新処理
//===========================================
void CGamePlayer::Update(void)
{
	switch (CGame::GetState())
	{
	case CGame::STATE_START:		// スタート状態

		// スタート状態の更新処理
		StartUpdate();

		break;

	case CGame::STATE_COUNTDOWN:	// カウントダウン状態

		// カウントダウン状態の更新処理
		CountDownUpdate();

		break;

	case CGame::STATE_PLAY:			// ゲーム状態

		// 通常状態の更新処理
		MainUpdate();

		break;

	case CGame::STATE_GOAL:			// ゴール状態

		// ゴール状態の更新処理
		GoalUpdate();

		break;

	case CGame::STATE_FINISH:		// 終了状態

		// 終了状態の更新処理
		FinishUpdate();

		break;
	}

	// ステージの左端との当たり判定
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
void CGamePlayer::Draw(void)
{
	// 描画処理
	CPlayer::Draw();
}

//===========================================
// ステージの左端の当たり判定
//===========================================
void CGamePlayer::CollisionStageLeft(void)
{
	// 位置を設定する
	D3DXVECTOR3 pos = GetPos();

	if (pos.x <= m_stageLeft.x)
	{ // ステージの左端より左にいた場合

		// プレイヤーの位置をステージの左端に動かす
		pos.x = m_stageLeft.x;
	}

	// 位置を更新する
	SetPos(pos);
}