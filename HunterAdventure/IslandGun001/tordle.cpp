//============================================================
//
// タードル処理 [tordle.cpp]
// Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "tordle.h"
#include "motion.h"
#include "file.h"
#include "manager.h"

#include "game.h"
#include "player.h"
#include "useful.h"

//------------------------------------------------------------
// 無名名前空間
//------------------------------------------------------------
namespace
{
	const float SPEED = 5.0f;				// 移動量
	const float KNOCKBACK_HEIGHT = 5.0f;	// ノックバック値の高さ
	const float CHASE_DISTANCE = 2000.0f;	// 追跡状態に入る距離
}

//================================
// コンストラクタ
//================================
CTordle::CTordle() : CEnemy()
{
	// 全ての値をクリアする
	m_action = ACTION_WALK;				// 行動
	m_rotDest = NONE_D3DXVECTOR3;		// 目標の向き
	m_fMoveX = 0.0f;					// 移動量(X軸)
	m_fMoveZ = 0.0f;					// 移動量(Z軸)
	m_walking.pPosDest = nullptr;		// 徘徊経路関係の情報
	m_walking.nNowPosIdx = 0;			// 現在の位置の番号
	m_walking.nNumPos = 0;				// 位置の総数
}

//================================
// デストラクタ
//================================
CTordle::~CTordle()
{

}

//================================
// 初期化処理
//================================
HRESULT CTordle::Init(void)
{
	if (FAILED(CCharacter::Init()))
	{ // 初期化処理に失敗した場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// モデルの総数を設定
	SetNumModel(CMotion::GetNumModel(CMotion::STYLE_TORDLE));

	// データの設定処理
	CCharacter::SetData();

	if (GetMotion() == nullptr)
	{ // モーションが NULL だった場合

		// モーションの生成処理
		CreateMotion();
	}
	else
	{ // ポインタが NULL じゃない場合

		// 停止
		assert(false);
	}

	if (GetMotion() != nullptr)
	{ // ポインタが NULL じゃない場合

		// モーションの情報を取得する
		GetMotion()->SetInfo(CMotion::STYLE_TORDLE, GetHierarchy(), GetNumModel());
	}
	else
	{ // ポインタが NULL じゃない場合

		// 停止
		assert(false);
	}

	// 値を返す
	return S_OK;
}

//================================
// 終了処理
//================================
void CTordle::Uninit(void)
{
	if (m_walking.pPosDest != nullptr)
	{ // 目的の位置が NULL じゃない場合

		// 目的の位置を NULL にする
		delete[] m_walking.pPosDest;
		m_walking.pPosDest = nullptr;
	}

	// 終了処理
	CEnemy::Uninit();
}

//================================
// 更新処理
//================================
void CTordle::Update(void)
{
	if (CGame::GetState() == CGame::STATE_PLAY)
	{ // プレイ状態以外

		// 前回の位置を設定する
		SetPosOld(GetPos());

		switch (GetState())
		{
		case STATE_NONE:

			switch (m_action)
			{
			case CTordle::ACTION_WALK:

				// 徘徊処理
				Walking();

				// 到着処理
				Arrival();

				break;

			case CTordle::ACTION_CHASE:

				// 追跡処理
				Chase();

				// 移動処理
				Move();

				break;

			default:

				// 停止
				assert(false);

				break;
			}

			// 状態判断処理
			StateJudge();

			// 向きの移動処理
			RotMove();

			break;

		case STATE_DAMAGE:

			// 移動処理
			Move();

			break;

		default:

			// 停止
			assert(false);

			break;
		}

		// 更新処理
		CEnemy::Update();
	}
}

//================================
// 描画処理
//================================
void CTordle::Draw(void)
{
	// 描画処理
	CEnemy::Draw();
}

//================================
// 情報の設定処理
//================================
void CTordle::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type)
{
	// 情報の設定処理
	CEnemy::SetData(pos, rot, type);

	// モーションのリセット処理
	GetMotion()->ResetMotion(MOTIONTYPE_MOVE);

	// 全ての値を設定する
	m_action = ACTION_WALK;				// 行動
	m_rotDest = NONE_D3DXVECTOR3;		// 目標の向き
	m_fMoveX = 0.0f;					// 移動量(X軸)
	m_fMoveZ = 0.0f;					// 移動量(Z軸)
	SetRoute();							// 徘徊経路
}

//===========================================
// ヒット処理
//===========================================
void CTordle::Hit(const int nDamage, const float fKnockback)
{
	// ノックバックさせる
	m_fMoveX = sinf(GetRot().y + D3DX_PI) * fKnockback;
	SetGravity(KNOCKBACK_HEIGHT);
	m_fMoveZ = cosf(GetRot().y + D3DX_PI) * fKnockback;

	// 体力を取得する
	int nLife = GetLife();

	// 体力を減らす
	nLife -= nDamage;

	// 体力を適用する
	SetLife(nLife);

	// ダメージ状態にする
	SetState(STATE_DAMAGE);

	// 死亡処理
	Death();
}

//===========================================
// 経路の設定処理
//===========================================
void CTordle::SetRoute(void)
{
	// 徘徊経路を選択
	int nRoute = rand() % CManager::Get()->GetFile()->GetEnemyRouteNum();

	// 位置の総数を取得
	m_walking.nNumPos = CManager::Get()->GetFile()->GetEnemyRouteNumPos(nRoute);

	// 経路のメモリを確保する
	m_walking.pPosDest = new D3DXVECTOR3[m_walking.nNumPos];

	for (int nCnt = 0; nCnt < m_walking.nNumPos; nCnt++)
	{
		// 徘徊経路を取得
		m_walking.pPosDest[nCnt] = CManager::Get()->GetFile()->GetEnemyRoute(nRoute, nCnt);
	}
}

//===========================================
// 徘徊処理
//===========================================
void CTordle::Walking(void)
{
	D3DXVECTOR3 pos = GetPos();

	// 向きを設定する
	m_rotDest.y = atan2f(m_walking.pPosDest[m_walking.nNowPosIdx].x - pos.x, m_walking.pPosDest[m_walking.nNowPosIdx].z - pos.z);

	// 移動量を設定する
	m_fMoveX = fabsf(sinf(m_rotDest.y) * SPEED);
	m_fMoveZ = fabsf(cosf(m_rotDest.y) * SPEED);
}

//===========================================
// 到着処理
//===========================================
void CTordle::Arrival(void)
{
	// 位置を取得
	D3DXVECTOR3 pos = GetPos();

	if (useful::FrameCorrect(m_walking.pPosDest[m_walking.nNowPosIdx].x, &pos.x, m_fMoveX) == true ||
		useful::FrameCorrect(m_walking.pPosDest[m_walking.nNowPosIdx].z, &pos.z, m_fMoveZ) == true)
	{ // 目的の位置に着いた場合

		// 位置を補正する
		pos.x = m_walking.pPosDest[m_walking.nNowPosIdx].x;
		pos.z = m_walking.pPosDest[m_walking.nNowPosIdx].z;

		// 番号を進める
		m_walking.nNowPosIdx = (m_walking.nNowPosIdx + 1) % m_walking.nNumPos;
	}

	// 位置を適用
	SetPos(pos);
}

//===========================================
// 追跡処理
//===========================================
void CTordle::Chase(void)
{
	D3DXVECTOR3 pos;						// 敵の位置
	D3DXVECTOR3 posPlayer;					// プレイヤーの位置
	float fRot = 0.0f;						// 向き

	if (CGame::GetPlayer() != nullptr)
	{ // プレイヤーが存在した場合

		pos = GetPos();									// 位置を取得する
		posPlayer = CGame::GetPlayer()->GetPos();		// プレイヤーの位置を取得する

		// 向きを算出する
		fRot = atan2f((posPlayer.x - pos.x), (posPlayer.z - pos.z));

		// 移動量を設定する
		m_fMoveX = sinf(fRot) * SPEED;
		m_fMoveZ = cosf(fRot) * SPEED;

		// 目標の向きを設定する
		m_rotDest.y = fRot;
	}
}

//===========================================
// 移動処理
//===========================================
void CTordle::Move(void)
{
	// 位置と重力を取得する
	D3DXVECTOR3 pos = GetPos();

	// 移動する
	pos.x += m_fMoveX;
	pos.z += m_fMoveZ;

	// 位置と重力を適用する
	SetPos(pos);
}

//===========================================
// 向きの移動処理
//===========================================
void CTordle::RotMove(void)
{
	// 向きを取得する
	D3DXVECTOR3 rot = GetRot();

	// 向きの補正処理
	useful::RotCorrect(m_rotDest.y, &rot.y, 0.1f);

	// 向きを適用する
	SetRot(rot);
}

//===========================================
// 状態判断処理
//===========================================
void CTordle::StateJudge(void)
{
	if (CGame::GetPlayer() != nullptr)
	{ // プレイヤーが存在した場合

		D3DXVECTOR3 pos = GetPos();			// 位置
		D3DXVECTOR3 posPlayer = CGame::GetPlayer()->GetPos();		// プレイヤーの位置を取得する
		float fDist = sqrtf((posPlayer.x - pos.x) * (posPlayer.x - pos.x) + (posPlayer.z - pos.z) * (posPlayer.z - pos.z));		// 距離

		if (fDist <= CHASE_DISTANCE)
		{ // 距離が一定以下の場合

			// 追跡状態
			m_action = ACTION_CHASE;
		}
		else
		{ // 上記以外

			// 徘徊状態
			m_action = ACTION_WALK;
		}
	}
}