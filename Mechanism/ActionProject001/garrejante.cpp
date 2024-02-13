//===========================================
//
// ガレジャンテのメイン処理[garrejante.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "garrejante.h"
#include "stun.h"
#include "useful.h"

#include "destruction.h"
#include "Particle.h"
#include "fraction.h"
#include "sound.h"

//-------------------------------------------
// マクロ定義
//-------------------------------------------
#define COLLISION_SIZE		(D3DXVECTOR3(50.0f,130.0f,50.0f))		// 当たり判定のサイズ
#define DEATH_DSTR_SIZE		(D3DXVECTOR3(40.0f,40.0f,0.0f))			// 死亡時の撃破のサイズ
#define DEATH_DSTR_COL		(D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f))		// 死亡時の撃破の色
#define DEATH_DSTR_LIFE		(20)									// 死亡時の撃破の寿命
#define FRACTION_COUNT		(6)										// 破片の数
#define HIT_DSTR_SIZE		(D3DXVECTOR3(40.0f,40.0f,0.0f))			// ヒット時の撃破のサイズ
#define HIT_DSTR_COL		(D3DXCOLOR(1.0f, 0.7f, 0.1f, 1.0f))		// ヒット時の撃破の色
#define HIT_DSTR_LIFE		(10)									// ヒット時の撃破の寿命
#define STUN_COUNT			(300)									// 気絶が直るまでのカウント
#define SMASH_ROTMOVE_X		(0.05f)									// 吹き飛び状態の向きの移動量(X軸)
#define SMASH_ROTMOVE_Z		(0.06f)									// 吹き飛び状態の向きの移動量(Z軸)
#define SMASH_DEATH_COUNT	(150)									// 吹き飛んで死ぬまでのカウント数
#define DEATH_COUNT			(30)									// 死亡するまでのカウント
#define STUN_SOUND_COUNT	(27)									// 気絶音がなるカウント数

//==============================
// コンストラクタ
//==============================
CGarrejante::CGarrejante() : CEnemy(CObject::TYPE_ENEMY, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_state = STATE_NONE;		// 状態
	m_nStateCount = 0;			// 状態カウント
}

//==============================
// デストラクタ
//==============================
CGarrejante::~CGarrejante()
{

}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CGarrejante::Init(void)
{
	if (FAILED(CEnemy::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_state = STATE_NONE;		// 状態
	m_nStateCount = 0;			// 状態カウント

	// 値を返す
	return S_OK;
}

//========================================
//ブロックの終了処理
//========================================
void CGarrejante::Uninit(void)
{
	// 終了処理
	CEnemy::Uninit();
}

//========================================
//ブロックの更新処理
//========================================
void CGarrejante::Update(void)
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
		case CGarrejante::STATE_NONE:


			break;

		case CGarrejante::STATE_DEATH:

			// 状態カウントを加算する
			m_nStateCount++;

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

		case CGarrejante::STATE_SMASH:

			// 状態カウントを加算する
			m_nStateCount++;

			// 吹き飛び状態処理
			Smash();

			// 重力処理
			Gravity();

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
}

//=====================================
//ブロックの描画処理
//=====================================
void CGarrejante::Draw(void)
{
	// 描画処理
	CEnemy::Draw();
}

//=====================================
// ヒット処理
//=====================================
void CGarrejante::Hit(void)
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
void CGarrejante::SmashHit(void)
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
void CGarrejante::StunHit(void)
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
void CGarrejante::SetData(const D3DXVECTOR3& pos, const TYPE type)
{
	// 情報の設定処理
	CEnemy::SetData(pos, type);

	// モデルの情報を設定する
	SetFileData(CXFile::TYPE_GARREJANTE);

	// 全ての値を設定する
	m_state = STATE_NONE;		// 状態
	m_nStateCount = 0;			// 状態カウント

	// 情報の設定処理
	SetEnableStep(false);			// 踏みつけられる設定
	SetMove(NONE_D3DXVECTOR3);		// 移動量
	SetCollSize(COLLISION_SIZE);	// 当たり判定のサイズ
}

//=====================================
// 吹き飛び状態処理
//=====================================
void CGarrejante::Smash(void)
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
	pos += move;

	// 情報を適用する
	SetPos(pos);		// 位置
	SetRot(rot);		// 向き
}

//=====================================
// 台の着地判定処理
//=====================================
void CGarrejante::TableLand(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 move = GetMove();		// 移動量を取得する

	// 重力を0.0fにする
	move.y = 0.0f;

	// 移動量を適用する
	SetMove(move);
}