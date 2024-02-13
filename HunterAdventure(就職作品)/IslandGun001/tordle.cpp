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
}

//================================
// コンストラクタ
//================================
CTordle::CTordle() : CEnemy()
{
	// 全ての値をクリアする
	m_rotDest = NONE_D3DXVECTOR3;		// 目標の向き
	m_fMoveX = 0.0f;					// 移動量(X軸)
	m_fMoveZ = 0.0f;					// 移動量(Z軸)
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
	// 終了処理
	CEnemy::Uninit();
}

//================================
// 更新処理
//================================
void CTordle::Update(void)
{
	// 前回の位置を設定する
	SetPosOld(GetPos());

	if (GetState() == STATE_NONE)
	{ // 通常状態の場合

		// 追跡処理
		Chase();

		// 向きの移動処理
		RotMove();
	}

	// 移動処理
	Move();

	// 更新処理
	CEnemy::Update();
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
	m_rotDest = NONE_D3DXVECTOR3;		// 目標の向き
	m_fMoveX = 0.0f;					// 移動量(X軸)
	m_fMoveZ = 0.0f;					// 移動量(Z軸)
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