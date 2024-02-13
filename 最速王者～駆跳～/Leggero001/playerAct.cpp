//============================================
//
// プレイヤー行動管理のメイン処理[playerAct.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "scene.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "playerAct.h"
#include "player.h"
#include "motion.h"
#include "landUI.h"

#include "actionUI.h"
#include "aura.h"
#include "debugproc.h"
#include "Effect.h"
#include "fraction.h"
#include "orbit.h"
#include "ripple.h"
#include "rubsmoke.h"
#include "score.h"
#include "speedUI.h"
#include "sound.h"
#include "useful.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define PLAYER_WALLKICK_SPEED_R			(D3DXVECTOR3(8.0f, 17.0f, 0.0f))		// 壁キック時の右に飛ぶ速度
#define PLAYER_WALLKICK_SPEED_L			(D3DXVECTOR3(-8.0f, 17.0f, 0.0f))		// 壁キック時の左に飛ぶ速度
#define WALLKICK_FRACTION_COL			(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f))		// 壁キック時の破片の色
#define PLAYER_WALLFRIC_GRAVITY			(-0.5f)		// プレイヤーの壁擦り状態の重力
#define PLAYER_BACKPACK_NUM				(1)			// バックパックのあるモデルの番号
#define PLAYER_JET_SHIFT_DIST			(D3DXVECTOR3(-7.0f,29.0f,0.0f))		// ジェット噴射のとき体の中心からずらす距離
#define PLAYER_JET_ADD_MOVE				(-9.5f)		// ジェット噴射の追加移動量
#define PLAYER_BIGJUMP_ADD_POS_Y		(30.0f)		// 超ジャンプ時のエフェクトの位置の加算数
#define PLAYER_CLIFF_SPEED_SUCCESS		(0.3f)		// 崖離れで成功したときの速度
#define PLAYER_CLIFF_JUMP_SUCCESS		(10.0f)		// 崖離れで成功したときのジャンプ量
#define PLAYER_CLIFF_GRAVITY_SUCCESS	(0.5f)		// 崖離れで成功したときの重力
#define PLAYER_CLIFF_SPEED_FAILED		(0.7f)		// 崖離れで失敗したときの速度
#define PLAYER_CLIFF_JUMP_FAILED		(4.2f)		// 崖離れで失敗したときのジャンプ量
#define PLAYER_CLIFF_GRAVITY_FAILED		(0.095f)	// 崖離れで失敗したときの重力
#define PLAYER_HANGING_DESTROT_SUB		(-0.7f)		// ぶら下がり状態の目的の向きの移動量の減衰係数
#define PLAYER_HANGING_ROT_SUB			(0.05f)		// ぶら下がり状態の向きの移動量の減衰係数
#define PLAYER_POLEDOWN_POSY_MOVE		(3.5f)		// ポール下り状態のY軸の移動量
#define PLAYER_POLEDOWN_ROT_MOVE		(0.25f)		// ポール下り状態の向きの移動量
#define PLAYER_RUB_SOUND_COUNT			(60)		// 擦る音を鳴らすカウント数
#define PLAYER_FAST_LAMPJUMP_MOVE_X		(10.0f)		// 最速街灯ジャンプの移動量(X軸)
#define PLAYER_LAMPJUMP_MOVE_X_MAGNI	(11.0f)		// 街灯ジャンプの移動量の倍率(X軸)
#define PLAYER_LAMPJUMP_MIN_MOVE_Y		(5.0f)		// 街灯ジャンプの移動量の最小限(Y軸)
#define PLAYER_LAMPJUMP_SCORE_MOVE		(12.0f)		// 街灯ジャンプで得点が入る移動量

//=========================================
// コンストラクタ
//=========================================
CPlayerAct::CPlayerAct()
{
	// 全ての値をクリアする
	m_posObject = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 関与しているオブジェクトの位置
	m_Action = ACT_NONE;		// 行動
	m_nActCount = 0;			// 行動カウント
	m_fCliffUpSpeed = 0.0f;		// 崖登りの速度
	m_fCliffGravity = 0.0f;		// 崖上がり時の重力
	m_fRotDest = 0.0f;			// 目標の向き
	m_bWallOld = false;			// 前回の壁擦り状況
	m_bRightKick = false;		// 右壁を蹴ったかどうか
	m_bRightLeave = false;		// 壁離れ時の向き
	m_bCliffSuccess = false;	// 崖上がりに成功したかどうか
}

//=========================================
// デストラクタ
//=========================================
CPlayerAct::~CPlayerAct()
{

}

//===========================================
// プレイヤーの初期化処理
//===========================================
HRESULT CPlayerAct::Init(void)
{
	// 全ての値を初期化する
	m_posObject = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 関与しているオブジェクトの位置
	m_Action = ACT_NONE;		// 行動
	m_nActCount = 0;			// 行動カウント
	m_fCliffUpSpeed = 0.0f;		// 崖登りの速度
	m_fCliffGravity = 0.0f;		// 崖上がり時の重力
	m_fRotDest = 0.0f;			// 目標の向き
	m_bWallOld = false;			// 前回の壁擦り状況
	m_bRightKick = false;		// 右壁を蹴ったかどうか
	m_bRightLeave = false;		// 壁離れ時の向き
	m_bCliffSuccess = false;	// 崖上がりに成功したかどうか

	// 値を返す
	return S_OK;
}

//===========================================
// プレイヤーの終了処理
//===========================================
void CPlayerAct::Uninit(void)
{

}

//===========================================
// 生成処理
//===========================================
CPlayerAct* CPlayerAct::Create(void)
{
	// ローカルポインタ宣言
	CPlayerAct* pPlayerAct = nullptr;

	if (pPlayerAct == nullptr)
	{ // プレイヤーのポインタが NULL の場合

		// メモリを確保する
		pPlayerAct = new CPlayerAct;
	}
	else
	{ // プレイヤーのポインタが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pPlayerAct != nullptr)
	{ // メモリ確保に成功していた場合

		if (FAILED(pPlayerAct->Init()))
		{ // 初期化に失敗した場合

			// NULL を返す
			return nullptr;
		}
	}
	else
	{ // メモリ確保に失敗した場合

		// NULL を返す
		return nullptr;
	}

	// ポインタ を返す
	return pPlayerAct;
}

//===========================================
// 状態の設定処理
//===========================================
void CPlayerAct::SetAct(const ACT act)
{
	// 状態を設定する
	m_Action = act;
}

//===========================================
// 状態の取得処理
//===========================================
CPlayerAct::ACT CPlayerAct::GetAct(void) const
{
	// 状態を返す
	return m_Action;
}

//===========================================
// 前回の壁擦り状況の設定処理
//===========================================
void CPlayerAct::SetWallOld(const bool bWall)
{
	// 前回の壁擦り状況を設定する
	m_bWallOld = bWall;
}

//===========================================
// 前回の壁擦り状況の取得処理
//===========================================
bool CPlayerAct::GetWallOld(void) const
{
	// 前回の壁擦り状況を返す
	return m_bWallOld;
}

//===========================================
// 壁離れ時の向き設定処理
//===========================================
void CPlayerAct::SetRightLeave(const bool bRightLeave)
{
	// 壁離れ時の向きを設定する
	m_bRightLeave = bRightLeave;
}

//===========================================
// 壁離れ時の向き取得処理
//===========================================
bool CPlayerAct::GetRightLeave(void) const
{
	// 壁離れ時の向きを返す
	return m_bRightLeave;
}

//===========================================
// 右壁を蹴ったかの設定処理
//===========================================
void CPlayerAct::SetRightKick(const bool bRight)
{
	// 右壁を蹴ったかどうかを設定する
	m_bRightKick = bRight;
}

//===========================================
// 関与しているオブジェクトの位置の設定処理
//===========================================
void CPlayerAct::SetPosObject(const D3DXVECTOR3& pos)
{
	// 関与しているオブジェクト位置を設定する
	m_posObject = pos;
}

//===========================================
// 関与しているオブジェクトの位置の取得処理
//===========================================
D3DXVECTOR3 CPlayerAct::GetPosObject(void) const
{
	// 関与しているオブジェクト位置を返す
	return m_posObject;
}

//===========================================
// 崖捕まり時の重力の取得処理
//===========================================
float CPlayerAct::GetCliffGravity(void) const
{
	// 崖離れ時の重力を返す
	return m_fCliffGravity;
}

//===========================================
// 目標の向きの設定処理
//===========================================
void CPlayerAct::SetRotDest(const float fRot)
{
	// 向きの移動量を設定する
	m_fRotDest = fRot;
}

//=======================================
// 行動状態処理
//=======================================
void CPlayerAct::Action(CPlayer& player)
{
	switch (m_Action)
	{
	case ACT_NONE:					// 無し

		// 通常行動処理
		ActNone(player);

		break;

	case ACT_BIGJUMP:				// 超ジャンプ

		// 超ジャンプ行動処理
		ActSuperJump(player);

		break;

	case ACT_WALLFRIC:				// 壁擦り状態

		// 壁擦り行動処理
		ActWallFriction(player);

		break;

	case ACT_WALLLEAVE:				// 壁離れ状態

		// 壁離れ行動処理
		ActWallLeave(player);

		break;

	case ACT_WALLSTEPIN:			// 壁踏み込み処理

		// 壁踏み込み処理
		ActWallStepIn(player);

		break;

	case ACT_WALLKICK:				// 壁キック

		// 壁キック行動処理
		ActWallKick(player);

		break;

	case ACT_WALLAFTERGLOW:			// 壁キックの余韻

		// 壁キックの余韻行動処理
		ActWallAfterGlaw(player);

		break;

	case ACT_CLIFFCATCH:			// 崖つかまり状態

		// 崖捕まり行動処理
		ActCliffCatch(player);

		break;

	case ACT_CLIFFCLIMB:			// 崖降り

		// 崖登り行動処理
		ActCliffClimb(player);

		break;

	case ACT_CLIFFFALL:				// 崖降り状態

		// 崖降り行動処理
		ActCliffFall(player);

		break;

	case ACT_CLIFFLAND:				// 崖離し着地

		// 崖登り着地行動処理
		ActCliffLand(player);

		break;

	case ACT_CLIFFAFTERGLOW:		// 余韻状態

		// 余韻行動処理
		ActCliffAfterGlaw(player);

		break;

	case ACT_HANGING:				// ぶら下がり状態

		// ぶら下がり行動状態
		ActHanging(player);

		break;

	case ACT_LAMPJUMP:				// ランプから飛んでいる状態

		// ランプから飛んでいる状態
		ActLampJump(player);

		break;

	case ACT_POLEDOWN:				// ポール下り状態

		// ポール下り状態処理
		ActPoleDown(player);

		break;

	default:						// 上記以外

		// 行動変化処理
		ActionChange(ACT_NONE);

		break;
	}
}

//=======================================
// アクション変化処理
//=======================================
void CPlayerAct::ActionChange(const ACT action)
{
	// 行動状態の設定
	m_Action = action;

	// 行動カウントを初期化する
	m_nActCount = 0;
}

//=======================================
// 崖登り成功判定の取得処理
//=======================================
bool CPlayerAct::GetCliffCatch(void) const
{
	// 崖登り成功判定を返す
	return m_bCliffSuccess;
}

//=======================================
// アクションカウントの取得処理
//=======================================
int CPlayerAct::GetActCount(void) const
{
	// 攻撃カウントを返す
	return m_nActCount;
}

//=======================================
// 通常行動処理
//=======================================
void CPlayerAct::ActNone(CPlayer& player)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = player.GetRot();		// 向きを取得

	// 行動カウントを0にする
	m_nActCount = 0;

	if (player.GetRight() == true)
	{ // 右を向いていた場合

		// 向きを右向きにする
		rot.y = D3DX_PI * 0.5f;
	}
	else
	{ // 左を向いていた場合

		// 向きを左向きにする
		rot.y = D3DX_PI * -0.5f;
	}

	// 向きを設定する
	player.SetRot(rot);
}

//=======================================
// 超ジャンプ行動処理
//=======================================
void CPlayerAct::ActSuperJump(CPlayer& player)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = player.GetRot();		// 向きを取得する

	if (player.GetRight() == true)
	{ // 右を向いていた場合

		// 向きを右向きにする
		rot.y = D3DX_PI * 0.5f;
	}
	else
	{ // 左を向いていた場合

		// 向きを左向きにする
		rot.y = D3DX_PI * -0.5f;
	}

	// 行動カウントを加算する
	m_nActCount++;

	// 超ジャンプ時のエフェクト処理
	SuperJumpEffect(player);

	if (player.GetMove().y >= 0.0f)
	{ // 縦の移動量が 0.0f 以上の場合

		// ローカル変数宣言
		D3DXVECTOR3 BodyPos = player.GetPos() + player.GetHierarchy(PLAYER_BACKPACK_NUM)->GetPos();		// 体の位置を取得する
		D3DXVECTOR3 EffectMove = player.GetMove();														// エフェクトの移動量を取得する

		// 体の位置を設定する
		BodyPos.x += sinf(player.GetRot().y) * PLAYER_JET_SHIFT_DIST.x;
		BodyPos.y += PLAYER_JET_SHIFT_DIST.y;

		// エフェクトの移動量を設定する
		EffectMove.y += PLAYER_JET_ADD_MOVE;

		// エフェクトの設定処理
		CEffect::Create(BodyPos, EffectMove, 6, 10.0f, CEffect::TYPE_FIRE);
	}

	// 向きの設定処理
	player.SetRot(rot);
}

//=======================================
// 壁擦り行動処理
//=======================================
void CPlayerAct::ActWallFriction(CPlayer& player)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = player.GetPos();		// 位置を取得する
	D3DXVECTOR3 move = player.GetMove();	// 移動量を取得する
	D3DXVECTOR3 rot = player.GetRot();		// 向きを取得する

	if (player.GetRight() == true)
	{ // 右を向いていた場合

		// 向きを右向きにする
		rot.y = D3DX_PI * 0.5f;
	}
	else
	{ // 左を向いていた場合

		// 向きを左向きにする
		rot.y = D3DX_PI * -0.5f;
	}

	if (move.y <= 1.0f)
	{ // 移動量が 0.0f 以下の場合

		// 重力を制限する
		move.y = PLAYER_WALLFRIC_GRAVITY;
	}

	if (m_nActCount % PLAYER_WALLFRIC_DUST_COUNT == 0)
	{ // 行動カウントが一定数に達した場合

		// ローカル変数宣言
		D3DXVECTOR3 RubPos;			// 煙の位置

		// 位置を設定する
		RubPos.x = sinf(rot.y) * PLAYER_WIDTH;
		RubPos.y = (float)(rand() % (int)(PLAYER_HEIGHT));
		RubPos.z = 0.0f;

		// 爆発(埃)の生成処理
		CRubSmoke::Create(pos + RubPos);
	}

	if (m_nActCount % PLAYER_RUB_SOUND_COUNT == 0)
	{ // 行動カウントが一定数に達した場合

		// 体を擦る音を鳴らす
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_RUBBING);
	}

	// 行動カウントを加算する
	m_nActCount++;

	if(player.GetStamina() >= PLAYER_WALLKICK_STAMINA)
	{ // プレイヤーに壁キックできるスタミナがあった場合

		// ローカル変数宣言
		D3DXVECTOR3 EffectPos;		// エフェクトの位置

		// エフェクトの位置を設定する
		EffectPos.x = rand() % ((int)(PLAYER_WIDTH) * 2) - PLAYER_WIDTH;
		EffectPos.y = (float)(rand() % (int)(AURA_RAND_SIZE));
		EffectPos.z = 0.0f;

		// オーラを生成する
		CAura::Create(pos + EffectPos, AURA_NORMAL_LIFE);
	}

	// 情報を更新する
	player.SetRot(rot);		// 向き
	player.SetMove(move);	// 移動量
}

//=======================================
// 壁離し行動処理
//=======================================
void CPlayerAct::ActWallLeave(CPlayer& player)
{
	// ローカル変数宣言
	D3DXVECTOR3 move = player.GetMove();	// 移動量を取得する
	D3DXVECTOR3 rot = player.GetRot();		// 向きを取得する

	// 行動カウントを加算する
	m_nActCount++;

	if (m_nActCount >= PLAYER_WALLLEAVE_COUNT)
	{ // 行動カウントが一定数以上になった場合

		// 通常行動に変える
		ActionChange(ACT_NONE);
	}

	// 重力を制限する
	move.y = PLAYER_WALLFRIC_GRAVITY;

	// ローカル変数宣言
	D3DXVECTOR3 EffectPos;		// エフェクトの位置

	// エフェクトの位置を設定する
	EffectPos.x = rand() % ((int)(PLAYER_WIDTH) * 2) - PLAYER_WIDTH;
	EffectPos.y = (float)(rand() % (int)(AURA_RAND_SIZE));
	EffectPos.z = 0.0f;

	// オーラを生成する
	CAura::Create(player.GetPos() + EffectPos, AURA_NORMAL_LIFE);

	// 向きの設定処理
	player.SetRot(rot);		// 向き
	player.SetMove(move);	// 移動量
}

//=======================================
// 壁踏み込み処理
//=======================================
void CPlayerAct::ActWallStepIn(CPlayer& player)
{
	// ローカル変数宣言
	D3DXVECTOR3 move = player.GetMove();	// 移動量を取得する

	// 重力を制限する
	move.y = 0.0f;

	if (player.GetMotion()->GetType() != CPlayer::MOTIONTYPE_WALLSTEPIN)
	{ // モーションが壁踏み込みじゃない場合

		if (m_bRightKick == true)
		{ // 右壁を蹴った場合

			// 壁キック(右版)
			WallKickRight(player);
		}
		else
		{ // 左壁を蹴った場合

			// 壁キック(左版)
			WallKickLeft(player);
		}

		// この先の処理を行わない
		return;
	}

	// 情報を更新する
	player.SetMove(move);	// 移動量
}

//=======================================
// 壁キック行動処理
//=======================================
void CPlayerAct::ActWallKick(CPlayer& player)
{
	// 行動カウントを加算する
	m_nActCount++;

	if (m_nActCount >= PLAYER_WALLKICK_COUNT)
	{ // 一定数に達した場合

		// 通常状態にする
		m_Action = ACT_NONE;

		// 行動カウントを 0 にする
		m_nActCount = 0;
	}

	if (player.GetMove().y >= 0.0f)
	{ // 縦の移動量が 0.0f 以上の場合

		// ローカル変数宣言
		D3DXVECTOR3 BodyPos = player.GetPos() + player.GetHierarchy(PLAYER_BACKPACK_NUM)->GetPos();		// 体の位置を取得する
		D3DXVECTOR3 EffectMove = player.GetMove();														// エフェクトの移動量を取得する

		// 体の位置を設定する
		BodyPos.x += sinf(player.GetHierarchy(PLAYER_BACKPACK_NUM)->GetRot().y) * PLAYER_JET_SHIFT_DIST.x;
		BodyPos.y += PLAYER_JET_SHIFT_DIST.y;

		// エフェクトの移動量を設定する
		EffectMove.y += PLAYER_JET_ADD_MOVE;

		// エフェクトの設定処理
		CEffect::Create(BodyPos, EffectMove, 6, 10.0f, CEffect::TYPE_FIRE);
	}
}

//=======================================
// 壁キックの余韻行動処理
//=======================================
void CPlayerAct::ActWallAfterGlaw(CPlayer& player)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = player.GetPos();		// 位置を取得する
	D3DXVECTOR3 move = player.GetMove();	// 移動量を取得する
	D3DXVECTOR3 rot = player.GetRot();		// 向きを取得する

	if (m_nActCount % PLAYER_RUB_SOUND_COUNT == 0)
	{ // 行動カウントが一定数に達した場合

		// 体を擦る音を鳴らす
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_RUBBING);
	}

	// 行動カウントを加算する
	m_nActCount++;

	if (m_nActCount >= PLAYER_WALLAFTERGLOW_COUNT)
	{ // 一定数に達した場合

		// 通常状態にする
		m_Action = ACT_NONE;

		// 行動カウントを 0 にする
		m_nActCount = 0;
	}

	if (player.GetMove().y >= 0.0f)
	{ // 縦の移動量が 0.0f 以上の場合

		// ローカル変数宣言
		D3DXVECTOR3 BodyPos = pos + player.GetHierarchy(PLAYER_BACKPACK_NUM)->GetPos();		// 体の位置を取得する
		D3DXVECTOR3 EffectMove = move;											// エフェクトの移動量を取得する

		// 体の位置を設定する
		BodyPos.x += sinf(player.GetHierarchy(PLAYER_BACKPACK_NUM)->GetRot().y) * PLAYER_JET_SHIFT_DIST.x;
		BodyPos.y += PLAYER_JET_SHIFT_DIST.y;

		// エフェクトの移動量を設定する
		EffectMove.y += PLAYER_JET_ADD_MOVE;

		// エフェクトの設定処理
		CEffect::Create(BodyPos, EffectMove, 6, 10.0f, CEffect::TYPE_FIRE);
	}

	if (move.y <= 1.0f)
	{ // 移動量が 0.0f 以下の場合

		// 重力を制限する
		move.y = PLAYER_WALLFRIC_GRAVITY;
	}

	if (m_nActCount % PLAYER_WALLFRIC_DUST_COUNT == 0)
	{ // 行動カウントが一定数に達した場合

		// ローカル変数宣言
		D3DXVECTOR3 RubPos;		// 煙の位置

		// 位置を設定する
		RubPos.x = sinf(rot.y) * PLAYER_WIDTH;
		RubPos.y = (float)(rand() % (int)(PLAYER_HEIGHT));
		RubPos.z = 0.0f;

		// 爆発(埃)の生成処理
		CRubSmoke::Create(pos + RubPos);
	}

	// ローカル変数宣言
	D3DXVECTOR3 EffectPos;		// エフェクトの位置

	// エフェクトの位置を設定する
	EffectPos.x = rand() % ((int)(PLAYER_WIDTH) * 2) - PLAYER_WIDTH;
	EffectPos.y = (float)(rand() % (int)(AURA_RAND_SIZE));
	EffectPos.z = 0.0f;

	// オーラを生成する
	CAura::Create(pos + EffectPos, AURA_NORMAL_LIFE);

	// 情報を更新する
	player.SetMove(move);	// 移動量
}

//=======================================
// 崖捕まり行動処理
//=======================================
void CPlayerAct::ActCliffCatch(CPlayer& player)
{
	// ローカル変数宣言
	D3DXVECTOR3 move = player.GetMove();		// 移動量を取得する

	// 移動量を0.0fにする
	move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// カウントを加算する
	m_nActCount++;

	if (m_nActCount <= PLAYER_CLIFFCLIMB_SUCCESS_COUNT)
	{ // カウント数が一定以下の場合

		// ローカル変数宣言
		D3DXVECTOR3 EffectPos;		// エフェクトの位置

		// エフェクトの位置を設定する
		EffectPos.x = rand() % ((int)(PLAYER_WIDTH) * 2) - PLAYER_WIDTH;
		EffectPos.y = (float)(rand() % (int)(AURA_RAND_SIZE));
		EffectPos.z = 0.0f;

		// オーラを生成する
		CAura::Create(player.GetPos() + EffectPos, AURA_NORMAL_LIFE);
	}

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_SPACE) == true ||
		CManager::GetInputGamePad()->GetPress(CInputGamePad::JOYKEY_A, 0) == true)
	{ // SPACEキーを押した場合

		if (m_nActCount <= PLAYER_CLIFFCLIMB_SUCCESS_COUNT)
		{ // カウント数が一定以下の場合

			// 行動状態変化処理
			ActionChange(ACT_CLIFFCLIMB);

			// ジャンプさせる
			move.y = PLAYER_CLIFF_JUMP_SUCCESS;

			// ジャンプする
			player.SetJump(true);

			// 崖上がりの速度を設定する
			m_fCliffUpSpeed = PLAYER_CLIFF_SPEED_SUCCESS;

			// 崖上がり時の重力を設定する
			m_fCliffGravity = PLAYER_CLIFF_GRAVITY_SUCCESS;

			// 崖上がり成功
			m_bCliffSuccess = true;

			// 軌跡の全頂点の位置の設定処理
			player.GetOrbit()->PosSet(player.GetPos());

			// 崖上がりモーションを設定する
			player.GetMotion()->Set(CPlayer::MOTIONTYPE_CLIFFUP);
		}
		else
		{ // 上記以外

			// 行動状態変化処理
			ActionChange(ACT_CLIFFCLIMB);

			// ジャンプさせる
			move.y = PLAYER_CLIFF_JUMP_FAILED;

			// ジャンプする
			player.SetJump(true);

			// 崖上がりの速度を設定する
			m_fCliffUpSpeed = PLAYER_CLIFF_SPEED_FAILED;

			// 崖上がり時の重力を設定する
			m_fCliffGravity = PLAYER_CLIFF_GRAVITY_FAILED;

			// 崖上がり失敗
			m_bCliffSuccess = false;

			// よじ登りモーションを設定する
			player.GetMotion()->Set(CPlayer::MOTIONTYPE_ASCENT);
		}

		// オーラを全て消す
		CObject::AnyAllClear(CObject::PRIORITY_ENTITY, CObject::TYPE_AURA);
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_S) == true ||
		CManager::GetInputGamePad()->GetGameStickLYPress(0) <= SHRT_MIN ||
		m_nActCount >= PLAYER_CLIFFCLIMB_FALL_COUNT)
	{ // Sキーを押すまたは、カウントが一定数以上になった場合

		// 行動状態変化処理
		ActionChange(ACT_CLIFFFALL);
	}

	// 移動量を更新する
	player.SetMove(move);
}

//=======================================
// 崖登り行動処理
//=======================================
void CPlayerAct::ActCliffClimb(CPlayer& player)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = player.GetPos();		// 位置を取得する
	D3DXVECTOR3 rot = player.GetRot();		// 向きを取得する

	// 移動量を加算する
	pos.x += sinf(rot.y) * m_fCliffUpSpeed;

	// 位置・向きの設定処理
	player.SetPos(pos);
	player.SetRot(rot);

	// 行動カウントを加算する
	m_nActCount++;

	if (m_bCliffSuccess == true)
	{ // 崖登りに成功していた場合

		if (m_nActCount >= PLAYER_CLIFFCLIMB_AURA_COUNT)
		{ // 行動カウントが一定数を超えた場合

			// ローカル変数宣言
			D3DXVECTOR3 EffectPos;		// エフェクトの位置

			// エフェクトの位置を設定する
			EffectPos.x = rand() % ((int)(PLAYER_WIDTH) * 2) - PLAYER_WIDTH;
			EffectPos.y = (float)(rand() % (int)(AURA_RAND_SIZE));
			EffectPos.z = 0.0f;

			// オーラを生成する
			CAura::Create(pos + EffectPos, AURA_CLIFF_LIFE);
		}
	}
}

//=======================================
// 崖降り行動処理
//=======================================
void CPlayerAct::ActCliffFall(CPlayer& player)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = player.GetRot();		// 向きを取得

	// 行動カウントを加算する
	m_nActCount++;

	if (player.GetRight() == true)
	{ // 右を向いていた場合

		// 向きを右向きにする
		rot.y = D3DX_PI * 0.5f;
	}
	else
	{ // 左を向いていた場合

		// 向きを左向きにする
		rot.y = D3DX_PI * -0.5f;
	}

	// 向きを設定する
	player.SetRot(rot);

	if (m_nActCount >= PLAYER_CLIFFFALL_COUNT)
	{ // 行動カウントが一定以上になった場合

		// 行動状態変化処理
		ActionChange(ACT_NONE);
	}
}

//=======================================
// 崖登り着地行動処理
//=======================================
void CPlayerAct::ActCliffLand(CPlayer& player)
{
	// ローカル変数宣言
	float fSpeed = player.GetSpeed();

	if (m_bCliffSuccess == true)
	{ // 崖登りに成功していた場合

		if (m_nActCount <= PLAYER_CLIFFLAND_AURA_COUNT)
		{ // 行動カウントが一定数を超えた場合

			// ローカル変数宣言
			D3DXVECTOR3 EffectPos;		// エフェクトの位置

			// エフェクトの位置を設定する
			EffectPos.x = rand() % ((int)(PLAYER_WIDTH) * 2) - PLAYER_WIDTH;
			EffectPos.y = (float)(rand() % (int)(AURA_RAND_SIZE));
			EffectPos.z = 0.0f;

			// オーラを生成する
			CAura::Create(player.GetPos() + EffectPos, AURA_CLIFF_LIFE);
		}

		// カウントを加算する
		m_nActCount++;

		if (m_nActCount >= PLAYER_CLIFFLAND_COUNT)
		{ // 行動カウントが一定数以上になった場合(失敗時)

			// 行動状態変化処理
			ActionChange(ACT_CLIFFAFTERGLOW);

			if (fSpeed <= PLAYER_MIN_SPEED)
			{ // 速度が最小値以下の場合

				// 速度を設定する
				player.SetSpeed(PLAYER_MIN_SPEED);
			}
			else
			{ // 上記以外

				// 速度を下げる
				fSpeed -= PLAYER_CLIFF_SUB_SPEED;
			}

			// スタミナの計算処理
			player.CalcStamina(-PLAYER_CLIFFLAND_STAMINA);

			// 着地UIの生成処理
			CLandUI::Create(CLandUI::TYPE_BAD);

			// オーラを全て消す
			CObject::AnyAllClear(CObject::PRIORITY_ENTITY, CObject::TYPE_AURA);

			// 着地失敗音を鳴らす
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_LANDINGMISS);

			// ブーイングを鳴らす
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_CLIFFFAILED);
		}
		else
		{ // 上記以外(成功時)

			if (CManager::GetInputKeyboard()->GetTrigger(DIK_SPACE) == true ||
				CManager::GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true)
			{ // ボタンカウントが一定数になった場合

				if (fSpeed >= PLAYER_MAX_SPEED)
				{ // 速度が最大値以上の場合

					// 速度を設定する
					fSpeed = PLAYER_MAX_SPEED;

					// スーパーブーストの音を鳴らす
					CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_SUPERDASH_START);

					// ブースト状態にする
					player.SetState(CPlayer::STATE_BOOST);
				}
				else
				{ // 上記以外

					// 速度を加算する
					fSpeed += PLAYER_CLIFF_ADD_SPEED;
				}

				// 行動状態変化処理
				ActionChange(ACT_CLIFFAFTERGLOW);

				// ボタンの情報を設定
				player.SetButtonCount(0);		// ボタンカウント
				player.SetButtonPush(false);	// ボタンの押し状況

				// 着地UIの生成処理
				CLandUI::Create(CLandUI::TYPE_GOOD);

				// オーラを全て消す
				CObject::AnyAllClear(CObject::PRIORITY_ENTITY, CObject::TYPE_AURA);

				// アクションUI(着地ターボ)を生成
				CActionUI::Create(CActionUI::TYPE_LANDTURBO, PLAYER_CLIFFLAND_SCORE);

				// ターボ音を鳴らす
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_TURBO);

				// 歓声を鳴らす
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_CLIFFSUCCESS);
			}
		}
	}
	else
	{ // 崖登りに失敗していた場合

		// 行動状態変化処理
		ActionChange(ACT_CLIFFAFTERGLOW);
	}

	// 速度を設定する
	player.SetSpeed(fSpeed);
}

//=======================================
// 余韻行動処理
//=======================================
void CPlayerAct::ActCliffAfterGlaw(CPlayer& player)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = player.GetRot();		// 向きを取得

	if (player.GetRight() == true)
	{ // 右を向いていた場合

		// 向きを右向きにする
		rot.y = D3DX_PI * 0.5f;
	}
	else
	{ // 左を向いていた場合

		// 向きを左向きにする
		rot.y = D3DX_PI * -0.5f;
	}

	// 向きを設定する
	player.SetRot(rot);

	//行動カウントを加算する
	m_nActCount++;

	if (m_nActCount >= PLAYER_CLIFFAFTERGLOW_COUNT)
	{ // 行動カウントが一定以上になった場合

		// 行動状態変化処理
		ActionChange(ACT_NONE);
	}
}

//=======================================
// ぶら下がり行動処理
//=======================================
void CPlayerAct::ActHanging(CPlayer& player)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = player.GetPos();			// 位置を取得する
	D3DXVECTOR3 move = player.GetMove();		// 移動量を取得する
	D3DXVECTOR3 rot = player.GetRot();			// 向きを取得する

	// 行動カウントを加算する
	m_nActCount++;

	if (m_fRotDest == 0.0f)
	{ // 目標の向きが 0.0f の場合

		// 向きの移動量を設定する
		rot.x = 0.0f;
	}

	//if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true)
	//{ // Dキーを押した場合

	//	// 目的の向きを更新する
	//	m_fRotDest *= 1.1f;

	//	// 向きの正規化
	//	RotNormalize(&m_fRotDest);
	//}

	//if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true)
	//{ // Aキーを押した場合

	//	// 目的の向きを更新する
	//	m_fRotDest *= 1.1f;

	//	// 向きの正規化
	//	RotNormalize(&m_fRotDest);
	//}

	if (m_fRotDest <= -D3DX_PI * 0.49f)
	{ // 向きが一定以下になった場合

		// 向きを設定する
		m_fRotDest = -D3DX_PI * 0.49f;
	}

	if (m_fRotDest >= D3DX_PI * 0.49f)
	{ // 向きが一定以上になった場合

		// 向きを設定する
		m_fRotDest = D3DX_PI * 0.49f;
	}

	if (player.GetRight() == true)
	{ // 右向きの場合

		if (m_fRotDest > 0.0f)
		{ // 目標の向きが 0.0f 超過の場合

			// 向きのホーミング処理
			RotCorrect(m_fRotDest, &rot.x, 0.05f);

			// 向きを加算する
			rot.x += 0.01f;

			if (rot.x >= m_fRotDest)
			{ // 目標の向きを超えた場合

				// 目標の向きを設定する
				m_fRotDest *= -0.9f;

				// 街灯の軋む音を鳴らす
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_LAMPCREAK);
			}
		}
		else if (m_fRotDest < 0.0f)
		{ // 目標の向きが 0.0f 未満の場合

			// 向きのホーミング処理
			RotCorrect(m_fRotDest, &rot.x, 0.05f);

			// 向きを減算する
			rot.x -= 0.01f;

			if (rot.x <= m_fRotDest)
			{ // 目標の向きを超えた場合

				// 目標の向きを設定する
				m_fRotDest *= -0.9f;

				// 街灯の軋む音を鳴らす
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_LAMPCREAK);
			}
		}

		// 位置を設定する
		pos.x = m_posObject.x + sinf(rot.x) * PLAYER_HEIGHT;
		pos.y = m_posObject.y - cosf(rot.x) * PLAYER_HEIGHT;
	}
	else
	{ // 左向きの場合

		if (-m_fRotDest > 0.0f)
		{ // 目標の向きが 0.0f 超過の場合

			// 向きのホーミング処理
			RotCorrect(-m_fRotDest, &rot.x, 0.05f);

			// 向きを加算する
			rot.x += 0.01f;

			if (rot.x >= -m_fRotDest)
			{ // 目標の向きを超えた場合

				// 目標の向きを設定する
				m_fRotDest *= -0.9f;

				// 街灯の軋む音を鳴らす
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_LAMPCREAK);
			}
		}
		else if (-m_fRotDest < 0.0f)
		{ // 目標の向きが 0.0f 未満の場合

			// 向きのホーミング処理
			RotCorrect(-m_fRotDest, &rot.x, 0.05f);

			// 向きを減算する
			rot.x -= 0.01f;

			if (rot.x <= -m_fRotDest)
			{ // 目標の向きを超えた場合

				// 目標の向きを設定する
				m_fRotDest *= -0.9f;

				// 街灯の軋む音を鳴らす
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_LAMPCREAK);
			}
		}

		// 位置を設定する
		pos.x = m_posObject.x + sinf(-rot.x) * PLAYER_HEIGHT;
		pos.y = m_posObject.y - cosf(-rot.x) * PLAYER_HEIGHT;
	}

	// プレイヤーのスイング処理
	PlayerSwing(player);

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_SPACE) == true || 
		CManager::GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true)
	{ // SPACEキーを押した場合

		// 軌跡の全頂点の位置の設定
		player.GetOrbit()->PosSet(pos);

		if (m_fRotDest >= 0.0f)
		{ // 目標の向きが 0.0f 以上の場合
			
			if (m_nActCount <= PLAYER_FAST_LAMPJUMP_COUNT)
			{ // 最速街灯ジャンプの場合

				// 移動量を設定する
				move.x = PLAYER_FAST_LAMPJUMP_MOVE_X;
			}
			else
			{ // 上記以外

				// 移動量を設定する
				move.x = (m_fRotDest - fabsf(fabsf(m_fRotDest) - fabsf(rot.x))) * PLAYER_LAMPJUMP_MOVE_X_MAGNI;
			}

			if (move.x >= PLAYER_LAMPJUMP_SCORE_MOVE)
			{ // 移動量が一定以上の場合

				// アクションUIを生成する
				CActionUI::Create(CActionUI::TYPE_LAMPJUMP, PLAYER_LAMPJUMP_SCORE);
			}

			// 右向き状況の設定処理
			player.SetRight(true);
		}
		else
		{ // 上記以外

			if (m_nActCount <= PLAYER_FAST_LAMPJUMP_COUNT)
			{ // 最速街灯ジャンプの場合

				// 移動量を設定する
				move.x = -PLAYER_FAST_LAMPJUMP_MOVE_X;
			}
			else
			{ // 上記以外

				// 移動量を設定する
				move.x = (m_fRotDest + fabsf(fabsf(m_fRotDest) - fabsf(rot.x))) * PLAYER_LAMPJUMP_MOVE_X_MAGNI;
			}

			if (move.x <= -PLAYER_LAMPJUMP_SCORE_MOVE)
			{ // 移動量が一定以下の場合

				// アクションUIを生成する
				CActionUI::Create(CActionUI::TYPE_LAMPJUMP, PLAYER_LAMPJUMP_SCORE);
			}

			// 右向き状況の設定処理
			player.SetRight(false);
		}

		// 行動変更処理
		ActionChange(ACT_LAMPJUMP);

		if (m_nActCount <= PLAYER_FAST_LAMPJUMP_COUNT)
		{ // 最速街灯ジャンプの場合

			// 移動量を設定する
			move.y = PLAYER_LAMPJUMP_MIN_MOVE_Y;
		}
		else
		{ // 上記以外

			// 縦の移動量を設定する
			move.y = fabsf(m_fRotDest) + PLAYER_LAMPJUMP_MIN_MOVE_Y;
		}

		// 向きを 0.0f に直す
		rot.x = 0.0f;

		// 街灯の情報設定処理
		player.SetStreetLamp(nullptr);

		// ランプジャンプモーションを設定する
		player.GetMotion()->Set(CPlayer::MOTIONTYPE_LAMPJUMP);

		// 街灯ジャンプを鳴らす
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_LAMPJUMP);

		// 速度を上げる
		player.SetSpeed(player.GetSpeed() + PLAYER_LAMPJUMP_ADD_SPEED);
	}

	CManager::GetDebugProc()->Print("目的の向き：%f", m_fRotDest);

	// 情報を設定する
	player.SetPos(pos);		// 位置を更新する
	player.SetMove(move);	// 移動量を更新する
	player.SetRot(rot);		// 向きを更新する
}

//=======================================
// 街灯から飛んでいる状態
//=======================================
void CPlayerAct::ActLampJump(CPlayer& player)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = player.GetRot();		// 向きを取得

	if (player.GetRight() == true)
	{ // 右を向いていた場合

		// 向きを右向きにする
		rot.y = D3DX_PI * 0.5f;
	}
	else
	{ // 左を向いていた場合

		// 向きを左向きにする
		rot.y = D3DX_PI * -0.5f;
	}

	// 向きを設定する
	player.SetRot(rot);

	//行動カウントを加算する
	m_nActCount++;

	//if (m_nActCount >= PLAYER_POLEJUMP_COUNT)
	//{ // 行動カウントが一定以上になった場合

	//	// 行動状態変化処理
	//	ActionChange(ACT_NONE);
	//}
}

//=======================================
// ポール下り状態処理
//=======================================
void CPlayerAct::ActPoleDown(CPlayer& player)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = player.GetPos();		// 位置を取得する
	D3DXVECTOR3 rot = player.GetRot();		// 向きを取得する
	float fNowRot;							// 現在の支柱に対する方向

	// 向きを算出する
	fNowRot = atan2f((pos.x - m_posObject.x), (pos.z - m_posObject.z));

	// 向きを加算する
	fNowRot += PLAYER_POLEDOWN_ROT_MOVE;

	// 向きを設定する
	rot.y = fNowRot + (D3DX_PI * 0.5f);

	// プレイヤーを下げていく
	pos.y -= PLAYER_POLEDOWN_POSY_MOVE;

	// 向きの正規化
	RotNormalize(&rot.y);

	// 位置を設定する
	pos.x = m_posObject.x + sinf(fNowRot) * POLE_PLAYER_DIST;
	pos.z = m_posObject.z + cosf(fNowRot) * POLE_PLAYER_DIST;

	// 情報を更新する
	player.SetPos(pos);		// 位置
	player.SetRot(rot);		// 向き
}

//=============================================================================それぞれの細かい処理=============================================================================

//=======================================
// 超ジャンプ中のエフェクト生成処理
//=======================================
void CPlayerAct::SuperJumpEffect(CPlayer& player)
{
	// ローカル変数宣言
	float fAngle, fSpeed;				// 角度と速度
	D3DXVECTOR3 move;					// 移動量
	D3DXVECTOR3 pos = player.GetPos();	// 位置

	// 高さを設定する
	pos.y += PLAYER_HEIGHT + PLAYER_BIGJUMP_ADD_POS_Y;

	// 向きを設定する
	fAngle = (float)(rand() % 629 - 314) / 100.0f;

	//移動量を設定する
	fSpeed = (float)(rand() % 500) / 100.0f + 10.0f;

	//移動量を設定する
	move.x = player.GetMove().x + sinf(fAngle) * fSpeed;
	move.y = player.GetMove().y - fSpeed;
	move.z = player.GetMove().z + cosf(fAngle) * fSpeed;

	// エフェクトを生成する
	CEffect::Create(pos, move, 15, 20.0f, CEffect::TYPE_BIGJUMP);

	//移動量を設定する
	move.x = player.GetMove().x + sinf(-fAngle) * fSpeed;
	move.y = player.GetMove().y - fSpeed;
	move.z = player.GetMove().z + cosf(-fAngle) * fSpeed;

	// エフェクトを生成する
	CEffect::Create(pos, move, 15, 20.0f, CEffect::TYPE_BIGJUMP);
}

//=======================================
// プレイヤーのスイング処理
//=======================================
void CPlayerAct::PlayerSwing(CPlayer& player)
{
	// 向きを取得する
	D3DXVECTOR3 rot = player.GetHierarchy(0)->GetRot();

	// 向きを加算する
	rot.x = player.GetRot().x * 0.3f;

	// 向きを更新する
	player.GetHierarchy(0)->SetRot(rot);

	// 向きを取得する
	rot = player.GetHierarchy(1)->GetRot();

	// 向きを加算する
	rot.x = player.GetRot().x * -0.3f;

	// 向きを更新する
	player.GetHierarchy(1)->SetRot(rot);

	for (int nCnt = 8; nCnt < 12; nCnt++)
	{
		// 向きを取得する
		rot = player.GetHierarchy(nCnt)->GetRot();

		// 向きを加算する
		rot.x = player.GetRot().x * 0.65f;

		if (rot.x >= 0.02f)
		{ // 向きが一定以上に達した場合

			// 向きを補正する
			rot.x = 0.02f;
		}

		// 向きを更新する
		player.GetHierarchy(nCnt)->SetRot(rot);
	}
}

//=======================================
// 壁キック(右版)
//=======================================
void CPlayerAct::WallKickRight(CPlayer& player)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = player.GetRot();		// 向き
	D3DXVECTOR3 move = player.GetMove();	// 移動量

	// 移動量を設定する
	move.x = PLAYER_WALLKICK_SPEED_R.x + player.GetSpeed() * 0.5f;
	move.y = PLAYER_WALLKICK_SPEED_R.y + player.GetSpeed() * 0.1f;

	// 着地の波紋を生成する
	CRipple::Create(player.GetPos(), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * -0.25f));

	// 右を向く
	player.SetRight(true);

	// 向きを右向きにする
	rot.y = D3DX_PI * 0.5f;

	// 壁キックの共通処理
	WallKickProcess(player);

	// 情報の設定処理
	player.SetRot(rot);			// 向きの設定処理
	player.SetMove(move);		// 移動量の設定処理
}

//=======================================
// 壁キック(左版)
//=======================================
void CPlayerAct::WallKickLeft(CPlayer& player)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = player.GetRot();		// 向き
	D3DXVECTOR3 move = player.GetMove();	// 移動量

	// 移動量を設定する
	move.x = PLAYER_WALLKICK_SPEED_L.x - player.GetSpeed() * 0.5f;
	move.y = PLAYER_WALLKICK_SPEED_L.y + player.GetSpeed() * 0.1f;

	// 着地の波紋を生成する
	CRipple::Create(player.GetPos(), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.25f));

	// 左を向く
	player.SetRight(false);

	// 向きを左向きにする
	rot.y = D3DX_PI * -0.5f;

	// 壁キックの共通処理
	WallKickProcess(player);

	// 情報の設定処理
	player.SetRot(rot);			// 向きの設定処理
	player.SetMove(move);		// 移動量の設定処理
}

//=======================================
// 壁キック共通の処理
//=======================================
void CPlayerAct::WallKickProcess(CPlayer& player)
{
	// 行動状態変化処理
	ActionChange(CPlayerAct::ACT_WALLKICK);

	// 軌跡の全頂点の位置の設定
	player.GetOrbit()->PosSet(player.GetPos());

	// 壁踏み込みモーションの設定処理
	player.GetMotion()->Set(CPlayer::MOTIONTYPE_WALLSTEPIN);

	// スタミナの計算処理
	player.CalcStamina(-PLAYER_WALLKICK_STAMINA);

	if (CManager::GetCamera() != nullptr &&
		CManager::GetCamera()->GetType() != CCamera::TYPE_BOOST)
	{ // カメラの情報があるかつ、ブースト状態以外の場合

		// 壁キック時のカメラの設定処理
		CManager::GetCamera()->SetType(CCamera::TYPE_WALLKICK);
	}

	// アクションUI(壁キック)を生成
	CActionUI::Create(CActionUI::TYPE_WALLKICK, PLAYER_WALLKICK_SCORE);

	// 破片の設定処理
	CFraction::SetWallKick(player.GetPos(), -1, WALLKICK_FRACTION_COL);

	// 壁擦りカウントを初期化する
	player.SetWallFricCount(0);

	// 壁キック音を鳴らす
	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_WALLKICK);

	// 速度を上げる
	player.SetSpeed(player.GetSpeed() + PLAYER_WALLKICK_ADD_SPEED);
}