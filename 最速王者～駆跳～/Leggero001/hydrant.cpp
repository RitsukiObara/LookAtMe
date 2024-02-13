//===========================================
//
// 消火栓のメイン処理[obstacle.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "hydrant.h"
#include "collision.h"
#include "sound.h"
#include "useful.h"

#include "manager.h"
#include "input.h"

//==============================
// コンストラクタ
//==============================
CHydrant::CHydrant() : CObstacle(CObject::TYPE_OBSTACLE, CObject::PRIORITY_BLOCK)
{
	// 全ての値をクリアする
	m_state = STATE_NONE;			// 状態
	m_fGravity = 0.0f;				// 重力
	m_fRotMove = 0.0f;				// 向きの移動量
}

//==============================
// デストラクタ
//==============================
CHydrant::~CHydrant()
{

}

//==============================
//消火栓の初期化処理
//==============================
HRESULT CHydrant::Init(void)
{
	if (FAILED(CObstacle::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_state = STATE_NONE;			// 状態
	m_fGravity = 0.0f;				// 重力
	m_fRotMove = 0.0f;				// 向きの移動量

	// 値を返す
	return S_OK;
}

//========================================
//消火栓の終了処理
//========================================
void CHydrant::Uninit(void)
{
	// 終了処理
	CObstacle::Uninit();
}

//========================================
//消火栓の更新処理
//========================================
void CHydrant::Update(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する
	D3DXVECTOR3 rot = GetRot();		// 向きを取得する

	// 前回の位置を設定する
	SetPosOld(pos);

	switch (m_state)
	{
	case STATE_NONE:			// 通常状態

		break;

	case STATE_SMASH:			// 吹き飛び状態

		// X軸の移動量を加算する
		pos.x += 20.0f;

		// Y軸の移動量を加算する
		pos.y += 30.0f;

		// 重力処理
		Gravity(&m_fGravity, pos, 2.5f);

		// 位置を更新する
		SetPos(pos);

		// 向きを加算する
		rot.z += m_fRotMove;

		// 向きの正規化
		RotNormalize(&rot.z);

		// 向きを更新する
		SetRot(rot);

		// ブロックの当たり判定
		if (collision::ObstacleBlockCollision(this) == true)
		{ // ブロックの当たり判定に入った場合

			// 破壊音を鳴らす
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_CRUSH);

			// 終了処理
			Uninit();

			// この先の処理を行わない
			return;
		}

		// 木箱の当たり判定処理
		collision::ObstacleWoodBoxCollision(this);

		if (GetPos().y <= 0.0f)
		{ // 高さが 0.0f 以下になった場合

			// 終了処理
			Uninit();

			// この先の処理を行わない
			return;
		}

		break;
	}

	// 更新処理
	CObstacle::Update();
}

//=====================================
//消火栓の描画処理
//=====================================
void CHydrant::Draw(void)
{
	// 描画処理
	CObstacle::Draw();
}

//=====================================
// ヒット処理
//=====================================
bool CHydrant::Hit(void)
{
	// 吹き飛び状態にする
	m_state = STATE_SMASH;

	// 向きの移動量を設定する
	m_fRotMove = ((rand() % 500) * 0.001f) + 0.1f;

	// 当たり判定の設定処理
	SetCollision(false);

	// アクション判定の設定処理
	SetAttack(false);

	// 攻撃音を鳴らす
	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_ATTACK);

	// true を返す
	return true;
}

//=====================================
// 情報の設定処理
//=====================================
void CHydrant::SetData(const D3DXVECTOR3& pos, const TYPE type)
{
	// 初期化処理
	SetFileData(CXFile::TYPE_HYDRANT);		// データの設定処理

	// 全ての値を設定する
	m_state = STATE_NONE;			// 状態
	m_fGravity = 0.0f;				// 重力
	m_fRotMove = 0.0f;				// 向きの移動量

	// 情報の設定処理
	CObstacle::SetData(pos, type);
}