//============================================
//
// チュートリアルプレイヤーのメイン処理[player_tutorial.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "player_tutorial.h"
#include "tutorial.h"
#include "collision.h"
#include "renderer.h"
#include "area.h"
#include "useful.h"

#include "player_controller.h"
#include "motion.h"
#include "player_action.h"
#include "handgun.h"
#include "aim.h"
#include "lifeUI.h"
#include "airplane.h"
#include "door.h"

// 定数定義
namespace
{
	const D3DXVECTOR3 COLLISION_SIZE = D3DXVECTOR3(40.0f, 200.0f, 40.0f);		// 当たり判定時のサイズ
	const float TRANS_DEPTH = -180.0f;				// 遷移の時の目的のZ座標
	const float TRANS_DEST_ROT = 0.0f;				// 遷移の時の目的の向き
	const float TRANS_DEST_CAMERAROT_X = 1.3f;		// 遷移の時の目的の向き
	const float TRANS_DEST_CAMERAROT_Y = 0.0f;		// 遷移の時の目的の向き
	const float TRANS_DEST_CAMERA_DIST = 800.0f;	// 遷移の時の目的の向き
	const float TRANS_CORRECT = 0.05f;				// 遷移の時の補正係数
	const int MOVE_COUNT = 65;						// 移動するカウント
	const float MOVE_DEPTH = 180.0f;				// 移動する目的のZ座標
	const float TRANS_GRAVITY = 1.0f;				// 遷移状態中の重力
	const D3DXVECTOR3 STAGE_VTXMAX = D3DXVECTOR3(2500.0f, 0.0f, 1500.0f);		// ステージの最大値
	const D3DXVECTOR3 STAGE_VTXMIN = D3DXVECTOR3(-2500.0f, 0.0f, -2500.0f);		// ステージの最小値
}

//=========================================
// コンストラクタ
//=========================================
CTutorialPlayer::CTutorialPlayer() : CPlayer()
{
	// 全ての値をクリアする
	m_nTransCount = 0;		// 遷移カウント
}

//=========================================
// デストラクタ
//=========================================
CTutorialPlayer::~CTutorialPlayer()
{

}

//===========================================
// プレイヤーの初期化処理
//===========================================
HRESULT CTutorialPlayer::Init(void)
{
	if (FAILED(CPlayer::Init()))
	{ // 初期化処理に失敗した場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 値を返す
	return S_OK;
}

//===========================================
// プレイヤーの終了処理
//===========================================
void CTutorialPlayer::Uninit(void)
{
	// 終了処理
	CPlayer::Uninit();

	// プレイヤーのNULL化処理
	CTutorial::DeletePlayer();
}

//===========================================
// 更新処理
//===========================================
void CTutorialPlayer::Update(void)
{
	// 前回の位置の設定処理
	SetPosOld(GetPos());

	switch (CTutorial::GetState())
	{
	case CTutorial::STATE_NONE:

		if (collision::SignboardCollision(GetPos(), COLLISION_SIZE.x) == true)
		{ // 看板に近づいた場合

			// この先の処理を行わない
			return;
		}

		if (collision::DoorHit(GetPos(), COLLISION_SIZE.x) == true)
		{ // ドアを開けた場合

			// この先の処理を行わない
			return;
		}

		// 操作処理
		GetController()->Control(this);

		// 移動処理
		Move();

		if (GetMotion() != nullptr)
		{ // モーションが NULL じゃない場合

			// モーションの更新処理
			GetMotion()->Update();
		}

		if (GetAction() != nullptr)
		{ // 行動が NULL じゃない場合

			// 行動の更新処理
			GetAction()->Update(this);
		}

		for (int nCntGun = 0; nCntGun < NUM_HANDGUN; nCntGun++)
		{
			if (GetHandGun(nCntGun) != nullptr)
			{ // 拳銃が NULL じゃない場合

				// 更新処理
				GetHandGun(nCntGun)->Update();
			}
		}

		// 緊急のリロード処理
		EmergentReload();

		if (GetAim() != nullptr)
		{ // エイムが NULL じゃない場合

			// エイムの更新処理
			GetAim()->Update();
		}

		if (GetLifeUI() != nullptr)
		{ // 寿命が NULL じゃない場合

			// 寿命を設定する
			GetLifeUI()->SetLife(GetLife());
		}

		break;

	case CTutorial::STATE_EXPL:

		break;

	case CTutorial::STATE_TRANS:

		// 遷移状態処理
		Trans();

		if (GetMotion() != nullptr)
		{ // モーションが NULL じゃない場合

			// モーションの更新処理
			GetMotion()->Update();
		}

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	// 区分の設定処理
	SetAreaIdx(area::SetFieldIdx(GetPos()));

	// ヤシの実との当たり判定
	collision::PalmFruitHit(this, COLLISION_SIZE.x, COLLISION_SIZE.y);

	// 金の骨との当たり判定
	collision::GoldBoneCollision(*this, COLLISION_SIZE);

	// 起伏地面との当たり判定処理
	ElevationCollision();

	// 当たり判定処理
	Collision();

	if (CTutorial::GetState() == CTutorial::STATE_NONE)
	{ // 通常状態の場合

		// ドアとの当たり判定
		DoorCollision();

		// ステージとの当たり判定
		StageCollision();
	}
}

//===========================================
// プレイヤーの描画処理
//===========================================
void CTutorialPlayer::Draw(void)
{
	// 描画処理
	CPlayer::Draw();
}

//=======================================
// 情報の設定処理
//=======================================
void CTutorialPlayer::SetData(const D3DXVECTOR3& pos)
{
	// 情報の設定処理
	CPlayer::SetData(pos);

	// 飛行機を消去する
	GetAirplane()->Uninit();
	DeleteAirplane();

	// 位置を再設定する
	SetPos(pos);

	// 全ての値をクリアする
	m_nTransCount = 0;		// 遷移カウント
}

//===========================================
// 生成処理
//===========================================
CTutorialPlayer* CTutorialPlayer::Create(const D3DXVECTOR3& pos)
{
	// プレイヤーのインスタンスを生成
	CTutorialPlayer* pPlayer = nullptr;

	if (pPlayer == nullptr)
	{ // オブジェクトが NULL の場合

		// メモリを確保する
		pPlayer = new CTutorialPlayer;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return pPlayer;
	}

	if (pPlayer != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pPlayer->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pPlayer->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// プレイヤーのポインタを返す
	return pPlayer;
}

//===========================================
// ドアとの当たり判定
//===========================================
void CTutorialPlayer::DoorCollision(void)
{
	// 位置と移動量を取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 move = GetMove();

	// ドアとの当たり判定
	if (collision::DoorCollision(&pos, GetPosOld(), COLLISION_SIZE) == true)
	{ // 上に乗った場合

		// ジャンプ状況を false にする
		SetEnableJump(false);

		// 重力を0にする
		move.y = 0.0f;
	}

	// 位置と移動量を適用
	SetPos(pos);
	SetMove(move);
}

//===========================================
// 遷移状態処理
//===========================================
void CTutorialPlayer::Trans(void)
{
	// 遷移カウントを加算する
	m_nTransCount++;

	CDoor* pDoor = CTutorial::GetDoor();	// ドアの情報
	D3DXVECTOR3 pos = GetPos();				// 位置
	D3DXVECTOR3 rot = GetRot();				// 向き
	D3DXVECTOR3 move = GetMove();			// 移動量
	D3DXVECTOR3 rotCamera = CManager::Get()->GetCamera()->GetRot();
	float fDistCamera = CManager::Get()->GetCamera()->GetDistance();

	if (pDoor != nullptr)
	{ // ドアが NULL じゃない場合

		if (m_nTransCount >= MOVE_COUNT)
		{ // 遷移カウントが一定数以上の場合

			if (pDoor->GetState() == CDoor::STATE_OPEN &&
				GetMotion()->GetType() != MOTIONTYPE_MOVE)
			{ // ドアが開き状態の場合

				// 移動モーションにする
				GetMotion()->Set(MOTIONTYPE_MOVE);
			}

			if (useful::FrameCorrect(pDoor->GetPos().z + MOVE_DEPTH, &pos.z, GetController()->GetSpeedInit()) == true)
			{ // 目的の位置に着いた場合

				// 閉じ状態にする
				pDoor->SetState(CDoor::STATE_CLOSE);

				// 通常モーションにする
				GetMotion()->Set(MOTIONTYPE_NEUTRAL);
			}
		}
		else
		{ // 上記以外

			// 移動する
			useful::Correct(pDoor->GetPos().x, &pos.x, TRANS_CORRECT);
			useful::Correct(pDoor->GetPos().z + TRANS_DEPTH, &pos.z, TRANS_CORRECT);

			// 正面を向く
			useful::RotCorrect(TRANS_DEST_ROT, &rot.y, TRANS_CORRECT);
			useful::RotCorrect(TRANS_DEST_CAMERAROT_X, &rotCamera.x, TRANS_CORRECT);
			useful::RotCorrect(TRANS_DEST_CAMERAROT_Y, &rotCamera.y, TRANS_CORRECT);
			useful::Correct(TRANS_DEST_CAMERA_DIST, &fDistCamera, TRANS_CORRECT);
		}
	}

	// 重力処理
	useful::Gravity(&move.y, &pos.y, TRANS_GRAVITY);

	// 情報を適用
	SetPos(pos);
	SetRot(rot);
	SetMove(move);
	CManager::Get()->GetCamera()->SetRot(rotCamera);
	CManager::Get()->GetCamera()->SetDistance(fDistCamera);
}

//===========================================
// ステージとの当たり判定
//===========================================
void CTutorialPlayer::StageCollision(void)
{
	// 位置を取得
	D3DXVECTOR3 pos = GetPos();

	if (pos.x + COLLISION_SIZE.x >= STAGE_VTXMAX.x)
	{ // 左からはみ出た場合

		// 位置を補正する
		pos.x = STAGE_VTXMAX.x - COLLISION_SIZE.x;
	}

	if (pos.x - COLLISION_SIZE.x <= STAGE_VTXMIN.x)
	{ // 右からはみ出た場合

		// 位置を補正する
		pos.x = STAGE_VTXMIN.x + COLLISION_SIZE.x;
	}

	if (pos.z + COLLISION_SIZE.z >= STAGE_VTXMAX.z)
	{ // 奥からはみ出た場合

		// 位置を補正する
		pos.z = STAGE_VTXMAX.z - COLLISION_SIZE.z;
	}

	if (pos.z - COLLISION_SIZE.z <= STAGE_VTXMIN.z)
	{ // 手前からはみ出た場合

		// 位置を補正する
		pos.z = STAGE_VTXMIN.z + COLLISION_SIZE.z;
	}

	// 位置を適用
	SetPos(pos);
}