//=======================================
//
// ゲームのゴールのメイン処理[game_goal.cpp]
// Author：小原立暉
//
//=======================================
#include "game.h"
#include "manager.h"
#include "game_goal.h"

#include "collision.h"
#include "game_time.h"
#include "goalUI.h"
#include "player.h"
#include "score.h"
#include "sound.h"

//---------------------------------------
// マクロ定義
//---------------------------------------

//==========================================
// コンストラクタ
//==========================================
CGameGoal::CGameGoal() : CGoal(CObject::TYPE_GOAL, CObject::PRIORITY_PLAYER)
{

}

//==========================================
// デストラクタ
//==========================================
CGameGoal::~CGameGoal()
{

}

//==========================================
// ゴールの初期化処理
//==========================================
HRESULT CGameGoal::Init(void)
{
	if (FAILED(CGoal::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//========================================
// ゴールの終了処理
//========================================
void CGameGoal::Uninit(void)
{
	// 終了処理
	CGoal::Uninit();
}

//========================================
// ゴールの更新処理
//========================================
void CGameGoal::Update(void)
{
	// 透明度の変化
	AlphaChange();

	// ゴールとプレイヤーの当たり判定
	if (collision::GoalPlayerCollision(this) == true)
	{ // 当たり判定が true だった場合

		// ゴール時の歓声を鳴らす
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_GOALCHEERS);

		if (CScene::GetPlayer() != nullptr)
		{ // プレイヤーが NULL じゃない場合

			// ゴールUIの生成処理
			CGoalUI::Create(CScene::GetPlayer()->GetPos());
		}

		if (CGame::GetTime() != nullptr)
		{ // タイムの情報があった場合

			// クリアタイムを設定する
			CManager::SetClearTime(CGame::GetTime()->GetTotalTime());
		}

		if (CGame::GetScore() != nullptr)
		{ // スコアの情報があった場合

			// アクションスコアを設定する
			CManager::SetScore(CGame::GetScore()->GetScore());
		}

		// クリア状況の設定処理
		CManager::SetClear(true);

		// ゲームの進行状態の設定処理
		CGame::SetState(CGame::STATE_GOAL);

		// 終了処理
		Uninit();

		// この先の処理を行わない
		return;
	}
}

//===========================================
// ゴールの描画処理
//===========================================
void CGameGoal::Draw(void)
{
	// 描画処理
	CGoal::Draw();
}