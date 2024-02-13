//===========================================
//
// マシンドリーのメイン処理[machidori.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "machidori.h"
#include "useful.h"

#include "objectElevation.h"
#include "elevation_manager.h"
#include "player.h"
#include "fraction.h"
#include "destruction.h"
#include "Particle.h"
#include "collision.h"
#include "stun.h"
#include "ripple.h"
#include "sound.h"

//-------------------------------------------
// マクロ定義
//-------------------------------------------
#define COLLISION_SIZE			(D3DXVECTOR3(40.0f, 93.0f, 44.0f))		// 当たり判定のサイズ
#define MOVE_DISTANCE			(1400.0f)		// 移動する距離
#define ROT_MOVE				(0.05f)			// 向きの移動量
#define ADD_HEIGHT				(300.0f)		// 追加する高さ
#define MOVE_HEIGHT				(50.0f)			// Y軸の移動量
#define MOVE_WIDTH				(2.0f)			// X軸の移動量
#define STANDBY_CORRECT			(0.08f)			// スタンバイ状態の補正
#define STANDBY_ROT_DEST		(0.4f)			// スタンバイ状態の目的の向き
#define STANDBY_COUNT			(60)			// スタンバイ状態のカウント
#define ATTACK_CORRECT			(0.6f)			// スタンバイ状態の補正
#define ATTACK_ROT_DEST			(0.0f)			// スタンバイ状態の目的の向き
#define ATTACK_DOWN				(20.0f)			// 攻撃状態の降下量
#define ATTACK_COUNT			(80)			// 攻撃状態のカウント
#define UP_HEIGHT				(10.0f)			// 攻撃状態の高さ
#define FRACTION_COUNT			(6)				// 破片の数
#define DEATH_VIB_COUNT			(4)				// 死亡時の振動するカウント
#define DEATH_COUNT				(60)			// 死亡するまでのカウント
#define DEATH_DSTR_SIZE			(D3DXVECTOR3(40.0f,40.0f,0.0f))			// 死亡時の撃破のサイズ
#define DEATH_DSTR_COL			(D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f))		// 死亡時の撃破の色
#define DEATH_DSTR_LIFE			(20)			// 死亡時の撃破の寿命
#define SMASH_ROTMOVE_Z			(0.02f)			// 吹き飛び時の向きの移動量
#define SMASH_DEATH_COUNT		(70)			// 吹き飛んでから死ぬまでのカウント
#define HIT_DSTR_SIZE			(D3DXVECTOR3(40.0f,40.0f,0.0f))			// ヒット時の撃破のサイズ
#define HIT_DSTR_COL			(D3DXCOLOR(1.0f, 0.7f, 0.1f, 1.0f))		// ヒット時の撃破の色
#define HIT_DSTR_LIFE			(10)			// ヒット時の撃破の寿命
#define STUN_COUNT				(150)			// 気絶時のカウント数
#define STUN_DOWN				(15.0f)			// 気絶時の減少量
#define STUN_SOUND_COUNT		(27)			// 気絶音がなるカウント数

//==============================
// コンストラクタ
//==============================
CMachidori::CMachidori() : CEnemy(CObject::TYPE_ENEMY, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_state = STATE_FLY;	// 状態
	m_nStateCount = 0;		// 状態カウント
	m_fCurveRot = 0.0f;		// カーブ用向き
	m_fHeight = ADD_HEIGHT;	// 追加の高さ
	m_bRight = true;		// 前回の右向き状況
}

//==============================
// デストラクタ
//==============================
CMachidori::~CMachidori()
{

}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CMachidori::Init(void)
{
	if (FAILED(CEnemy::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_state = STATE_FLY;	// 状態
	m_nStateCount = 0;		// 状態カウント
	m_fCurveRot = 0.0f;		// カーブ用向き
	m_fHeight = ADD_HEIGHT;	// 追加の高さ
	m_bRight = true;		// 前回の右向き状況

	// 値を返す
	return S_OK;
}

//========================================
//ブロックの終了処理
//========================================
void CMachidori::Uninit(void)
{
	// 終了処理
	CEnemy::Uninit();
}

//========================================
//ブロックの更新処理
//========================================
void CMachidori::Update(void)
{
	// 前回の位置を設定する
	SetPosOld(GetPos());

	if (IsStun() == true)
	{ // 気絶状況が true の場合

		// ローカル変数宣言
		int nStunCount = GetStunCount();		// 気絶カウントを取得する

		if (nStunCount % STUN_SOUND_COUNT == 0)
		{ // 気絶カウントが一定数に達した場合

			// 気絶音を流す
			CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_STUN);
		}

		// 気絶カウントを加算する
		nStunCount++;

		if (GetStun() != nullptr)
		{ // 気絶演出が NULL じゃない場合

			// ローカル変数宣言
			D3DXVECTOR3 pos = GetPos();		// 位置

			// 気絶演出の位置の設定処理
			GetStun()->SetPos(D3DXVECTOR3(pos.x, pos.y + GetCollSize().y, pos.z));
		}

		// 高さの設定処理
		Height();

		// 起伏地面との当たり判定
		if (ElevationCollision() == false &&
			TableCollision() == false)
		{
			// 高さを下げる
			m_fHeight -= STUN_DOWN;
		}

		if (nStunCount % STUN_COUNT == 0)
		{ // 状態カウントが一定数になった場合

			// 気絶カウントを初期化する
			nStunCount = 0;

			// 気絶状況を false にする
			SetEnableStun(false);

			// 状態カウントを初期化する
			m_nStateCount = 0;

			// 上昇状態に設定する
			m_state = STATE_UP;

			// 気絶演出の消去処理
			DeleteStun();
		}

		// 気絶カウントを適用する
		SetStunCount(nStunCount);
	}
	else
	{ // 上記以外

		switch (m_state)
		{
		case STATE_FLY:

			// 起伏地面の当たり判定
			Elevation();

			// カーブ用の向きを加算する
			m_fCurveRot += ROT_MOVE;

			// 向きの正規化
			useful::RotNormalize(&m_fCurveRot);

			// 高さの設定処理
			Height();

			// プレイヤーの判定処理
			CheckPlayer();

			// 移動処理
			Move();

			break;

		case STATE_STANDBY:

			// スタンバイ状態
			StandBy();

			// 状態カウントを加算する
			m_nStateCount++;

			if (m_nStateCount % STANDBY_COUNT == 0)
			{ // 状態カウントが一定以上になった場合

				// 状態カウントを0にする
				m_nStateCount = 0;

				// 攻撃状態にする
				m_state = STATE_ATTACK;
			}

			break;

		case STATE_ATTACK:

			// 攻撃状態での向きの設定処理
			AttackRot();

			// 高さの設定処理
			Height();

			// 起伏地面との当たり判定
			if (ElevationCollision() == true ||
				BlockCollision() == true ||
				TableCollision() == true)
			{
				if (m_nStateCount == 0)
				{ // 初めて地面にぶつかったとき

					// 波紋の生成
					CRipple::Create(GetPos(), NONE_D3DXVECTOR3);
				}

				// 状態カウントを加算する
				m_nStateCount++;
			}
			else
			{ // 上記以外

				// 高さを下げる
				m_fHeight -= ATTACK_DOWN;
			}

			if (m_nStateCount >= ATTACK_COUNT)
			{ // 状態カウントが一定以上の場合

				// 前回の左右状況の設定処理
				SetRightOld();

				// 状態カウントを初期化する
				m_nStateCount = 0;

				// 上昇状態にする
				m_state = STATE_UP;
			}

			break;

		case STATE_UP:

			// 上昇状態の上昇処理
			UpAscent();

			break;

		case STATE_DEATH:

			// 状態カウントを加算する
			m_nStateCount++;

			// 死亡時の振動処理
			DeathVib();

			// 重力処理
			Gravity();

			// 起伏地面との当たり判定
			ElevationCollision();

			// ブロックとの当たり判定
			BlockCollision();

			// 台との当たり判定
			TableCollision();

			// 煙の生成処理
			Smoke();

			if (m_nStateCount >= DEATH_COUNT)
			{ // 状態カウントが一定数になった場合

				// 撃破の生成処理
				CDestruction::Create(GetPos(), DEATH_DSTR_SIZE, DEATH_DSTR_COL, CDestruction::TYPE_THORN, DEATH_DSTR_LIFE);

				// パーティクルの生成処理
				CParticle::Create(GetPos(), CParticle::TYPE_ENEMYDEATH);

				// ローカル変数宣言
				CFraction::TYPE type = CFraction::TYPE_SCREW;

				for (int nCnt = 0; nCnt < FRACTION_COUNT; nCnt++)
				{
					// 種類を設定する
					type = (CFraction::TYPE)(rand() % CFraction::TYPE_RING);

					// 破片の生成処理
					CFraction::Create(GetPos(), type);
				}

				// 敵の破壊音を鳴らす
				CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_ENEMYBREAK);

				// 終了処理
				Uninit();

				// この先の処理を行わない
				return;
			}

			break;

		case STATE_SMASH:

			// 状態カウントを加算する
			m_nStateCount++;

			// 重力処理
			Gravity();

			// 吹き飛び処理
			Smash();

			if (m_nStateCount >= SMASH_DEATH_COUNT)
			{ // 状態カウントが一定数になった場合

				// 終了処理
				Uninit();

				// この先の処理を行わない
				return;
			}

			break;

		default:

			// 停止
			assert(false);

			break;
		}
	}

	// 敵同士の当たり判定
	//collision::EnemyToEnemy(this);
}

//=====================================
//ブロックの描画処理
//=====================================
void CMachidori::Draw(void)
{
	// 描画処理
	CEnemy::Draw();
}

//=====================================
// ヒット処理
//=====================================
void CMachidori::Hit(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 posDstr = GetPos();		// 撃破を出す位置
	D3DXVECTOR3 move = GetMove();		// 移動量

	// ヒット処理
	CEnemy::Hit();

	// 当たり判定状況を OFF にする
	SetEnableCollision(false);

	// 状態カウントを初期化する
	m_nStateCount = 0;

	// 死亡状態に設定する
	m_state = STATE_DEATH;

	// 移動量を設定する
	move.y = 0.0f;

	// 移動量を適用する
	SetMove(move);

	// エフェクトを出す位置を設定する
	posDstr.y += GetCollSize().y;

	// 撃破の生成処理
	CDestruction::Create(posDstr, HIT_DSTR_SIZE, HIT_DSTR_COL, CDestruction::TYPE_AIRY, HIT_DSTR_LIFE);
}

//=====================================
// 吹き飛びヒット処理
//=====================================
void CMachidori::SmashHit(void)
{
	// 吹き飛びヒット処理
	CEnemy::SmashHit();

	// 当たり判定状況を OFF にする
	SetEnableCollision(false);

	// 状態カウントを初期化する
	m_nStateCount = 0;

	// 吹き飛び状態に設定する
	m_state = STATE_SMASH;

	// 撃破の生成処理
	CDestruction::Create(GetPos(), DEATH_DSTR_SIZE, DEATH_DSTR_COL, CDestruction::TYPE_THORN, DEATH_DSTR_LIFE);

	// パーティクルの生成処理
	CParticle::Create(GetPos(), CParticle::TYPE_ENEMYDEATH);

	// ローカル変数宣言
	CFraction::TYPE type = CFraction::TYPE_SCREW;

	for (int nCnt = 0; nCnt < FRACTION_COUNT; nCnt++)
	{
		// 種類を設定する
		type = (CFraction::TYPE)(rand() % CFraction::TYPE_RING);

		// 破片の生成処理
		CFraction::Create(GetPos(), type);
	}
}

//=====================================
// 気絶のヒット処理
//=====================================
void CMachidori::StunHit(void)
{
	if (IsStun() == false)
	{ // 気絶状況が false の場合

		// 敵の気絶処理
		CEnemy::StunHit();

		// 状態カウントを初期化する
		m_nStateCount = 0;
	}
}

//=====================================
// 情報の設定処理
//=====================================
void CMachidori::SetData(const D3DXVECTOR3& pos, const TYPE type)
{
	// 情報の設定処理
	CEnemy::SetData(pos, type);

	// モデルの情報を設定する
	SetFileData(CXFile::TYPE_MACHIDORI);

	// 全ての値を設定する
	m_state = STATE_FLY;	// 状態
	m_nStateCount = 0;		// 状態カウント
	m_fCurveRot = 0.0f;		// カーブ用向き
	m_fHeight = ADD_HEIGHT;	// 追加の高さ

	// 前回の左右状況の設定処理
	SetRightOld();

	// 情報の設定処理
	SetEnableStep(true);			// 踏みつけられる設定
	SetCollSize(COLLISION_SIZE);	// 当たり判定のサイズ
}

//=====================================
// プレイヤーの判断処理
//=====================================
void CMachidori::CheckPlayer(void)
{
	// ローカルポインタ宣言
	CPlayer* pPlayer = CPlayer::Get();				// プレイヤーの情報を取得する
	D3DXVECTOR3 pos = GetPos();						// 位置
	D3DXVECTOR3 move = GetMove();					// 移動量
	D3DXVECTOR3 vtxMax = GetFileData().vtxMax;		// 最大値
	D3DXVECTOR3 vtxMin = GetFileData().vtxMin;		// 最小値

	if (pPlayer != nullptr)
	{ // プレイヤーの情報がある場合

		if (pPlayer->GetPos().z <= pos.z + vtxMax.z &&
			pPlayer->GetPos().z >= pos.z + vtxMin.z &&
			fabsf(pos.x - pPlayer->GetPos().x) <= MOVE_DISTANCE)
		{ // 一定距離内の場合

			if (CPlayer::Get() != nullptr)
			{ // プレイヤーの情報がある場合

				if (pPlayer->GetPos().x >= pos.x)
				{ // プレイヤーの位置が自身よりも右にいる場合

					// 向きを設定する
					move.x = MOVE_WIDTH;

					if (m_bRight == false)
					{ // 前回が左向きの場合

						// スタンバイ状態にする
						m_state = STATE_STANDBY;
					}
				}
				else
				{ // 上記以外

					// 向きを設定する
					move.x = -MOVE_WIDTH;

					if (m_bRight == true)
					{ // 前回が右向きの場合

						// スタンバイ状態にする
						m_state = STATE_STANDBY;
					}
				}
			}
		}
		else
		{ // 上記以外

			// 移動量を無くす
			move.x = 0.0f;
		}
	}
	else
	{ // 上記以外

		// 移動量を無くす
		move.x = 0.0f;
	}

	// 移動量を適用する
	SetMove(move);
}

//=====================================
// 移動処理
//=====================================
void CMachidori::Move(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();			// 位置を取得する
	D3DXVECTOR3 move = GetMove();		// 移動量を取得する

	// 位置を設定する
	pos.x += move.x;

	// 位置を適用する
	SetPos(pos);
}

//=====================================
// 高さ設定処理
//=====================================
void CMachidori::Height(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();				// 位置を取得する

	// 位置を設定する
	pos.y = m_fElevPos + sinf(m_fCurveRot) * MOVE_HEIGHT + m_fHeight;

	// 位置を適用する
	SetPos(pos);
}

//=====================================
// 起伏地面の当たり判定
//=====================================
void CMachidori::Elevation(void)
{
	// ローカル変数宣言
	CElevation* pMesh = CElevationManager::Get()->GetTop();		// 起伏の先頭のオブジェクトを取得する
	D3DXVECTOR3 pos = GetPos();				// 位置を取得する
	float fHeight = 0.0f;					// 高さ

	while (pMesh != nullptr)
	{ // 地面の情報がある限り回す

		// 当たり判定を取る
		fHeight = pMesh->ElevationCollision(pos);

		// 高さを設定する
		m_fElevPos = fHeight;

		// 位置を設定する(ここで一時的に決めるだけ)
		pos.y = m_fElevPos;

		// 次のポインタを取得する
		pMesh = pMesh->GetNext();
	}
}

//=====================================
// 前回の左右状況の設定処理
//=====================================
void CMachidori::SetRightOld(void)
{
	if (CPlayer::Get() != nullptr)
	{ // プレイヤーの情報がある場合

		// ローカル変数宣言
		D3DXVECTOR3 posPlayer = CPlayer::Get()->GetPos();	// プレイヤーの位置
		D3DXVECTOR3 pos = GetPos();		// 位置を取得する

		if (posPlayer.x >= pos.x)
		{ // プレイヤーの位置が自身よりも右にいる場合

			// 右に設定する
			m_bRight = true;
		}
		else
		{ // 上記以外

			// 左に設定する
			m_bRight = false;
		}
	}
}

//=====================================
// スタンバイ処理
//=====================================
void CMachidori::StandBy(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = GetRot();		// 向きを取得する

	// 向きの補正処理
	useful::RotCorrect(STANDBY_ROT_DEST, &rot.x, STANDBY_CORRECT);

	// 向きを適用する
	SetRot(rot);
}

//=====================================
// 攻撃状態での向きの設定処理
//=====================================
void CMachidori::AttackRot(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = GetRot();		// 向きを取得する

	// 向きの補正処理
	useful::RotCorrect(ATTACK_ROT_DEST, &rot.x, ATTACK_CORRECT);

	// 向きを適用する
	SetRot(rot);
}

//=====================================
// 上昇状態の上昇処理
//=====================================
void CMachidori::UpAscent(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();				// 位置を取得する

	// 追加の高さを設定する
	m_fHeight += UP_HEIGHT;

	if (m_fHeight >= ADD_HEIGHT)
	{ // 高さが一定数以上になった場合

		// 高さを設定する
		m_fHeight = ADD_HEIGHT;

		// 浮遊状態にする
		m_state = STATE_FLY;
	}

	// 位置を設定する
	pos.y = m_fElevPos + sinf(m_fCurveRot) * MOVE_HEIGHT + m_fHeight;

	// 位置を適用する
	SetPos(pos);
}

//=====================================
// 死亡時の振動処理
//=====================================
void CMachidori::DeathVib(void)
{
	if (m_nStateCount % DEATH_VIB_COUNT == 0)
	{
		// ローカル変数宣言
		D3DXVECTOR3 rot = GetRot();		// 向き

		// 向きを設定する
		rot.x = (rand() % 51 - 25) * 0.01f;
		rot.z = (rand() % 51 - 25) * 0.01f;

		// 向きを適用する
		SetRot(rot);
	}
}

//=====================================
// 煙の生成処理
//=====================================
void CMachidori::Smoke(void)
{
	// ローカル変数宣言
	float fShift;		// ずらす幅
	D3DXVECTOR3 move;	// 移動量

	// ずらす幅を設定する
	fShift = (float)(rand() % 30);

	// 移動量を設定する
	move.x = (float)((rand() % 21 - 10) * 0.01f);
	move.y = (float)(rand() % 5 + 3);
	move.z = (float)((rand() % 21 - 10) * 0.01f);

	// 煙の生成
	CEffect::Create(D3DXVECTOR3(GetPos().x + fShift, GetPos().y, GetPos().z), move, 80, 50.0f, CEffect::TYPE_SMOKE, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), false);
}

//=====================================
// 吹き飛び状態の処理
//=====================================
void CMachidori::Smash(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置
	D3DXVECTOR3 rot = GetRot();		// 向き
	D3DXVECTOR3 move = GetMove();	// 移動量

	// 向きを設定する
	rot.z += SMASH_ROTMOVE_Z;

	// 向きの正規化処理
	useful::RotNormalize(&rot.z);

	// 位置を更新する
	pos.x += move.x;
	pos.z += move.z;

	// 情報を適用する
	SetPos(pos);		// 位置
	SetRot(rot);		// 向き
}