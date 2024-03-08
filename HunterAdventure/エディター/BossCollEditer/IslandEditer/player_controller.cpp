//===========================================
//
// プレイヤーのコントローラーのメイン処理[player_controller.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "player_controller.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "useful.h"

#include "player.h"
#include "motion.h"
#include "player_action.h"
#include "aim.h"
#include "bullet.h"
#include "bulletUI.h"
#include "handgun.h"
#include "dagger.h"
#include "objectElevation.h"

//-------------------------------------------
// 無名名前空間
//-------------------------------------------
namespace
{
	const float CAMERA_ELEVATION_HEIGHT = 30.0f;	// カメラの起伏地面の高さ
	const float JUMP = 24.0f;						// ジャンプ力
	const float CAMERA_ROT_CORRECT = 0.000003f;		// カメラの向きの補正倍率
	const float CAMERA_HEIGHT = 0.000002f;			// カメラの高さの倍率
	const float SHOT_ADD_HEIGHT = 160.0f;			// 射撃時の高さの追加量
	const float AIM_SHIFT = 1000.0f;				// エイムを表示する幅
	const float CAMERA_MOUSE_MAGNI = 5000.0f;		// マウスでのカメラ操作の倍率
	const float SHOT_SHIFT_ROT[NUM_HANDGUN] =		// 射撃時のずらす向き
	{
		(-D3DX_PI * 0.06f),
		(D3DX_PI * 0.06f),
	};
	const float SHOT_SHIFT_LENGTH = 95.0f;			// 射撃時のずらす幅
	const float SHOTGUN_GRAVITY = 15.0f;			// 散弾状態の時の重力
	const float SHOTGUN_RECOIL = 7.0f;				// 散弾状態の反動
	const int LAST_SHOTCOUNT = 2;					// ラストファイアの猶予フレーム
	const int LAST_SHOT_BULLET = 20;				// ラストファイアの弾の数
	const int SHOTGUN_RAND_ROT = 60;				// 散弾のランダムで飛ばす向き
	const int LAST_RAND_ROT = 80;					// ラストファイアのランダムで飛ばす向き
	const int NUM_SHOTGUN_BULLET = 8;				// 散弾で飛ばす弾の数
	const int DODGE_INTERVAL = 90;					// 回避インターバル
	const int SHOT_INTERVAL = 10;					// 撃つインターバル
	const float SPEED = 14.0f;						// 速度
}

//=========================
// コンストラクタ
//=========================
CPlayerController::CPlayerController()
{
	// 全ての値をクリアする
	m_nShotCount = 0;			// 射撃カウント
	m_fStickRot = 0.0f;			// スティックの向き
	m_fSpeed = SPEED;			// 速度
	m_bRightShot = true;		// 右で撃つかどうか
}

//=========================
// デストラクタ
//=========================
CPlayerController::~CPlayerController()
{

}

//=========================
// 行動の初期化処理
//=========================
HRESULT CPlayerController::Init(void)
{
	// 成功を返す
	return S_OK;
}

//=========================
// 行動の終了処理
//=========================
void CPlayerController::Uninit(void)
{
	// 自身を消去する
	delete this;
}

//=======================================
// 操作処理
//=======================================
void CPlayerController::Control(CPlayer* pPlayer)
{
	if (CManager::Get()->GetInputGamePad()->GetConnect() == true)
	{ // ゲームパッドが接続されている場合

		// カメラの操作処理
		CameraControl(pPlayer);
	}
	else
	{ // 上記以外

		// マウスでのカメラの操作処理
		CameraMouse(pPlayer);
	}

	if (pPlayer->GetAction()->GetAction() != CPlayerAction::ACTION_DODGE &&
		pPlayer->GetAction()->GetAction() != CPlayerAction::ACTION_DAGGER &&
		pPlayer->GetAction()->GetAction() != CPlayerAction::ACTION_SHOTGUN &&
		pPlayer->GetAction()->GetAction() != CPlayerAction::ACTION_SWOOP)
	{ // 一定状態以外の場合

		if (CManager::Get()->GetInputGamePad()->GetConnect() == true)
		{ // ゲームパッドが刺さっていた場合

			// 向きの移動処理
			RotMove(pPlayer);
		}
		else
		{ // 上記以外

			// キーボードでの処理
			KeyboardMove(pPlayer);
		}

		if (pPlayer->GetAction() != nullptr &&
			pPlayer->GetAction()->GetAction() != CPlayerAction::ACTION_RELOAD)
		{ // リロード状態以外

			// ジャンプ処理
			Jump(pPlayer);

			// 攻撃処理
			Shot(pPlayer);

			// ダガー処理
			Dagger(pPlayer);

			// 回避処理
			Avoid(pPlayer);
		}
	}
}

//=======================================
// 生成処理
//=======================================
CPlayerController* CPlayerController::Create(void)
{
	// ローカルオブジェクトを生成
	CPlayerController* pControl = nullptr;		// コントローラ－のインスタンスを生成

	if (pControl == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pControl = new CPlayerController;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pControl != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pControl->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// コントローラーを返す
	return pControl;
}

//=======================================
// 起伏地面とカメラの当たり判定
//=======================================
void CPlayerController::ElevationCamera(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 posV = CManager::Get()->GetCamera()->GetPosV();	// 位置を取得する
	float fHeight = 0.0f;			// 高さ
	CListManager<CElevation*> list = CElevation::GetList();
	CElevation* pElev = nullptr;			// 先頭の小判
	CElevation* pElevEnd = nullptr;		// 末尾の値
	int nIdx = 0;

	// while文処理
	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pElev = list.GetTop();

		// 末尾の値を取得する
		pElevEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// 当たり判定を取る
			fHeight = pElev->ElevationCollision(posV) + CAMERA_ELEVATION_HEIGHT;

			if (posV.y < fHeight)
			{ // 当たり判定の位置が高かった場合

				// 高さを設定する
				posV.y = fHeight;
			}

			if (pElev == pElevEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pElev = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}

	// 位置を更新する
	CManager::Get()->GetCamera()->SetPosV(posV);
}

//=======================================
// 向きの設定処理
//=======================================
void CPlayerController::RotMove(CPlayer* pPlayer)
{
	// ローカル変数を宣言する
	float fStickRotX = 0.0f;	// スティックのX座標
	float fStickRotY = 0.0f;	// スティックのY座標
	D3DXVECTOR3 rotDest = pPlayer->GetRotDest();		// 目的の向き
	D3DXVECTOR3 move = pPlayer->GetMove();				// 移動量
	D3DXVECTOR3 CameraRot = CManager::Get()->GetCamera()->GetRot();	// カメラの向き

	// スティックの向きを取る
	fStickRotY = (float)(CManager::Get()->GetInputGamePad()->GetGameStickLYPress(0));
	fStickRotX = (float)(CManager::Get()->GetInputGamePad()->GetGameStickLXPress(0));

	if (fStickRotY != 0 ||
		fStickRotX != 0)
	{ // 右スティックをどっちかに倒した場合

		// スティックの向きを設定する
		m_fStickRot = atan2f(fStickRotX, fStickRotY);

		// 向きの正規化
		useful::RotNormalize(&m_fStickRot);

		// 向きにカメラの向きを加算する
		m_fStickRot += CameraRot.y;

		if (pPlayer->GetAction()->GetAction() != CPlayerAction::ACTION_SHOT)
		{ // 射撃状態以外の場合

			// 向きの正規化
			useful::RotNormalize(&m_fStickRot);

			// 向きを設定する
			rotDest.y = m_fStickRot;

			// 目的の向きを設定する
			pPlayer->SetRotDest(rotDest);
		}

		// 移動量を設定する
		move.x = sinf(m_fStickRot) * m_fSpeed;
		move.z = cosf(m_fStickRot) * m_fSpeed;

		if (pPlayer->GetMotion()->GetType() != CPlayer::MOTIONTYPE_MOVE)
		{ // 移動モーションじゃなかった場合

			// 移動モーションを設定する
			pPlayer->GetMotion()->Set(CPlayer::MOTIONTYPE_MOVE);
		}
	}
	else
	{ // 上記以外

		// 移動量を設定する
		move.x = 0.0f;
		move.z = 0.0f;

		if (pPlayer->GetMotion()->GetType() != CPlayer::MOTIONTYPE_NEUTRAL)
		{ // 移動モーションじゃなかった場合

			// 移動モーションを設定する
			pPlayer->GetMotion()->Set(CPlayer::MOTIONTYPE_NEUTRAL);
		}
	}

	// 移動量を設定する
	pPlayer->SetMove(move);
}

//=======================================
// キーボードでの設定処理
//=======================================
void CPlayerController::KeyboardMove(CPlayer* pPlayer)
{
	// ローカル変数を宣言する
	float fMoveX = 0.0f;	// X軸の移動量
	float fMoveZ = 0.0f;	// Z軸の移動量
	D3DXVECTOR3 rotDest = pPlayer->GetRotDest();		// 目的の向き
	D3DXVECTOR3 move = pPlayer->GetMove();				// 移動量
	D3DXVECTOR3 CameraRot = CManager::Get()->GetCamera()->GetRot();	// カメラの向き

	if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_W) == true)
	{ // Wキーを押した場合

		// Z軸の移動量を設定する
		fMoveZ = 1.0f;
	}

	if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_S) == true)
	{ // Sキーを押した場合

		// Z軸の移動量を設定する
		fMoveZ = -1.0f;
	}

	if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_A) == true)
	{ // Aキーを押した場合

		// X軸の移動量を設定する
		fMoveX = -1.0f;
	}

	if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_D) == true)
	{ // Dキーを押した場合

		// X軸の移動量を設定する
		fMoveX = 1.0f;
	}

	if (fMoveX != 0 ||
		fMoveZ != 0)
	{ // 右スティックをどっちかに倒した場合

		// スティックの向きを設定する
		m_fStickRot = atan2f(fMoveX, fMoveZ);

		// 向きの正規化
		useful::RotNormalize(&m_fStickRot);

		// 向きにカメラの向きを加算する
		m_fStickRot += CameraRot.y;

		if (pPlayer->GetAction()->GetAction() != CPlayerAction::ACTION_SHOT)
		{ // 射撃状態以外の場合

			// 向きの正規化
			useful::RotNormalize(&m_fStickRot);

			// 向きを設定する
			rotDest.y = m_fStickRot;
		}

		// 移動量を設定する
		move.x = sinf(m_fStickRot) * m_fSpeed;
		move.z = cosf(m_fStickRot) * m_fSpeed;

		if (pPlayer->GetMotion()->GetType() != CPlayer::MOTIONTYPE_MOVE)
		{ // 移動モーションじゃなかった場合

			// 移動モーションを設定する
			pPlayer->GetMotion()->Set(CPlayer::MOTIONTYPE_MOVE);
		}
	}
	else
	{ // 上記以外

		// 移動量を設定する
		move.x = 0.0f;
		move.z = 0.0f;

		if (pPlayer->GetMotion()->GetType() != CPlayer::MOTIONTYPE_NEUTRAL)
		{ // 移動モーションじゃなかった場合

			// 移動モーションを設定する
			pPlayer->GetMotion()->Set(CPlayer::MOTIONTYPE_NEUTRAL);
		}
	}

	// 移動量と目標の向きを設定する
	pPlayer->SetMove(move);
	pPlayer->SetRotDest(rotDest);
}

//=======================================
// ジャンプ処理
//=======================================
void CPlayerController::Jump(CPlayer* pPlayer)
{
	// 移動量を取得する
	D3DXVECTOR3 move = pPlayer->GetMove();

	if ((CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true ||
		CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_SPACE) == true) &&
		pPlayer->IsJump() == false)
	{ // Aボタンを押した場合

		// 移動量を設定する
		move.y = JUMP;

		// ジャンプしている
		pPlayer->SetEnableJump(true);
	}

	// 移動量を適用する
	pPlayer->SetMove(move);
}

//=======================================
// カメラの操作処理
//=======================================
void CPlayerController::CameraControl(CPlayer* pPlayer)
{
	D3DXVECTOR3 CameraRot = CManager::Get()->GetCamera()->GetRot();		// カメラの向きを取得する
	float fStickRotX, fStickRotY;		// スティックの向き

	// 右スティックの傾け度を取得する
	fStickRotX = CManager::Get()->GetInputGamePad()->GetGameStickRXPress(0);
	fStickRotY = CManager::Get()->GetInputGamePad()->GetGameStickRYPress(0);

	// カメラの向きを加算する
	CameraRot.y += (fStickRotX * CAMERA_ROT_CORRECT);

	// 向きの正規化
	useful::RotNormalize(&CameraRot.y);

	// カメラの向きを加算する
	CameraRot.x -= (fStickRotY * CAMERA_HEIGHT);

	if (CameraRot.x >= D3DX_PI - 0.02f)
	{ // 向きが一定を超えた場合

		CameraRot.x = D3DX_PI - 0.02f;
	}
	else if (CameraRot.x <= 0.02f)
	{ // 向きが一定を超えた場合

		CameraRot.x = 0.02f;
	}

	if (pPlayer->GetAim() != nullptr)
	{ // エイムが NULL じゃない場合

		D3DXVECTOR3 posShot;		// 弾を出す位置

		// 位置を設定する
		posShot.x = pPlayer->GetPos().x;
		posShot.y = pPlayer->GetPos().y + SHOT_ADD_HEIGHT;
		posShot.z = pPlayer->GetPos().z;

		// 射撃の位置を設定する
		pPlayer->GetAim()->SetPosPlayer(posShot);
	}

	//// 起伏地面とカメラの当たり判定
	//ElevationCamera();

	// 向きを適用する
	CManager::Get()->GetCamera()->SetRot(CameraRot);
}

//=======================================
// マウスでのカメラの操作処理
//=======================================
void CPlayerController::CameraMouse(CPlayer* pPlayer)
{
	D3DXVECTOR3 CameraRot = CManager::Get()->GetCamera()->GetRot();		// カメラの向きを取得する
	float fMoveX = 0.0f;
	float fMoveZ = 0.0f;		// スティックの向き

	fMoveX = CManager::Get()->GetInputMouse()->GetMove().x * CAMERA_MOUSE_MAGNI;
	fMoveZ = -CManager::Get()->GetInputMouse()->GetMove().y * CAMERA_MOUSE_MAGNI;

	// カメラの向きを加算する
	CameraRot.y += (fMoveX * CAMERA_ROT_CORRECT);

	// 向きの正規化
	useful::RotNormalize(&CameraRot.y);

	// カメラの向きを加算する
	CameraRot.x -= (fMoveZ * CAMERA_HEIGHT);

	if (CameraRot.x >= D3DX_PI - 0.01f)
	{ // 向きが一定を超えた場合

		CameraRot.x = D3DX_PI - 0.01f;
	}
	else if (CameraRot.x <= 0.0f + 0.01f)
	{ // 向きが一定を超えた場合

		CameraRot.x = 0.0f + 0.01f;
	}

	if (pPlayer->GetAim() != nullptr)
	{ // エイムが NULL じゃない場合

		D3DXVECTOR3 posShot;		// 弾を出す位置

		// 位置を設定する
		posShot.x = pPlayer->GetPos().x;
		posShot.y = pPlayer->GetPos().y + SHOT_ADD_HEIGHT;
		posShot.z = pPlayer->GetPos().z;

		// 射撃の位置を設定する
		pPlayer->GetAim()->SetPosPlayer(posShot);
	}

	//// 起伏地面とカメラの当たり判定
	//ElevationCamera();

	// 向きを適用する
	CManager::Get()->GetCamera()->SetRot(CameraRot);
}

//=======================================
// 射撃系処理
//=======================================
void CPlayerController::Shot(CPlayer* pPlayer)
{
	if ((pPlayer->GetAction()->GetAction() == CPlayerAction::ACTION_NONE ||
		pPlayer->GetAction()->GetAction() == CPlayerAction::ACTION_SHOT) &&
		(CManager::Get()->GetInputGamePad()->GetPress(CInputGamePad::JOYKEY_RB, 0) == true ||
		CManager::Get()->GetInputGamePad()->GetPress(CInputGamePad::JOYKEY_LB, 0) == true ||
		CManager::Get()->GetInputMouse()->GetPress(CInputMouse::MOUSE_L) == true) &&
		pPlayer->GetBulletUI()->GetNumBullet() > 0)
	{ // 残弾があり、RB・LBキーを押した場合

		if (pPlayer->IsJump() == true)
		{ // ジャンプしていた場合

			// 散弾処理
			ShotGun(pPlayer);
		}
		else
		{ // 上記以外

			// 拳銃処理
			HandGun(pPlayer);
		}

		// 射撃カウントを加算する
		m_nShotCount++;
	}

	if (pPlayer->GetAction()->GetAction() != CPlayerAction::ACTION_SHOTGUN &&
		(CManager::Get()->GetInputGamePad()->GetRelease(CInputGamePad::JOYKEY_RB, 0) == true ||
		CManager::Get()->GetInputGamePad()->GetRelease(CInputGamePad::JOYKEY_LB, 0) == true ||
		CManager::Get()->GetInputMouse()->GetRelease(CInputMouse::MOUSE_L) == true))
	{ // RB・LBボタンを離した場合

		// 射撃カウントを0にする
		m_nShotCount = 0;

		if (pPlayer->GetAction() != nullptr &&
			pPlayer->GetAction()->GetAction() != CPlayerAction::ACTION_RELOAD)
		{ // リロード状態以外の場合

			// 通常状態にする
			pPlayer->GetAction()->SetAction(CPlayerAction::ACTION_NONE);
		}
	}
}

//=======================================
// 拳銃処理
//=======================================
void CPlayerController::HandGun(CPlayer* pPlayer)
{
	// 残弾数を取得する
	int nNumBullet = pPlayer->GetBulletUI()->GetNumBullet();

	// 射撃状態にする
	pPlayer->GetAction()->SetAction(CPlayerAction::ACTION_SHOT);

	if (m_nShotCount % SHOT_INTERVAL == 0)
	{ // 一定カウントごとに

		D3DXVECTOR3 pos;		// 弾の出る位置を宣言
		D3DXVECTOR3 rot;		// 弾の出る向きを宣言

		// 向きを設定する
		rot = CManager::Get()->GetCamera()->GetRot();

		// 向きの正規化
		useful::RotNormalize(&rot.x);

		// 位置を設定する
		pos.x = pPlayer->GetPos().x + sinf(pPlayer->GetRot().y + SHOT_SHIFT_ROT[(int)(m_bRightShot)]) * SHOT_SHIFT_LENGTH;
		pos.y = pPlayer->GetPos().y + SHOT_ADD_HEIGHT;
		pos.z = pPlayer->GetPos().z + cosf(pPlayer->GetRot().y + SHOT_SHIFT_ROT[(int)(m_bRightShot)]) * SHOT_SHIFT_LENGTH;

		// 弾を撃つ
		CBullet::Create(pos, rot, CBullet::TYPE::TYPE_HANDGUN);

		// 右で撃つかどうかを変える
		m_bRightShot = !m_bRightShot;

		// 残弾数を減らす
		nNumBullet--;

		// 残弾数を適用する
		pPlayer->GetBulletUI()->SetNumBullet(nNumBullet);

		if (nNumBullet <= 0)
		{ // 残弾が0になった場合

			// リロード状態にする
			pPlayer->GetAction()->SetAction(CPlayerAction::ACTION_RELOAD);

			for (int nCnt = 0; nCnt < NUM_HANDGUN; nCnt++)
			{
				if (pPlayer->GetHandGun(nCnt) != nullptr)
				{ // 拳銃が NULL じゃない場合

					// リロード状態に設定する
					pPlayer->GetHandGun(nCnt)->SetState(CHandgun::STATE_RELOAD);
				}
			}
		}
	}
}

//=======================================
// 散弾処理
//=======================================
void CPlayerController::ShotGun(CPlayer* pPlayer)
{
	// 反動の移動量とカメラの向きを宣言
	D3DXVECTOR3 move;
	D3DXVECTOR3 rotCamera = CManager::Get()->GetCamera()->GetRot();
	int nNumBullet = pPlayer->GetBulletUI()->GetNumBullet();

	// 移動量を設定する
	move.x = sinf(rotCamera.y) * -SHOTGUN_RECOIL;
	move.y = SHOTGUN_GRAVITY;
	move.z = cosf(rotCamera.y) * -SHOTGUN_RECOIL;

	// 反動の移動量を設定する
	pPlayer->GetAction()->SetMoveRecoil(move);

	// 移動量を初期化する
	pPlayer->SetMove(NONE_D3DXVECTOR3);

	D3DXVECTOR3 pos = pPlayer->GetPos();	// プレイヤーの位置を宣言
	D3DXVECTOR3 rot;						// プレイヤーの向きを宣言
	D3DXVECTOR3 rotBullet;					// 弾の出る向きを宣言

	// 向きを設定する
	rot = CManager::Get()->GetCamera()->GetRot();

	// 向きの正規化
	useful::RotNormalize(&rot.x);

	// 位置を設定する
	pos.x = pPlayer->GetPos().x;
	pos.y = pPlayer->GetPos().y + SHOT_ADD_HEIGHT;
	pos.z = pPlayer->GetPos().z;

	if (nNumBullet == 1 &&
		m_nShotCount < LAST_SHOTCOUNT)
	{ // 最後の弾だった場合

		for (int nCnt = 0; nCnt < LAST_SHOT_BULLET; nCnt++)
		{
			// 弾の出る向きを設定する
			rotBullet.x = rot.x + (float)((rand() % LAST_RAND_ROT - (LAST_RAND_ROT / 2)) * 0.01f);
			rotBullet.y = rot.y + (float)((rand() % LAST_RAND_ROT - (LAST_RAND_ROT / 2)) * 0.01f);
			rotBullet.z = rot.z + (float)((rand() % LAST_RAND_ROT - (LAST_RAND_ROT / 2)) * 0.01f);

			// 弾を撃つ
			CBullet::Create(pos, rotBullet, CBullet::TYPE::TYPE_SHOTGUN);
		}
	}
	else
	{ // 上記以外

		for (int nCnt = 0; nCnt < NUM_SHOTGUN_BULLET; nCnt++)
		{
			// 弾の出る向きを設定する
			rotBullet.x = rot.x + (float)((rand() % SHOTGUN_RAND_ROT - (SHOTGUN_RAND_ROT / 2)) * 0.01f);
			rotBullet.y = rot.y + (float)((rand() % SHOTGUN_RAND_ROT - (SHOTGUN_RAND_ROT / 2)) * 0.01f);
			rotBullet.z = rot.z + (float)((rand() % SHOTGUN_RAND_ROT - (SHOTGUN_RAND_ROT / 2)) * 0.01f);

			// 弾を撃つ
			CBullet::Create(pos, rotBullet, CBullet::TYPE::TYPE_SHOTGUN);
		}
	}

	// 残弾数を減らす
	nNumBullet--;

	// 残弾数を適用する
	pPlayer->GetBulletUI()->SetNumBullet(nNumBullet);

	if (nNumBullet <= 0)
	{ // 残弾が0になった場合

		// リロード状態にする
		pPlayer->GetAction()->SetAction(CPlayerAction::ACTION_RELOAD);

		for (int nCnt = 0; nCnt < NUM_HANDGUN; nCnt++)
		{
			if (pPlayer->GetHandGun(nCnt) != nullptr)
			{ // 拳銃が NULL じゃない場合

				// 拳銃をリロード状態に設定する
				pPlayer->GetHandGun(nCnt)->SetState(CHandgun::STATE_RELOAD);
			}
		}
	}
	else
	{ // 上記以外

		// 散弾状態にする
		pPlayer->GetAction()->SetAction(CPlayerAction::ACTION_SHOTGUN);
	}

	// 反動状況を true にする
	pPlayer->GetAction()->SetEnableRecoil(true);
}

//=======================================
// 回避処理
//=======================================
void CPlayerController::Avoid(CPlayer* pPlayer)
{
	D3DXVECTOR3 rotDest = pPlayer->GetRotDest();

	if (pPlayer->IsJump() == false &&
		pPlayer->GetAction()->IsDodgeUse() == true &&
		(CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_B, 0) == true ||
			CManager::Get()->GetInputMouse()->GetTrigger(CInputMouse::MOUSE_R) == true))
	{ // 地上でBキーを押した場合

		// 回避状態にする
		pPlayer->GetAction()->SetAction(CPlayerAction::ACTION_DODGE);

		// 回避する向きを設定する
		pPlayer->GetAction()->SetDodgeRot(m_fStickRot);

		if (pPlayer->GetMotion()->GetType() != CPlayer::MOTIONTYPE_DODGE)
		{ // 回避モーションじゃない場合

			// 回避モーションにする
			pPlayer->GetMotion()->Set(CPlayer::MOTIONTYPE_DODGE);
		}

		// 目標の向きを設定する
		rotDest.y = m_fStickRot;

		// 回避インターバルを設定する
		pPlayer->GetAction()->SetDodgeInterval(DODGE_INTERVAL);

		// 使用可能状況をfalseにする
		pPlayer->GetAction()->SetEnableDodgeUse(false);
	}

	// 目的の向きを適用する
	pPlayer->SetRotDest(rotDest);
}

//=======================================
// ダガー処理
//=======================================
void CPlayerController::Dagger(CPlayer* pPlayer)
{
	if (CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_X, 0) == true ||
		CManager::Get()->GetInputMouse()->GetTrigger(CInputMouse::MOUSE_WHEEL) == true)
	{ // Xキーを押した場合

		if (pPlayer->IsJump() == true)
		{ // ジャンプ状況が true の場合

			// 急降下状態にする
			pPlayer->GetAction()->SetAction(CPlayerAction::ACTION_SWOOP);
		}
		else
		{ // 上記以外

			// ダガー状態にする
			pPlayer->GetAction()->SetAction(CPlayerAction::ACTION_DAGGER);
		}

		if (pPlayer->GetMotion()->GetType() != CPlayer::MOTIONTYPE_DAGGER)
		{ // ダガーモーション以外の場合

			// ダガーモーションを設定する
			pPlayer->GetMotion()->Set(CPlayer::MOTIONTYPE_DAGGER);
		}

		// ダガーを表示する
		pPlayer->GetDagger()->SetEnableDisp(true);

		// 拳銃を描画しない
		pPlayer->GetHandGun(0)->SetEnableDisp(false);
		pPlayer->GetHandGun(1)->SetEnableDisp(false);
	}
}