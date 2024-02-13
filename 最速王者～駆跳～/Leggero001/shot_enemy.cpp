//============================================
//
// 弾撃ち敵のメイン処理[shotenemy.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "model.h"
#include "shot_enemy.h"
#include "manager.h"
#include "game.h"
#include "collision.h"
#include "renderer.h"
#include "motion.h"
#include "useful.h"

#include "bullet.h"
#include "player.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define SHOTENEMY_SHOT_COUNT		(60)		// 敵が弾を打つインターバル
#define SHOTENEMY_ACT_CHANGE_ROT	(0.01f)		// 敵が行動状態を変える基準となる向き
#define SHOTENEMY_ROT_MAGNI			(0.5f)		// 敵が向きを変える倍率

//===========================================
// コンストラクタ
//===========================================
CShotEnemy::CShotEnemy() : CEnemy(CObject::TYPE_ENEMY, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_action = ACT_SHOT;		// 行動状態
	m_nShotCount = 0;			// 発射カウント
	m_fGravity = 0.0f;			// 重力
	m_fRotDest = 0.0f;			// 目的の向き
	m_fRotDiff = 0.0f;			// 向きの差分
}

//===========================================
// デストラクタ
//===========================================
CShotEnemy::~CShotEnemy()
{

}

//===========================================
// 初期化処理
//===========================================
HRESULT CShotEnemy::Init(void)
{
	// 全ての値をクリアする
	m_action = ACT_SHOT;		// 行動状態
	m_nShotCount = 0;			// 発射カウント
	m_fGravity = 0.0f;			// 重力
	m_fRotDest = 0.0f;			// 目的の向き
	m_fRotDiff = 0.0f;			// 向きの差分

	if (FAILED(CCharacter::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// ベタ打ち
	SetNumModel(14);

	// データの設定処理
	CCharacter::SetData();

	if (GetMotion() == nullptr)
	{ // モーションが NULL だった場合

		// モーションの設定処理
		SetMotion();
	}
	else
	{ // ポインタが NULL じゃない場合

		// 警告文
		MessageBox(NULL, "射撃敵のモーションのポインタが使われています！", "警告！", MB_ICONWARNING);
	}

	if (GetMotion() != nullptr)
	{ // ポインタが NULL じゃない場合

		// モーションの情報を取得する
		GetMotion()->SetModel(GetHierarchy(), GetNumModel());

		// ロード処理
		GetMotion()->Load(CMotion::TYPE_PLAYER);
	}
	else
	{ // ポインタが NULL じゃない場合

		// 警告文
		MessageBox(NULL, "射撃敵のモーションのメモリが確保できませんでした！", "警告！", MB_ICONWARNING);
	}

	// モーションの設定処理
	GetMotion()->Set(MOTIONTYPE_NEUTRAL);

	// 成功を返す
	return S_OK;
}

//===========================================
// 終了処理
//===========================================
void CShotEnemy::Uninit(void)
{
	// 終了
	CEnemy::Uninit();
}

//===========================================
// 更新処理
//===========================================
void CShotEnemy::Update(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する

	// 前回の位置を設定する
	SetPosOld(pos);

	// 振り向き処理
	Turn();

	switch (m_action)
	{
	case ACT_SHOT:

		// 弾撃ち状態での処理
		ActionShot();

		break;

	case ACT_TURN:

		// 振り向き状態での処理
		ActionTurn();

		break;
	}

	// 重力処理
	Gravity(&m_fGravity, pos, ENEMY_GRAVITY);

	// 位置を設定する
	SetPos(pos);

	if (collision::EnemyBlockCollision(this) == true)
	{ // ブロックとの当たり判定が true の場合

		// 重力を 0.0f にする
		m_fGravity = 0.0f;
	}

	// モーションの更新処理
	GetMotion()->Update();
}

//===========================================
// 描画処理
//===========================================
void CShotEnemy::Draw(void)
{
	// 描画処理
	CEnemy::Draw();
}

//===========================================
// 情報の設定処理
//===========================================
void CShotEnemy::SetData(const D3DXVECTOR3& pos)
{
	// 全ての値を初期化する
	SetPos(pos);			// 位置
	SetPosOld(pos);			// 前回の位置
	SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));			// 向き
	SetScale(NONE_SCALE);	// 拡大率

	for (int nCntData = 0; nCntData < GetNumModel(); nCntData++)
	{
		// 初期化処理
		GetHierarchy(nCntData)->SetPos(pos);										// 位置
		GetHierarchy(nCntData)->SetPosOld(pos);										// 前回の位置
		GetHierarchy(nCntData)->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));				// 向き
		GetHierarchy(nCntData)->SetScale(NONE_SCALE);								// 拡大率
		GetHierarchy(nCntData)->SetFileData(CXFile::TYPE(nCntData));	// データの設定処理
	}
}

//===========================================
// 初期の向きの設定処理
//===========================================
void CShotEnemy::InitRot(void)
{
	// ローカルポインタ宣言
	CPlayer* pPlayer = CScene::GetPlayer();		// プレイヤーの情報を取得する

	if (pPlayer != nullptr)
	{ // プレイヤーの情報がある場合

		if (pPlayer != nullptr)
		{ // プレイヤーが NULL じゃない場合

			if (pPlayer->GetPos().x > GetPos().x)
			{ // プレイヤーが敵より右にいた場合

				// 目的の向きを設定する
				m_fRotDest = -D3DX_PI * 0.5f;

				// 振り向き状態にする
				m_action = ACT_TURN;
			}
			else
			{ // プレイヤーが敵より左にいた場合

				// 目的の向きを設定する
				m_fRotDest = D3DX_PI * 0.5f;

				// 振り向き状態にする
				m_action = ACT_TURN;
			}
		}
	}
	else
	{ // プレイヤーの情報がない場合

		// 向きの設定処理
		SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
}

//===========================================
// 射撃処理
//===========================================
void CShotEnemy::Shot(void)
{
	// 発射カウントを加算する
	m_nShotCount++;

	if (m_nShotCount % SHOTENEMY_SHOT_COUNT == 0)
	{ // 発射カウントが一定数になった場合

		// 弾の生成処理
		CBullet::Create(D3DXVECTOR3(GetPos().x, GetPos().y + 50.0f, 0.0f), BULLET_SIZE, -GetRot().y, CBullet::TYPE_ENEMY);
	}
}

//===========================================
// 振り向き処理
//===========================================
void CShotEnemy::Turn(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する

	// ローカルポインタ宣言
	CPlayer* pPlayer = CScene::GetPlayer();		// プレイヤーの情報を取得する

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		if (pPlayer->GetPosOld().x < pos.x &&
			pPlayer->GetPos().x > pos.x)
		{ // プレイヤーが敵より右に移動した場合

			// 目的の向きを設定する
			m_fRotDest = -D3DX_PI * 0.5f;

			// 振り向き状態にする
			m_action = ACT_TURN;
		}
		else if(pPlayer->GetPosOld().x > pos.x &&
			pPlayer->GetPos().x < pos.x)
		{ // プレイヤーが敵より左に移動した場合

			// 目的の向きを設定する
			m_fRotDest = D3DX_PI * 0.5f;

			// 振り向き状態にする
			m_action = ACT_TURN;
		}
	}
}

//===========================================
// 弾撃ち状態での処理
//===========================================
void CShotEnemy::ActionShot(void)
{
	// 射撃処理
	Shot();
}

//===========================================
// 振り向き状態での処理
//===========================================
void CShotEnemy::ActionTurn(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = GetRot();		// 向きを取得する

	// 向きの差分を計算する
	m_fRotDiff = m_fRotDest - rot.y;

	// 向きに差分を加算する
	rot.y += m_fRotDiff * SHOTENEMY_ROT_MAGNI;

	// 向きの正規化
	RotNormalize(&rot.y);

	if (rot.y >= m_fRotDest - SHOTENEMY_ACT_CHANGE_ROT &&
		rot.y <= m_fRotDest + SHOTENEMY_ACT_CHANGE_ROT)
	{ // 向きがある程度向いた場合

		// 向きを設定する
		rot.y = m_fRotDest;

		// 弾撃ち状態にする
		m_action = ACT_SHOT;

		// 弾撃ちカウントを初期化する
		m_nShotCount = 0;
	}

	// 向きを更新する
	SetRot(rot);
}