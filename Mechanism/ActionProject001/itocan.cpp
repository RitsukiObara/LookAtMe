//===========================================
//
// イトキャンのメイン処理[itocan.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "itocan.h"
#include "useful.h"

#include "player.h"
#include "destruction.h"
#include "Particle.h"
#include "fraction.h"
#include "collision.h"
#include "stun.h"
#include "sound.h"

//-------------------------------------------
// マクロ定義
//-------------------------------------------
#define COLLISION_SIZE		(D3DXVECTOR3(32.0f, 128.0f, 40.0f))	// 当たり判定のサイズ
#define MOVE_DISTANCE		(1300.0f)							// 移動する距離
#define MOVE				(D3DXVECTOR3(3.0f, 0.0f, 0.0f))		// 移動量
#define ROT_MOVE_MAGNI		(0.15f)								// 向きの移動量の倍率
#define CHECK_COUNT			(15)								// チェックするカウント数
#define DEATH_SCALE			(0.2f)								// 死亡状態の拡大率
#define DEATH_ADD_SCALE		(0.15f)								// 死亡状態の加算する拡大率
#define DEATH_COUNT			(40)								// 死亡するまでのカウント
#define DEATH_DSTR_SIZE		(D3DXVECTOR3(40.0f,40.0f,0.0f))		// 死亡時の撃破のサイズ
#define DEATH_DSTR_COL		(D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f))	// 死亡時の撃破の色
#define DEATH_DSTR_LIFE		(20)								// 死亡時の撃破の寿命
#define FRACTION_COUNT		(8)									// 破片の数
#define SMASH_ROTMOVE_X		(0.3f)								// 吹き飛ぶ向きの移動量(X軸)
#define SMASH_ROTMOVE_Z		(0.2f)								// 吹き飛ぶ向きの移動量(Z軸)
#define SMASH_DEATH_COUNT	(70)								// 吹き飛んで死亡するまでのカウント
#define HIT_DSTR_SIZE		(D3DXVECTOR3(40.0f,40.0f,0.0f))		// ヒット時の撃破のサイズ
#define HIT_DSTR_COL		(D3DXCOLOR(1.0f, 0.7f, 0.1f, 1.0f))	// ヒット時の撃破の色
#define HIT_DSTR_LIFE		(10)								// ヒット時の撃破の寿命
#define STUN_COUNT			(150)								// 気絶演出時のカウント数
#define STUN_SOUND_COUNT	(27)								// 気絶音がなるカウント数

//==============================
// コンストラクタ
//==============================
CItocan::CItocan() : CEnemy(CObject::TYPE_ENEMY, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_state = STATE_STOP;		// 状態
	m_nStateCount = 0;			// 状態カウント
	m_fRotDest = 0.0f;			// 目標の向き
}

//==============================
// デストラクタ
//==============================
CItocan::~CItocan()
{

}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CItocan::Init(void)
{
	if (FAILED(CEnemy::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値をクリアする
	m_state = STATE_STOP;		// 状態
	m_nStateCount = 0;			// 状態カウント
	m_fRotDest = 0.0f;			// 目標の向き

	// 値を返す
	return S_OK;
}

//========================================
//ブロックの終了処理
//========================================
void CItocan::Uninit(void)
{
	// 終了処理
	CEnemy::Uninit();
}

//========================================
//ブロックの更新処理
//========================================
void CItocan::Update(void)
{
	// 前回の位置を設定する
	SetPosOld(GetPos());

	if (IsStun() == true)
	{ // 気絶状況が true の場合

		// ローカル変数宣言
		int nStunCount = GetStunCount();	// 気絶カウントを取得する

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

		if (nStunCount >= STUN_COUNT)
		{ // 状態カウントが一定数以上になった場合

			// 気絶カウントを初期化する
			nStunCount = 0;

			// 気絶状況を false にする
			SetEnableStun(false);

			// 状態カウントを初期化する
			m_nStateCount = 0;

			// 停止状態にする
			m_state = STATE_STOP;

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
		case STATE_STOP:		// 停止状態

			if (m_nStateCount % CHECK_COUNT == 0)
			{ // 一定カウント数になった場合

				// プレイヤーの判定処理
				CheckPlayer();
			}

			// 状態の判定処理
			CheckState();

			break;

		case STATE_MOVE:		// 移動状態

			if (m_nStateCount % CHECK_COUNT == 0)
			{ // 一定カウント数になった場合

				// プレイヤーの判定処理
				CheckPlayer();
			}

			// 状態の判定処理
			CheckState();

			// 向きの移動処理
			RotMove();

			// 移動処理
			Move();

			break;

		case STATE_DEATH:		// 死亡状態

			// 死亡時の拡大率処理
			DeathScaling();

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

		case STATE_SMASH:		// 吹き飛び状態

			// 吹き飛び状態処理
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

	// 重力処理
	Gravity();

	// 起伏地面との当たり判定
	ElevationCollision();

	// 台・ブロックとの当たり判定
	if (BlockCollision() == true || 
		TableCollision() == true)
	{ // 着地判定が true の場合

		// 台の着地判定処理
		TableLand();
	}

	// 敵同士の当たり判定
	//collision::EnemyToEnemy(this);

	// 状態カウントを加算する
	m_nStateCount++;
}

//=====================================
//ブロックの描画処理
//=====================================
void CItocan::Draw(void)
{
	// 描画処理
	CEnemy::Draw();
}

//=====================================
// ヒット処理
//=====================================
void CItocan::Hit(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 posDstr = GetPos();		// 撃破を出す位置

	// ヒット処理
	CEnemy::Hit();

	// 当たり判定状況を OFF にする
	SetEnableCollision(false);

	// 状態カウントを初期化する
	m_nStateCount = 0;

	// 死亡状態に設定する
	m_state = STATE_DEATH;

	// エフェクトを出す位置を設定する
	posDstr.y += GetCollSize().y;

	// 撃破の生成処理
	CDestruction::Create(posDstr, HIT_DSTR_SIZE, HIT_DSTR_COL, CDestruction::TYPE_AIRY, HIT_DSTR_LIFE);
}

//=====================================
// 吹き飛びヒット処理
//=====================================
void CItocan::SmashHit(void)
{
	// 吹き飛ばし処理
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
void CItocan::StunHit(void)
{
	if (IsStun() == false)
	{ // 気絶状況が false の場合

		// 敵の気絶処理
		CEnemy::StunHit();

		// 状態カウントを0にする
		m_nStateCount = 0;
	}
}

//=====================================
// 情報の設定処理
//=====================================
void CItocan::SetData(const D3DXVECTOR3& pos, const TYPE type)
{
	// 情報の設定処理
	CEnemy::SetData(pos, type);

	// モデルの情報を設定する
	SetFileData(CXFile::TYPE_ITOCAN);

	// 全ての値をクリアする
	m_state = STATE_STOP;			// 状態
	m_nStateCount = 0;			// 状態カウント
	m_fRotDest = 0.0f;			// 目標の向き

	// 情報の設定処理
	SetEnableStep(true);			// 踏みつけられる設定
	SetMove(MOVE);					// 移動量
	SetCollSize(COLLISION_SIZE);	// 当たり判定のサイズ
}

//=======================================
// プレイヤーの判定処理
//=======================================
void CItocan::CheckPlayer(void)
{
	if (CPlayer::Get() != nullptr)
	{ // プレイヤーの情報がある場合

		// ローカル変数宣言
		D3DXVECTOR3 posPlayer = CPlayer::Get()->GetPos();	// プレイヤーの位置
		D3DXVECTOR3 pos = GetPos();		// 位置を取得する

		if (posPlayer.x >= pos.x)
		{ // プレイヤーの位置が自身よりも右にいる場合

			// 向きを設定する
			m_fRotDest = -D3DX_PI * 0.5f;
		}
		else
		{ // 上記以外

			// 向きを設定する
			m_fRotDest = D3DX_PI * 0.5f;
		}
	}
}

//=======================================
// 状態の判定処理
//=======================================
void CItocan::CheckState(void)
{
	// ローカルポインタ宣言
	CPlayer* pPlayer = CPlayer::Get();				// プレイヤーの情報を取得する
	D3DXVECTOR3 pos = GetPos();						// 位置
	D3DXVECTOR3 vtxMax = GetFileData().vtxMax;		// 最大値
	D3DXVECTOR3 vtxMin = GetFileData().vtxMin;		// 最小値

	if (pPlayer != nullptr)
	{ // プレイヤーの情報がある場合

		if (pPlayer->GetPos().z <= pos.z + vtxMax.z &&
			pPlayer->GetPos().z >= pos.z + vtxMin.z &&
			fabsf(pos.x - pPlayer->GetPos().x) <= MOVE_DISTANCE)
		{ // 一定距離内の場合

			// 移動状態にする
			m_state = STATE_MOVE;
		}
		else
		{ // 上記以外

			// 位置を設定する
			pos = GetInitPos();

			// 前回の位置を設定する
			SetPosOld(GetInitPos());

			// 停止状態にする
			m_state = STATE_STOP;
		}
	}
	else
	{ // 上記以外

		// 停止状態にする
		m_state = STATE_STOP;
	}

	// 情報を適用する
	SetPos(pos);		// 位置
}

//=======================================
// 移動処理
//=======================================
void CItocan::Move(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置
	D3DXVECTOR3 rot = GetRot();		// 向き
	D3DXVECTOR3 move = GetMove();	// 移動量

	// 移動量を設定する
	pos.x += sinf(-rot.y) * move.x;

	// 位置を設定する
	SetPos(pos);
}

//=======================================
// 向き移動処理
//=======================================
void CItocan::RotMove(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = GetRot();		// 向きを取得する

	// 向きの設定処理
	useful::RotCorrect(m_fRotDest, &rot.y, ROT_MOVE_MAGNI);

	// 向きを適用する
	SetRot(rot);
}

//=======================================
// 死亡時の拡大率処理
//=======================================
void CItocan::DeathScaling(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 scale = GetScale();		// 拡大率を取得する

	// 拡大率の設定処理
	useful::FrameCorrect(DEATH_SCALE, &scale.y, DEATH_ADD_SCALE);

	// 拡大率の設定処理
	SetScale(scale);
}

//=======================================
// 吹き飛び状態処理
//=======================================
void CItocan::Smash(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置
	D3DXVECTOR3 rot = GetRot();		// 向き
	D3DXVECTOR3 move = GetMove();	// 移動量

	// 向きを設定する
	rot.x += SMASH_ROTMOVE_X;
	rot.z += SMASH_ROTMOVE_Z;

	// 向きの正規化処理
	useful::RotNormalize(&rot.x);
	useful::RotNormalize(&rot.z);

	// 位置を更新する
	pos.x += move.x;
	pos.z += move.z;

	// 情報を適用する
	SetPos(pos);		// 位置
	SetRot(rot);		// 向き
}

//=======================================
// 台の着地判定処理
//=======================================
void CItocan::TableLand(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 move = GetMove();		// 移動量を取得する

	// 重力を0.0fにする
	move.y = 0.0f;

	// 移動量を適用する
	SetMove(move);
}