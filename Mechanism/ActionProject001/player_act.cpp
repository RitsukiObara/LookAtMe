//============================================
//
// プレイヤーの行動状態のメイン処理[player_act.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "player.h"
#include "player_act.h"
#include "player_ability.h"
#include "collision.h"
#include "useful.h"
#include "input.h"

#include "motion.h"
#include "turn.h"
#include "sound.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define CANNON_COUNT			(40)								// 飛ぶまでのカウント
#define ROT_Y_MOVE				(D3DX_PI * 0.1f)					// 向きの移動量(Y軸)
#define JUMP_MOVE				(20.0f)								// ジャンプの移動量
#define CANNON_Y_MOVE			(30.0f)								// 大砲のY軸の移動量
#define CANNON_Z_MOVE			(15.0f)								// 大砲のZ軸の移動量
#define PLAYER_SPEED			(8.0f)								// プレイヤーのスピード
#define DAMAGE_COUNT			(25)								// ダメージ状態のカウント数
#define INVINCIBLE_ALPHA_CHANGE	(6)									// 無敵状態の透明度が変わるカウント
#define INVINCIBLE_COUNT		(70)								// 無敵状態のカウント数
#define TURN_SHIFT				(D3DXVECTOR3(55.0f, 25.0f, 0.0f))	// 振り向きのずれる座標
#define DEATH_COUNT				(20)								// 死ぬまでのカウント
#define DEATH_JUMP				(20.0f)								// 死んだときのジャンプ量
#define NONE_GRAVITY			(-0.9f)								// 通常状態の重力
#define HOVER_GRAVITY			(0.8f)								// ホバー状態の重力
#define FALL_GRAVITY			(-0.5f)								// 落下状態の重力
#define RUN_SOUND_COUNT			(22)									// 走り音の鳴るカウント数

//============================================
// コンストラクタ
//============================================
CPlayerAct::CPlayerAct()
{
	// 全ての値をクリアする
	m_state = STATE_NONE;			// 状態
	m_nStateCount = 0;			// 状態カウント
	m_bFront = true;			// 前後状況
}

//============================================
// デストラクタ
//============================================
CPlayerAct::~CPlayerAct()
{

}

//============================================
// 初期化処理
//============================================
HRESULT CPlayerAct::Init(void)
{
	// 全ての値を初期化する
	m_state = STATE_NONE;			// 状態
	m_nStateCount = 0;			// 状態カウント
	m_bFront = true;			// 前後状況

	// 成功を返す
	return S_OK;
}

//============================================
// 終了処理
//============================================
void CPlayerAct::Uninit(void)
{
	// 自分のメモリを解放する
	delete this;
}

//============================================
// 更新処理
//============================================
void CPlayerAct::Update(CPlayer& player)
{
	switch (m_state)
	{
	case STATE_NONE:		// 通常状態

		// 状態カウントを加算する
		m_nStateCount++;

		// 通常時の操作処理
		NoneControl(player);

		// 透明度の設定処理
		player.SetAlpha(1.0f);

		break;

	case STATE_DAMAGE:			// ダメージ状態

		// 状態カウントを加算する
		m_nStateCount++;

		if (m_nStateCount >= DAMAGE_COUNT)
		{ // 状態カウントが一定数以上の場合

			// 状態の設定処理
			SetState(STATE_INVINCIBLE);

			// ダメージ状態にする
			player.GetMotion()->Set(CPlayer::MOTIONTYPE_DAMAGE);

			// 向きの決定処理
			RotDecide(player);

			// 透明度を設定する
			player.SetAlpha(0.0f);
		}

		break;

	case STATE_INVINCIBLE:			// 無敵状態

		// 通常時の操作処理
		NoneControl(player);

		// 状態カウントを加算する
		m_nStateCount++;

		if (m_nStateCount % INVINCIBLE_ALPHA_CHANGE == 0)
		{ // 状態カウントが一定数に達するごとに

			// 透明度の入れ替え処理
			player.SwapAlpha();
		}

		if (m_nStateCount >= INVINCIBLE_COUNT)
		{ // 状態カウントが一定数以上だった場合

			// 透明度を設定する
			player.SetAlpha(1.0f);

			// 状態の設定処理
			SetState(STATE_NONE);
		}

		break;

	case STATE_BOUND:		// バウンド状態

		// 重力処理
		Gravity(player);

		// 移動処理
		Move(player);

		// 状態カウントを加算する
		m_nStateCount++;

		if (m_nStateCount >= 10)
		{ // 状態カウントが一定数以上の場合

			// 状態の設定処理
			SetState(STATE_NONE);
		}

		break;

	case STATE_CANNON:		// 大砲準備状態

		// 大砲で向きを変える処理
		CannonRot(player);

		// 重力のリセット処理
		GravityReset(player);

		// 状態カウントを加算する
		m_nStateCount++;

		if (m_nStateCount % CANNON_COUNT == 0)
		{ // 状態カウントが一定数の場合

			// 大砲で飛ぶ処理
			FlyMove(player);

			// 飛んでいる状態にする
			SetState(STATE_FLY);

			// 大砲で飛ぶ音を鳴らす
			CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_CANNON);
		}

		break;

	case STATE_FLY:			// 飛んでいる状態

		{
			// ローカル変数宣言
			D3DXVECTOR3 pos = player.GetPos();			// 位置を取得する
			D3DXVECTOR3 move = player.GetMove();		// 移動量を取得する

			// 重力を加算する
			move.y += NONE_GRAVITY;

			// 移動量を設定する
			pos += move;

			// 位置を設定する
			player.SetPos(pos);

			// 移動量を設定する
			player.SetMove(move);
		}

		break;

	case STATE_DEATH:		// 死亡状態

		// 状態カウントを加算する
		m_nStateCount++;

		if (m_nStateCount % DEATH_COUNT == 0)
		{ // 状態カウントが一定数に達した場合

			// 移動量を取得する
			D3DXVECTOR3 move = player.GetMove();

			// 落下状態にする
			m_state = STATE_FALL;

			// 移動量を設定する
			move.y = DEATH_JUMP;

			// 移動量を適用する
			player.SetMove(move);
		}

		break;

	case STATE_FALL:		// 落下状態

		// 重力処理
		Gravity(player);

		// 移動処理
		Move(player);

		break;

	default:

		// 停止
		assert(false);

		break;
	}
}

//============================================
// 情報の設定処理
//============================================
void CPlayerAct::SetData(void)
{
	// 全ての値を初期化する
	m_state = STATE_NONE;		// 状態
	m_nStateCount = 0;		// 状態カウント
	m_bFront = true;		// 前後関係
}

//============================================
// 状態の設定処理
//============================================
void CPlayerAct::SetState(const STATE act)
{
	// 行動状態を設定する
	m_state = act;

	// 状態カウントを初期化する
	m_nStateCount = 0;
}

//============================================
// 状態の取得処理
//============================================
CPlayerAct::STATE CPlayerAct::GetState(void) const
{
	// 行動状態を返す
	return m_state;
}

//============================================
// 前後状況の設定処理
//============================================
void CPlayerAct::SetFront(bool bFront)
{
	// 前後状況を設定する
	m_bFront = bFront;
}

//============================================
// 前後状況の取得処理
//============================================
bool CPlayerAct::GetFront(void) const
{
	// 前後状況を返す
	return m_bFront;
}

//============================================
// 生成処理
//============================================
CPlayerAct* CPlayerAct::Create(void)
{
	// ローカル変数宣言
	CPlayerAct* pAct = nullptr;		// 行動状態

	if (pAct == nullptr)
	{ // オブジェクトが NULL の場合

		// メモリを確保する
		pAct = new CPlayerAct;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return pAct;
	}

	if (pAct != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pAct->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "プレイヤーの行動の初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pAct->SetData();
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// プレイヤーのポインタを返す
	return pAct;
}

//=======================================
// 通常時の操作処理
//=======================================
void CPlayerAct::NoneControl(CPlayer& player)
{
	if (player.GetAbility()->GetAbility() != CAbility::ABILITY_GROUNDQUAKE)
	{ // グラウンドクエイク状態以外の場合

		// 操作処理
		Control(player);

		// 重力処理
		Gravity(player);

		// 向きの移動処理
		RotMove(player);

		// 移動処理
		Move(player);
	}
	else
	{ // 上記以外

		// グラウンドクエイク処理
		GroundQuake(player);
	}

	// 能力の更新処理
	player.GetAbility()->Update(player);
}

//=======================================
// グラウンドクエイク処理
//=======================================
void CPlayerAct::GroundQuake(CPlayer& player)
{
	// 位置を取得する
	D3DXVECTOR3 move = player.GetMove();

	// 速度を設定する
	player.SetSpeed(0.0f);

	// 移動量を設定する
	move.x = 0.0f;
	move.z = 0.0f;

	// 重力処理
	Gravity(player);

	// 移動処理
	Move(player);

	// 移動量を設定する
	player.SetMove(move);
}

//=======================================
// 向きの決定処理
//=======================================
void CPlayerAct::RotDecide(CPlayer& player)
{
	// 向きを取得する
	D3DXVECTOR3 rot = player.GetRot();

	if (player.IsRight() == true)
	{ // 右向きの場合

		// 向きを設定する
		rot.y = D3DX_PI * 0.5f;
	}
	else
	{ // 上記以外

		// 向きを設定する
		rot.y = -D3DX_PI * 0.5f;
	}

	// 向きを適用する
	player.SetRot(rot);
}

//=======================================
// 操作処理
//=======================================
void CPlayerAct::Control(CPlayer& player)
{
	// ローカル変数宣言
	D3DXVECTOR3 move = player.GetMove();		// 移動量
	D3DXVECTOR3 rotDest = player.GetRotDest();	// 目標の向き

	if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_D) == true)
	{ // Dキーを押した場合

		// モードごとのスピード処理
		ModeSpeed(player);

		// 移動の基本処理
		MoveProcess(player);

		if (player.GetAbility()->GetAbility() != CAbility::ABILITY_JETDASH &&
			player.GetAbility()->GetAbility() != CAbility::ABILITY_DASHJUMP)
		{ // 一定の状態以外の場合

			if (player.IsRight() == false)
			{ // 左向きだった場合

				// 右向きにする
				player.SetEnableRight(true);

				if (player.GetAbility()->GetAbility() != CAbility::ABILITY_JETDASH &&
					player.GetAbility()->GetAbility() != CAbility::ABILITY_DASHJUMP &&
					player.IsJump() == false)
				{ //地面で振り向いた場合

					// 振り向きの位置を設定する
					D3DXVECTOR3 pos = D3DXVECTOR3(player.GetPos().x + sinf(player.GetRot().y) * TURN_SHIFT.x, player.GetPos().y + TURN_SHIFT.y, player.GetPos().z);

					// 振り向き演出の生成
					CTurn::Create(pos, player.IsRight());
				}
			}

			// 目標の向きを設定する
			rotDest.y = D3DX_PI * 0.5f;
		}
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_A) == true)
	{ // Aキーを押した場合

		// モードごとのスピード処理
		ModeSpeed(player);

		// 移動の基本処理
		MoveProcess(player);

		if (player.GetAbility()->GetAbility() != CAbility::ABILITY_JETDASH &&
			player.GetAbility()->GetAbility() != CAbility::ABILITY_DASHJUMP)
		{ // 一定の状態以外の場合

			if (player.IsRight() == true)
			{ // 右向きだった場合

				// 左向き状況にする
				player.SetEnableRight(false);

				if (player.GetAbility()->GetAbility() != CAbility::ABILITY_JETDASH &&
					player.GetAbility()->GetAbility() != CAbility::ABILITY_DASHJUMP &&
					player.IsJump() == false)
				{ //地面で振り向いた場合

					// 振り向きの位置を設定する
					D3DXVECTOR3 pos = D3DXVECTOR3(player.GetPos().x + sinf(player.GetRot().y) * TURN_SHIFT.x, player.GetPos().y + TURN_SHIFT.y, player.GetPos().z);

					// 振り向き演出の生成
					CTurn::Create(pos, player.IsRight());
				}
			}

			// 目標の向きを設定する
			rotDest.y = -D3DX_PI * 0.5f;
		}
	}
	else
	{ // 上記以外

		// 速度を設定する
		player.SetSpeed(0.0f);

		// 移動状況を false にする
		player.SetEnableMove(false);

		if (player.GetAbility()->GetAbility() == CAbility::ABILITY_NONE &&
			player.GetMotion()->GetType() != CPlayer::MOTIONTYPE_NEUTRAL &&
			player.GetMotion()->GetType() != CPlayer::MOTIONTYPE_JUMP &&
			player.GetMotion()->GetType() != CPlayer::MOTIONTYPE_LANDING &&
			player.GetMotion()->GetType() != CPlayer::MOTIONTYPE_APPEAR)
		{ // 待機モーションじゃない場合

			// 待機モーションを設定する
			player.GetMotion()->Set(CPlayer::MOTIONTYPE_NEUTRAL);
		}
	}

	if (CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_SPACE) == true &&
		player.IsJump() == false)
	{ // SPACEキーを押した場合

		// ジャンプの移動量を設定する
		move.y = JUMP_MOVE;

		// ジャンプ状況を true にする
		player.SetEnableJump(true);

		// ジャンプ音を鳴らす
		CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_JUMP);

		if (player.GetAbility()->GetAbility() == CAbility::ABILITY_NONE &&
			player.GetMotion()->GetType() != CPlayer::MOTIONTYPE_JUMP)
		{ // ジャンプモーションじゃない場合

			// ジャンプモーションを設定する
			player.GetMotion()->Set(CPlayer::MOTIONTYPE_JUMP);
		}
	}

	// 目標の向きを適用する
	player.SetRotDest(rotDest);

	// 移動量を適用する
	player.SetMove(move);
}

//=======================================
// 移動の基本処理
//=======================================
void CPlayerAct::MoveProcess(CPlayer& player)
{
	if (player.IsMove() == false)
	{
		// 移動状況を true にする
		player.SetEnableMove(true);

		if (player.IsJump() == false)
		{ // 地上の場合

			// 走り始め音を鳴らす
			CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_RUNSTART);
		}
	}

	if (player.GetAbility()->GetAbility() == CAbility::ABILITY_NONE &&
		player.GetMotion()->GetType() != CPlayer::MOTIONTYPE_MOVE &&
		player.GetMotion()->GetType() != CPlayer::MOTIONTYPE_JUMP &&
		player.GetMotion()->GetType() != CPlayer::MOTIONTYPE_LANDING)
	{ // 移動モーションじゃない場合

		// 移動モーションを設定する
		player.GetMotion()->Set(CPlayer::MOTIONTYPE_MOVE);
	}

	if (player.IsJump() == false &&
		player.GetAbility()->GetAbility() != CAbility::ABILITY_JETDASH &&
		m_nStateCount % RUN_SOUND_COUNT == 0)
	{ // 地上を動いて一定時間経過した場合

		// 走り音を鳴らす
		CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_RUN);
	}
}

//=======================================
// モードごとのスピード処理
//=======================================
void CPlayerAct::ModeSpeed(CPlayer& player)
{
	switch (player.GetAbility()->GetAbility())
	{
	case CAbility::ABILITY_JETDASH:		// ジェットダッシュ

		// 速度を設定する
		player.SetSpeed(0.0f);

		break;

	case CAbility::ABILITY_DASHJUMP:	// ダッシュジャンプ

		// 別のところで速度に関する処理を行っているので無し

		break;

	default:

		// 速度を設定する
		player.SetSpeed(PLAYER_SPEED);

		break;
	}
}

//=======================================
// 移動処理
//=======================================
void CPlayerAct::Move(CPlayer& player)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = player.GetPos();		// 位置を取得する
	D3DXVECTOR3 rot = player.GetRot();		// 向きを取得する
	D3DXVECTOR3 move = player.GetMove();	// 移動量を取得する
	
	// 移動量を設定する
	move.x = sinf(rot.y) * player.GetSpeed();

	// 位置を設定する
	pos += move;

	// 位置を更新する
	player.SetPos(pos);
	player.SetMove(move);
}

//=======================================
// 重力処理
//=======================================
void CPlayerAct::Gravity(CPlayer& player)
{
	// ローカル変数宣言
	D3DXVECTOR3 move = player.GetMove();		// 移動量を取得する

	if (player.GetAbility()->GetAbility() == CAbility::ABILITY_HOVER)
	{ // ホバージェット状態の場合

		// 重力を設定する
		move.y = HOVER_GRAVITY;
	}
	else if (m_state == STATE_FALL)
	{ // 落下状態の場合

		// 重力を加算する
		move.y += FALL_GRAVITY;
	}
	else
	{ // 上記以外

		// 重力を加算する
		move.y += NONE_GRAVITY;
	}

	// 移動量を適用する
	player.SetMove(move);
}

//=======================================
// 向きの移動処理
//=======================================
void CPlayerAct::RotMove(CPlayer& player)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = player.GetRot();				// 向きを取得する
	D3DXVECTOR3 rotDest = player.GetRotDest();		// 目的の向きを取得する

	if (rot.y > rotDest.y - ROT_Y_MOVE &&
		rot.y < rotDest.y + ROT_Y_MOVE)
	{ // 向きが一定数の範囲に入った場合

		// 向きを補正する
		rot.y = rotDest.y;
	}
	else
	{ // 上記以外

		if (player.IsRight() == true)
		{ // 右向き状況の場合

			// 向きを減算する
			rot.y -= ROT_Y_MOVE;
		}
		else
		{ // 上記以外

			// 向きを加算する
			rot.y += ROT_Y_MOVE;
		}

		// 向きを正規化する
		useful::RotNormalize(&rot.y);
	}

	// 向きを更新する
	player.SetRot(rot);
}

//============================================
// 大砲で向きを変える処理
//============================================
void CPlayerAct::CannonRot(CPlayer& player)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = player.GetRot();		// 向きを取得する

	// 向きを設定する
	rot.x = -D3DX_PI * 0.5f;

	if (m_bFront == true)
	{ // 手前にいる場合

		// 向きを設定する
		rot.y = 0.0f;
	}
	else
	{ // 奥にいる場合

		// 向きを設定する
		rot.y = D3DX_PI;
	}

	// 向きを更新する
	player.SetRot(rot);
}

//============================================
// 重力のリセット処理
//============================================
void CPlayerAct::GravityReset(CPlayer& player)
{
	// ローカル変数宣言
	D3DXVECTOR3 move = player.GetMove();		// 移動量を取得する

	// 縦の移動量を設定する
	move.y = 0.0f;

	// 移動量を適用する
	player.SetMove(move);
}

//============================================
// 大砲で飛ぶ処理
//============================================
void CPlayerAct::FlyMove(CPlayer& player)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = player.GetRot();
	D3DXVECTOR3 move = player.GetMove();		// 移動量を取得する

	// 移動量を初期化する
	move.x = 0.0f;
	move.y = CANNON_Y_MOVE;
	move.z = cosf(rot.y) * CANNON_Z_MOVE;

	// 移動量を適用する
	player.SetMove(move);
}