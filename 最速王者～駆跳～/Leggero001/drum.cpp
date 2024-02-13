//===========================================
//
// ドラム缶のメイン処理[drum.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "drum.h"
#include "sound.h"
#include "useful.h"

#include "input.h"
#include "manager.h"
#include "debugproc.h"
#include "collpolygon.h"
#include "collision.h"
#include "fraction.h"

//-------------------------------------------
// マクロ定義
//-------------------------------------------
#define DRUM_ROT_MOVE			(0.1f)			// ドラム缶の向きの移動量
#define DRUM_MOVE				(6.0f)			// ドラム缶の移動量
#define DRUM_GRAVITY			(2.5f)			// ドラム缶の重力
#define DRUM_SOUND_COUNT		(50)			// ドラム缶のサウンドが鳴るカウント

//==============================
// コンストラクタ
//==============================
CDrum::CDrum() : CObstacle(CObject::TYPE_OBSTACLE, CObject::PRIORITY_BLOCK)
{
	// 全ての値をクリアする
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	m_fRotMove = 0.0f;							// 向きの移動量
	m_state = STATE_NONE;						// 状態
	m_nCollIdx = NONE_COLLIDX;					// 当たり判定ポリゴンの番号
	m_nSoundCount = 0;							// サウンドのカウント
}

//==============================
// デストラクタ
//==============================
CDrum::~CDrum()
{

}

//==============================
// ドラム缶の初期化処理
//==============================
HRESULT CDrum::Init(void)
{
	if (FAILED(CObstacle::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	m_fRotMove = 0.0f;							// 向きの移動量
	m_state = STATE_NONE;						// 状態
	m_nCollIdx = NONE_COLLIDX;					// 当たり判定ポリゴンの番号
	m_nSoundCount = 0;							// サウンドのカウント

	// 値を返す
	return S_OK;
}

//========================================
// ドラム缶の終了処理
//========================================
void CDrum::Uninit(void)
{
	if (m_nCollIdx != NONE_COLLIDX)
	{ // ポリゴンのインデックスが初期値じゃない場合

		// 当たり判定ポリゴンの破棄処理
		m_nCollIdx = CCollPolygon::Delete(m_nCollIdx);
	}

	// 終了処理
	CObstacle::Uninit();
}

//========================================
// ドラム缶の更新処理
//========================================
void CDrum::Update(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する
	D3DXVECTOR3 rot = GetRot();		// 向きを取得する

	// 前回の位置を設定する
	SetPosOld(pos);

	switch (m_state)
	{
	case STATE_NONE:

		break;

	case STATE_ROLL:

		if (m_nSoundCount % DRUM_SOUND_COUNT == 0)
		{ // サウンドカウントが一定数になった場合

			// ドラム缶が転がる音を鳴らす
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_DRUMROLL);
		}

		// サウンドカウントを加算する
		m_nSoundCount++;

		// 位置を加算する
		pos.x += m_move.x;

		// 向きを設定する
		rot.z -= m_fRotMove;

		// 向きの正規化
		RotNormalize(&rot.z);

		// 重力処理
		Gravity(&m_move.y, pos, DRUM_GRAVITY);

		// 位置と向きを更新する
		SetPos(pos);
		SetRot(rot);

		// 障害物とブロックの当たり判定
		if (collision::ObstacleBlockCollision(this) == true)
		{ // 当たり判定が true の場合

			// 縦の移動量を0にする
			m_move.y = 0.0f;
		}

		// 木箱の当たり判定処理
		collision::ObstacleWoodBoxCollision(this);

		// ブロックとの当たり判定
		if (collision::ObstacleBlockCrush(this) == true)
		{ // ヒット判定が true の場合

			// 破片の設定処理
			CFraction::SetObstacle(GetPos(), GetFileData().m_nTexIdx[0], GetMaterial(0).MatD3D.Diffuse);

			// 金属系破壊音を鳴らす
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_METALCRUSH);

			// 終了処理
			Uninit();

			// この先の処理を行わない
			return;
		}

		break;
	}

	// 更新処理
	CObstacle::Update();

	// 位置と向きの設定処理
	CCollPolygon::SetPosRot(m_nCollIdx, pos, D3DXVECTOR3(0.0f,0.0f,0.0f));	
}

//=====================================
// ドラム缶の描画処理
//=====================================
void CDrum::Draw(void)
{
	// 描画処理
	CObstacle::Draw();
}

//=====================================
// ヒット処理
//=====================================
bool CDrum::Hit(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する
	D3DXVECTOR3 rot = GetRot();		// 向きを取得する

	// 回転状態にする
	m_state = STATE_ROLL;

	// 移動量を設定する
	m_move.x = DRUM_MOVE;

	// 向きの移動量を設定する
	m_fRotMove = DRUM_ROT_MOVE;

	// 向きを設定する
	rot.x = 0.0f;

	// 位置を設定する
	pos.y = pos.y - GetFileData().vtxMin.y;

	// 位置と向きを更新する
	SetPos(pos);
	SetRot(rot);

	// 当たり判定の設定処理
	SetCollision(true);

	// アクション判定の設定処理
	SetAttack(false);

	// 当たり判定ポリゴンの破棄処理
	m_nCollIdx = CCollPolygon::Delete(m_nCollIdx);

	// 位置と向きを更新する
	SetPos(pos);
	SetRot(rot);

	// 攻撃音を鳴らす
	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_ATTACK);

	// true を返す
	return true;
}

//=====================================
// ヒット(左に飛ばす)処理
//=====================================
void CDrum::HitLeft(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する
	D3DXVECTOR3 rot = GetRot();		// 向きを取得する

	// 回転状態にする
	m_state = STATE_ROLL;

	// 移動量を設定する
	m_move.x = -DRUM_MOVE;

	// 向きの移動量を設定する
	m_fRotMove = -DRUM_ROT_MOVE;

	// 向きを設定する
	rot.x = 0.0f;

	// 位置を設定する
	pos.y = pos.y - GetFileData().vtxMin.y;

	// 位置と向きを更新する
	SetPos(pos);
	SetRot(rot);

	// 当たり判定の設定処理
	SetCollision(true);

	// アクション判定の設定処理
	SetAttack(false);

	// 当たり判定ポリゴンの破棄処理
	m_nCollIdx = CCollPolygon::Delete(m_nCollIdx);

	// 攻撃音を鳴らす
	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_ATTACK);

	// 位置と向きを更新する
	SetPos(pos);
	SetRot(rot);
}

//=====================================
// 情報の設定処理
//=====================================
void CDrum::SetData(const D3DXVECTOR3& pos, const TYPE type)
{
	// 初期化処理
	SetFileData(CXFile::TYPE_DRUM);		// データの設定処理

	// 全ての値を初期化する
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	m_fRotMove = 0.0f;							// 向きの移動量
	m_state = STATE_NONE;						// 状態

	// 情報の設定処理
	CObstacle::SetData(pos, type);

	// 向きの初期化処理
	D3DXVECTOR3 Initrot = GetRot();

	// 向きを初期化する
	Initrot.x = D3DX_PI * 0.5f;

	// 向きの設定処理
	SetRot(Initrot);

	// ローカル変数宣言
	D3DXVECTOR3 VtxMax, VtxMin;

	// 座標の最大値
	VtxMax = D3DXVECTOR3
	(
		GetFileData().vtxMax.x,
		-GetFileData().vtxMin.z,
		0.0f
	);

	// 座標の最小値
	VtxMin = D3DXVECTOR3
	(
		GetFileData().vtxMin.x,
		GetFileData().vtxMax.z,
		0.0f
	);

	// 当たり判定ポリゴンの生成処理
	m_nCollIdx = CCollPolygon::Create(GetPos(), VtxMax, VtxMin);
}