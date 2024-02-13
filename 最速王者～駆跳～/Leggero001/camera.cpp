//===========================================
//
// カメラのメイン処理[camera.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "tutorial.h"
#include "game.h"
#include "useful.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "object.h"
#include "model.h"
#include "scene.h"
#include "file.h"

#include "player.h"
#include "editAll.h"
#include "pause.h"
#include "game_time.h"
#include "countdown.h"

//-------------------------------------------
// マクロ定義
//-------------------------------------------

// カメラ全体
#define ASPECT_RATIO				(80.0f)				// 画面のアスペクト比
#define MIN_DISTANCE				(50.0f)				// 距離の最小値
#define MAX_DISTANCE				(8000.0f)			// 距離の最大値
#define DRAW_MIN_Z					(10.0f)				// Z軸の最小値
#define DRAW_MAX_Z					(50000.0f)			// Z軸の最大値

// 向き関係
#define ROT_Y_SPEED					(0.04f)				// Y軸の回転の速度
#define ROTATION_SPEED				(0.05f)				// 回り込み処理を行う基準のモデルの速度
#define ROTATION_ROT				(0.02f)				// カメラの角度の補正倍率

// 位置・距離関係
#define POS_SPEED					(30.0f)				// 移動速度
#define DIS_SPEED					(16.0f)				// 距離の移動量
#define CAMERA_DISTANCE				(300.0f)			// カメラの距離
#define POSR_POINT					(40.0f)				// 追従モードの注視点の位置
#define POSV_POINT					(40.0f)				// 追従モードの視点の位置
#define CORRECT_POSR				(0.22f)				// 注視点の補正倍率
#define CORRECT_POSV				(0.20f)				// 視点の補正倍率
#define RANKING_MOVE				(40.0f)				// ランキングカメラの移動量
#define RANKING_STOP				(25000.0f)			// ランキングカメラの止まる座標

// 特殊カメラ関係
#define CORRECT_UPWARD_POSV			(0.05f)				// 上向きカメラの視点の補正倍率
#define ZOOMOUT_DISTANCE			(600.0f)			// ズームアウトカメラの距離
#define ZOOMIN_DISTANCE				(150.0f)			// ズームインカメラの距離
#define ZOOM_DIST_CORRECT			(0.3f)				// ズームの距離の補正
#define LANDING_PROCESS_COUNT		(3)					// 着地カメラの距離が変化するカウント数
#define LANDING_CHANGE_COUNT		(9)					// 着地カメラのカウント数
#define LANDING_CORRECT_POSR		(0.3f)				// 着地カメラの注視点の補正倍率
#define LANDING_CORRECT_POSV		(0.3f)				// 着地カメラの視点の補正倍率
#define STAGE_CAMERA_SHIFT_WIDTH	(500.0f)			// ステージ投影カメラのずらす幅(X軸)
#define STAGE_CAMERA_HEIGHT			(500.0f)			// ステージ投影カメラの高さ
#define STAGE_CAMERA_DISTANCE		(1000.0f)			// ステージ投影カメラの距離
#define STAGE_CAMERA_MOVE			(200.0f)			// ステージ投影カメラの移動量
#define STAGE_CORRECT_POS			(0.5f)				// ステージ投影カメラの補正倍率
#define RUN_CAMERA_DISTANCE			(200.0f)			// 走り姿カメラの距離
#define BACK_CORRECT_MAGNI			(0.05f)				// 戻りカメラの補正倍率
#define BACK_DECISION_DISTANCE		(CAMERA_DISTANCE - 0.001f)		// 戻りカメラの判定が通る距離
#define BOOST_DISTANCE				(400.0f)			// ブーストカメラの距離
#define BOOST_DIST_CORRECT			(0.05f)				// ズームの距離の補正
#define PRECEDENCE_POS				(450.0f)			// 先行投影カメラの距離

//=======================
// コンストラクタ
//=======================
CCamera::CCamera()
{
	// 全ての情報をクリアする
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 視点
	m_posVDest = m_posV;						// 目的の視点
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 注視点
	m_posRDest = m_posR;						// 目的の注視点
	m_VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 上方向ベクトル
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	m_type = TYPE_NONE;							// 種類
	m_rotDest = m_rot.y;						// 目的の向き
	m_Dis = 0.0f;								// 距離
	m_DisDest = 0.0f;							// 目的の距離
	m_nSwingCount = 0;							// 揺れカメラのカウント
	m_bControl = false;							// 操作状況
}

//=======================
// デストラクタ
//=======================
CCamera::~CCamera()
{

}

//=======================
// カメラの初期化処理
//=======================
HRESULT CCamera::Init(void)
{
	//カメラの情報のリセット処理
	Reset();

	// ビューポートの設定処理
	m_viewport.X = (DWORD)0.0f;				// 描画する画面の左上X座標
	m_viewport.Y = (DWORD)0.0f;				// 描画する画面の左上Y座標
	m_viewport.Width = SCREEN_WIDTH;	// 描画する画面の幅
	m_viewport.Height = SCREEN_HEIGHT;	// 描画する画面の高さ
	m_viewport.MinZ = 0.0f;				// クリップボリュームの最小値
	m_viewport.MaxZ = 1.0f;				// クリップボリュームの最大値

	// 成功を返す
	return S_OK;
}

//=======================
// カメラの終了処理
//=======================
void CCamera::Uninit(void)
{

}

//=======================
// カメラの更新処理
//=======================
void CCamera::Update(void)
{
	switch (CManager::GetMode())
	{
	case CScene::MODE_TUTORIAL:	// チュートリアル

		switch (CTutorial::GetState())
		{
		case CTutorial::STATE_END:

			if (CScene::GetPlayer() != nullptr)
			{ // プレイヤーの情報があった場合

				// 目的の注視点を設定する
				m_posRDest.y = CScene::GetPlayer()->GetPos().y + PLAYER_HEIGHT * 0.5f;

				// 視点を補正
				m_posV.x += (m_posVDest.x - m_posV.x) * CORRECT_POSV;
				m_posV.y += (m_posVDest.y - m_posV.y) * CORRECT_POSR;
				m_posV.z += (m_posVDest.z - m_posV.z) * CORRECT_POSV;

				// 注視点を設定する
				m_posR.x = CScene::GetPlayer()->GetPos().x;
				m_posR.y += (m_posRDest.y - m_posR.y) * CORRECT_POSR;
				m_posR.z = CScene::GetPlayer()->GetPos().z;
			}

			break;

		default:

#if 1
			// 種類ごとの更新処理
			TypeProcess();
#else
			// 操作処理
			Control();
#endif
			break;
		}

		break;

	case CScene::MODE_GAME:		// ゲームモード

		if (CGame::GetPause() != nullptr &&
			CGame::GetPause()->GetPause() == false)
		{ // ポーズ中以外の場合

			switch (CGame::GetState())
			{
			case CGame::STATE_START:		// スタート時

				// 種類ごとの更新処理
				TypeProcess();

				break;

			case CGame::STATE_COUNTDOWN:	// カウントダウン

				// 種類ごとの更新処理
				TypeProcess();

				break;

			case CGame::STATE_PLAY:			// プレイ状態

// デバッグモード
#ifdef _DEBUG

// ポーズ以外のカメラ
#if 1
				if (CGame::GetPause() != nullptr)
				{ // ゲーム画面かつ、ポーズが NULL じゃない場合

					if (CGame::GetEdit() == false &&
						CGame::GetPause()->GetPause() == false)
					{ // プレイモードの場合

						// 種類ごとの更新処理
						TypeProcess();
					}
				}
#endif

// エディット状態のカメラ
#if 1
				if (CGame::GetEdit() == true)
				{ // エディット状態の場合

					// 操作処理
					Control();
				}

#endif

// リリースモード
#else

				if (CGame::GetPause() != nullptr)
				{ // ゲーム画面かつ、ポーズが NULL じゃない場合

					if (CGame::GetPause()->GetPause() == false)
					{ // プレイモードの場合

						// 種類ごとの更新処理
						TypeProcess();
					}
				}

#endif

				break;

			case CGame::STATE_GOAL:			// ゴール状態

				// 通常カメラにする
				SetType(CCamera::TYPE_NONE);

				// 種類ごとの更新処理
				TypeProcess();

				break;

			case CGame::STATE_FINISH:		// 終了状態

				// 通常カメラにする
				SetType(CCamera::TYPE_NONE);

				// 種類ごとの更新処理
				TypeProcess();

				break;

			case CGame::STATE_HITSTOP:		// ヒットストップ状態

				// カメラの処理無し(画面を止める)

				break;

			default:

				// 目的の注視点を設定する
				m_posR.x = 0.0f;
				m_posR.y = PLAYER_HEIGHT * 0.5f;
				m_posR.z = 0.0f;

				// 目的の視点を設定する
				m_posV.x = m_posR.x + sinf(m_rot.y) * -m_Dis;
				m_posV.y = m_posR.y;
				m_posV.z = m_posR.z + cosf(m_rot.y) * -m_Dis;

				break;
			}

// 一定状態以外の設定
#if 1
			if (m_type != TYPE_PRECEDENCE &&
				m_type != TYPE_STARTSTAGE &&
				m_type != TYPE_STARTRUN &&
				m_type != TYPE_STARTBACK)
			{ // 一定状態以外の場合

				// 位置の設定(正規化)処理
				PosSet();
			}
#endif
		}
		else
		{ // 上記以外

// デバッグモード
#ifdef _DEBUG

// ポーズ中のカメラ操作
#if 1
			if (m_bControl == true)
			{ // 捜査状況が true の場合

				// 操作処理
				Control();
			}
#endif

#endif
		}

		break;

	case CScene::MODE_RANKING:	// リザルト

		if (m_posV.x <= RANKING_STOP ||
			m_posV.x <= RANKING_STOP)
		{ // 位置が一定数以下だった場合

			// カメラを移動させる
			m_posV.x += RANKING_MOVE;
			m_posR.x += RANKING_MOVE;
		}

		break;

	default:					// 上記以外

		// 情報のリセット処理
		Reset();

		break;
	}

	// 向きの正規化処理
	RotNormalize(&m_rot.y);

#if 0

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_0) == true)
	{ // スペースキーを押した場合

		// カメラの情報のリセット処理
		Reset();
	}

#endif
}

//=======================
// カメラの設定処理
//=======================
void CCamera::Set(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

// デバッグ用
#ifdef _DEBUG

	if (CGame::GetEdit() == false)
	{ // プレイモードの場合

		// プロジェクションマトリックスを作成(透視投影)
		D3DXMatrixPerspectiveFovLH
		(
			&m_mtxProjection,									// プロジェクションマトリックス
			D3DXToRadian(ASPECT_RATIO),							// 視野角
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,			// 画面のアスペクト比
			DRAW_MIN_Z,											// Z値の最小値
			DRAW_MAX_Z											// Z値の最大値
		);
	}
	else
	{ // エディットモードの場合

		if (CGame::GetEditAll() != nullptr)
		{ // エディットオールが NULL じゃない場合

			if (CGame::GetEditAll()->GetStyle() == CEditAll::STYLE_BGOBJECT ||
				CGame::GetEditAll()->GetStyle() == CEditAll::STYLE_RANGEMOVE)
			{ // 背景オブジェクトと範囲移動の場合

				// プロジェクションマトリックスを作成(透視投影)
				D3DXMatrixPerspectiveFovLH
				(
					&m_mtxProjection,									// プロジェクションマトリックス
					D3DXToRadian(ASPECT_RATIO),							// 視野角
					(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,			// 画面のアスペクト比
					DRAW_MIN_Z,											// Z値の最小値
					DRAW_MAX_Z											// Z値の最大値
				);
			}
			else
			{ // 上記以外

				// プロジェクションマトリックスの作成(並行投影)
				D3DXMatrixOrthoLH
				(
					&m_mtxProjection,									// ビューマトリックス
					SCREEN_WIDTH,										// 画面の幅
					SCREEN_HEIGHT,										// 画面の高さ
					DRAW_MIN_Z,											// Z軸の最小値
					DRAW_MAX_Z											// Z軸の最大値
				);
			}
		}
	}

// リリース用
#else

	// プロジェクションマトリックスを作成(透視投影)
	D3DXMatrixPerspectiveFovLH
	(
		&m_mtxProjection,									// プロジェクションマトリックス
		D3DXToRadian(ASPECT_RATIO),							// 視野角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,			// 画面のアスペクト比
		DRAW_MIN_Z,											// Z値の最小値
		DRAW_MAX_Z											// Z値の最大値
	);

	//// プロジェクションマトリックスの作成(並行投影)
	//D3DXMatrixOrthoLH
	//(
	//	&m_mtxProjection,									// ビューマトリックス
	//	SCREEN_WIDTH,										// 画面の幅
	//	SCREEN_HEIGHT,										// 画面の高さ
	//	DRAW_MIN_Z,											// Z軸の最小値
	//	DRAW_MAX_Z											// Z軸の最大値
	//);

#endif

	// プロジェクトマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH
	(
		&m_mtxView,										// ビューマトリックス
		&m_posV,										// 視点
		&m_posR,										// 注視点
		&m_VecU											// 上方向ベクトル
	);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//=======================
// 種類の設定処理
//=======================
void CCamera::SetType(const TYPE type)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 向き
	CPlayer* pPlayer = CScene::GetPlayer();	// プレイヤーのポインタ

	// 種類を設定する
	m_type = type;

	switch (m_type)
	{
	case TYPE_NONE:		// 無し

		if (pPlayer != nullptr)
		{ // プレイヤーが NULL じゃない場合

			// プレイヤーの情報を取得する
			pos = pPlayer->GetPos();		// 位置
			rot = pPlayer->GetRot();		// 向き

			// 目的の注視点を設定する
			m_posRDest.x = pos.x;
			m_posRDest.z = pos.z;

			// 目的の視点を設定する
			m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * -m_Dis;
			m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * -m_Dis;

			// 目的の向きを設定する
			m_rotDest = 0.0f;
		}

		break;

	case TYPE_UPWARD:	// 上向きカメラ

		// 上向きカメラの設定処理
		SetUpward();

		break;

	case TYPE_PRECEDENCE:	// 先行追跡

		// 先行追跡カメラの設定処理
		SetPrecedence();

		break;

	case TYPE_ZOOMOUT:	// ズームアウト

		// ズームアウトの設定処理
		SetZoomOut();

		break;

	case TYPE_ZOOMIN:	// ズームイン

		// ズームインの設定処理
		SetZoomIn();

		break;

	case TYPE_STARTSTAGE:	// スタートカメラ(ステージ)

		// ステージ投影の設定処理
		SetStartStage();

		break;

	case TYPE_STARTRUN:		// スタートカメラ(走り姿)

		// 走り姿の設定処理
		SetStartRun();

		break;

	case TYPE_STARTBACK:	// スタートカメラ(戻り)

		// 戻りカメラの設定処理
		SetStartBack();

		break;

	case TYPE_LAND:			// 着地カメラ

		// 着地カメラの設定処理
		SetLand();

		break;

	case TYPE_WALLKICK:		// 壁キックカメラ

		// 壁キックの設定処理
		SetWallKick();

		break;

	case TYPE_BOOST:		// ブーストカメラ

		// ブーストカメラの設定処理
		SetBoost();

		break;
	}
}

//=======================
// 種類の取得処理
//=======================
CCamera::TYPE CCamera::GetType(void) const
{
	// 種類を返す
	return m_type;
}

//=======================
// 視点の位置の設定処理
//=======================
void CCamera::SetPosV(const D3DXVECTOR3 posV)
{
	// 視点の位置を設定する
	m_posV = posV;
}

//=======================
// 視点の位置の取得処理
//=======================
D3DXVECTOR3 CCamera::GetPosV(void) const
{
	// 視点の位置を返す
	return m_posV;
}

//=======================
// 注視点の位置の設定処理
//=======================
void CCamera::SetPosR(const D3DXVECTOR3 posR)
{
	// 注視点の位置を設定する
	m_posR = posR;
}

//=======================
// 注視点の位置の取得処理
//=======================
D3DXVECTOR3 CCamera::GetPosR(void) const
{
	// 注視点の位置を返す
	return m_posR;
}

//=======================
// 向きの設定処理
//=======================
void CCamera::SetRot(const D3DXVECTOR3& rot)
{
	// 向きの情報を設定する
	m_rot = rot;
}

//=======================
// 向きの取得処理
//=======================
D3DXVECTOR3 CCamera::GetRot(void) const
{
	// 向きの情報を返す
	return m_rot;
}

//=======================
// 距離の設定処理
//=======================
void CCamera::SetDistance(const float fDis)
{
	// 距離を設定する
	m_Dis = fDis;

	// 目的の視点を設定する
	m_posV.x = m_posR.x + sinf(m_rot.y) * -m_Dis;
	m_posV.z = m_posR.z + cosf(m_rot.y) * -m_Dis;
}

//=======================
// 距離の取得処理
//=======================
float CCamera::GetDistance(void) const
{
	// 距離を返す
	return m_Dis;
}

//=======================
// カメラの操作状況の切り替え処理
//=======================
void CCamera::ChangeControl(void)
{
	// 操作状況を切り替える
	m_bControl = m_bControl ? false : true;
}

//=======================
// カメラの情報のリセット処理
//=======================
void CCamera::Reset(void)
{
	// 全ての値をリセットする
	m_Dis = CAMERA_DISTANCE;					// 視点と注視点の距離
	m_posV = D3DXVECTOR3(0.0f, PLAYER_HEIGHT * 0.5f, -m_Dis);	// 視点の位置
	m_posVDest = m_posV;						// 目的の視点
	m_posR = D3DXVECTOR3(0.0f, PLAYER_HEIGHT * 0.5f, 0.0f);		// 注視点の位置
	m_posRDest = m_posR;						// 目的の注視点
	m_VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 上方向ベクトル
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	m_type = TYPE_NONE;							// 種類
	m_rotDest = m_rot.y;						// 目的の向き
	m_DisDest = CAMERA_DISTANCE;				// 目的の距離
	m_nSwingCount = 0;							// 揺れカメラのカウント
	m_bControl = false;							// 操作状況
}

//=======================
// カメラの回り込み処理
//=======================
void CCamera::Rotation(void)
{
	////カメラの目的の向きを設定する
	//m_rotDest = (pModelrot->y);

	////カメラの向きの差分を設定する
	//m_rotDiff = m_rotDest - m_rot.y;

	//if (m_rotDiff > D3DX_PI)
	//{//角度が3.14fより大きかった場合
	// //角度から1周分減らす
	//	m_rotDiff -= (2 * D3DX_PI);
	//}
	//else if (m_rotDiff < -D3DX_PI)
	//{//角度が-3.14fより小さかった場合
	// //角度に1周分加える
	//	m_rotDiff += (2 * D3DX_PI);
	//}

	////カメラの向きを補正する
	//m_rot.y += m_rotDiff * ROTATION_ROT;

	//if (m_rot.y > D3DX_PI)
	//{//角度が3.14fより大きかった場合
	// //角度から1周分減らす
	//	m_rot.y = -D3DX_PI;
	//}
	//else if (m_rot.y < -D3DX_PI)
	//{//角度が-3.14fより小さかった場合
	// //角度に1周分加える
	//	m_rot.y = D3DX_PI;
	//}
}

//=======================
// カメラの移動処理
//=======================
void CCamera::Move(void)
{
	if (CManager::GetInputKeyboard()->GetPress(DIK_UP) == true ||
		CManager::GetInputGamePad()->GetGameStickRYPress(0) > 0)
	{ // Wキーを押した場合

		if (CManager::GetInputKeyboard()->GetPress(DIK_RIGHT) == true ||
			CManager::GetInputGamePad()->GetGameStickRXPress(0) > 0)
		{ // Dキーを押した場合

			m_posR.x += sinf(D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
			m_posV.x += sinf(D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
			
			m_posR.y += cosf(D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
			m_posV.y += cosf(D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_LEFT) == true ||
			CManager::GetInputGamePad()->GetGameStickRXPress(0) < 0)
		{ // Aキーを押した場合

			m_posR.x += sinf(-D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
			m_posV.x += sinf(-D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;

			m_posR.y += cosf(-D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
			m_posV.y += cosf(-D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
		}
		else
		{ // Wキーのみを押した場合

			m_posR.x += sinf(m_rot.y) * POS_SPEED;
			m_posV.x += sinf(m_rot.y) * POS_SPEED;

			m_posR.y += cosf(m_rot.y) * POS_SPEED;
			m_posV.y += cosf(m_rot.y) * POS_SPEED;
		}
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_DOWN) == true ||
		CManager::GetInputGamePad()->GetGameStickRYPress(0) < 0)
	{ // Sキーを押した場合

		if (CManager::GetInputKeyboard()->GetPress(DIK_RIGHT) == true ||
			CManager::GetInputGamePad()->GetGameStickRXPress(0) > 0)
		{ // Dキーを押した場合

			m_posR.x += sinf(D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
			m_posV.x += sinf(D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;

			m_posR.y += cosf(D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
			m_posV.y += cosf(D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_LEFT) == true ||
			CManager::GetInputGamePad()->GetGameStickRXPress(0) < 0)
		{ // Aキーを押した場合

			m_posR.x += sinf(-D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
			m_posV.x += sinf(-D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;

			m_posR.y += cosf(-D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
			m_posV.y += cosf(-D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
		}
		else
		{ // Sキーのみを押した場合

			m_posR.x -= sinf(m_rot.y) * POS_SPEED;
			m_posV.x -= sinf(m_rot.y) * POS_SPEED;

			m_posR.y -= cosf(m_rot.y) * POS_SPEED;
			m_posV.y -= cosf(m_rot.y) * POS_SPEED;
		}
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_RIGHT) == true ||
		CManager::GetInputGamePad()->GetGameStickRXPress(0) > 0)
	{ // Dキーを押した場合

		m_posR.x += sinf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;
		m_posV.x += sinf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;

		m_posR.y += cosf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;
		m_posV.y += cosf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_LEFT) == true ||
		CManager::GetInputGamePad()->GetGameStickRXPress(0) < 0)
	{ // Aキーを押した場合

		m_posR.x -= sinf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;
		m_posV.x -= sinf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;

		m_posR.y += cosf(D3DX_PI * 0.5f - m_rot.y) * POS_SPEED;
		m_posV.y += cosf(D3DX_PI * 0.5f - m_rot.y) * POS_SPEED;
	}
}

//=======================
// カメラの視点操作処理
//=======================
void CCamera::MovePosV(void)
{
	if (CManager::GetInputKeyboard()->GetPress(DIK_Y) == true)
	{ // Yキーを押した場合
		
		// 視点を上に動かす
		m_posV.y += POS_SPEED;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_N) == true)
	{ // Nキーを押した場合

		// 視点を下に動かす
		m_posV.y -= POS_SPEED;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_Z) == true)
	{ // Zキーを押した場合

		// 視点を右に回す
		m_rot.y -= ROT_Y_SPEED;

		// カメラの視点を更新する
		m_posV.x = m_posR.x + sinf(m_rot.y) * -m_Dis;
		m_posV.z = m_posR.z + cosf(m_rot.y) * -m_Dis;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_C) == true)
	{ // Cキーを押した場合

		// 視点を左に回す
		m_rot.y += ROT_Y_SPEED;

		// カメラの視点を更新する
		m_posV.x = m_posR.x + sinf(m_rot.y) * -m_Dis;
		m_posV.z = m_posR.z + cosf(m_rot.y) * -m_Dis;
	}
}

//=======================
// カメラの注視点操作処理
//=======================
void CCamera::MovePosR(void)
{
	if (CManager::GetInputKeyboard()->GetPress(DIK_T) == true)
	{ // Tキーを押した場合

		// 注視点を上に動かす
		m_posR.y += POS_SPEED;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_B) == true)
	{ // Bキーを押した場合

		// 注視点を下に動かす
		m_posR.y -= POS_SPEED;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_Q) == true)
	{ // Qキーを押した場合

		// 注視点を右に回す
		m_rot.y -= ROT_Y_SPEED;

		// 注視点を更新する
		m_posR.x = m_posV.x + sinf(m_rot.y) * m_Dis;
		m_posR.z = m_posV.z + cosf(m_rot.y) * m_Dis;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_E) == true)
	{ // Eキーを押した場合

		// 注視点を左に回す
		m_rot.y += ROT_Y_SPEED;

		// 注視点を更新する
		m_posR.x = m_posV.x + sinf(m_rot.y) * m_Dis;
		m_posR.z = m_posV.z + cosf(m_rot.y) * m_Dis;
	}
}

//=======================
// カメラの距離操作処理
//=======================
void CCamera::MoveRange(void)
{
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{ // 左SHIFTキーを押している場合

		if (CManager::GetInputKeyboard()->GetPress(DIK_U) == true)
		{ // Uキーを押した場合

			// 距離を減算する
			m_Dis -= DIS_SPEED;

			if (m_Dis <= MIN_DISTANCE)
			{ // 100.0f以下の場合

				// 100.0fにする
				m_Dis = MIN_DISTANCE;
			}

			// カメラの視点を更新する
			m_posV.x = m_posR.x + sinf(m_rot.y) * -m_Dis;
			m_posV.z = m_posR.z + cosf(m_rot.y) * -m_Dis;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_M) == true)
		{ // Mキーを押した場合

			// 距離を減算する
			m_Dis += DIS_SPEED;

			if (m_Dis >= MAX_DISTANCE)
			{ // 800.0f以上の場合

				// 800.0fにする
				m_Dis = MAX_DISTANCE;
			}

			// カメラの視点を更新する
			m_posV.x = m_posR.x + sinf(m_rot.y) * -m_Dis;
			m_posV.z = m_posR.z + cosf(m_rot.y) * -m_Dis;
		}
	}
}

//=======================
// 操作処理
//=======================
void CCamera::Control(void)
{
	//カメラの移動処理
	Move();

	// カメラの視点操作処理
	MovePosV();

	// カメラの注視点操作処理
	MovePosR();

	// カメラの距離操作処理
	MoveRange();
}

//=======================
// カメラの位置の設定処理
//=======================
void CCamera::PosSet(void)
{
	// カメラの視点を更新する
	m_posV.x = m_posR.x + sinf(m_rot.y) * -m_Dis;
	m_posV.z = m_posR.z + cosf(m_rot.y) * -m_Dis;

	// カメラの注視点を更新する
	m_posR.x = m_posV.x + sinf(m_rot.y) * m_Dis;
	m_posR.z = m_posV.z + cosf(m_rot.y) * m_Dis;
}

//=======================
// カメラの種類の更新処理
//=======================
void CCamera::TypeProcess(void)
{
	switch (m_type)
	{
	case TYPE_NONE:		// 無し

		// 追跡処理
		Chase();

		break;

	case TYPE_UPWARD:

		// 上向きカメラの更新
		Upward();

		break;

	case TYPE_PRECEDENCE:	// 先行追跡カメラ

		// 先行追跡カメラの更新
		Precedence();

		break;

	case TYPE_LAND:			// 着地カメラ

		// 着地カメラの更新
		Land();

		break;

	case TYPE_ZOOMOUT:	// ズームアウト

		// カメラのズームアウト処理
		ZoomOut();

		break;

	case TYPE_ZOOMIN:	// ズームイン

		// カメラのズームイン処理
		ZoomIn();

		break;

	case TYPE_STARTSTAGE:	// スタートカメラ(ステージ)

		// カメラのステージ投影処理
		StartStage();

		break;

	case TYPE_STARTRUN:		// スタートカメラ(走り姿)

		// カメラの走り姿処理
		StartRun();

		break;

	case TYPE_STARTBACK:		// スタートカメラ(戻り)

		// カメラの戻り処理
		StartBack();

		break;

	case TYPE_WALLKICK:			// 壁キックカメラ

		// カメラの壁キックの処理
		WallKick();

		break;

	case TYPE_BOOST:			// ブーストカメラ

		// ブーストカメラの処理
		Boost();

		break;
	}
}

//=======================
// カメラの追跡処理
//=======================
void CCamera::Chase(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 rot;				// 向き
	CPlayer* pPlayer = CScene::GetPlayer();		// プレイヤーのポインタ
	m_DisDest = CAMERA_DISTANCE;	// 目的の距離

	// 距離の補正処理
	Correct(m_DisDest, &m_Dis, CORRECT_POSR);
	Correct(m_rotDest, &m_rot.y, CORRECT_POSR);

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		// プレイヤーの情報を取得する
		pos = pPlayer->GetPos();		// 位置
		rot = pPlayer->GetRot();		// 向き

		// 目的の注視点を設定する
		m_posRDest.x = pos.x;
		m_posRDest.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posRDest.z = pos.z;

		// 目的の視点を設定する
		m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * -m_Dis;
		m_posVDest.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * -m_Dis;

		// 注視点を補正
		m_posR.x += (m_posRDest.x - m_posR.x) * CORRECT_POSR;
		m_posR.y += (m_posRDest.y - m_posR.y) * CORRECT_POSR;
		m_posR.z += (m_posRDest.z - m_posR.z) * CORRECT_POSR;

		// 視点を補正
		m_posV.x += (m_posVDest.x - m_posV.x) * CORRECT_POSV;
		m_posV.y += (m_posVDest.y - m_posV.y) * CORRECT_POSR;
		m_posV.z += (m_posVDest.z - m_posV.z) * CORRECT_POSV;
	}
}

//=======================
// カメラの上向き処理
//=======================
void CCamera::Upward(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 向き
	CPlayer* pPlayer = CScene::GetPlayer();	// プレイヤーのポインタ

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		// プレイヤーの情報を取得する
		pos = pPlayer->GetPos();		// 位置
		rot = pPlayer->GetRot();		// 向き

		// 目的の注視点を設定する
		m_posRDest.x = pos.x;
		m_posRDest.z = pos.z;

		// 目的の視点を設定する
		m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * -m_Dis;
		m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * -m_Dis;

		// 注視点を補正
		m_posR.x += (pos.x - m_posR.x) * CORRECT_POSR;
		m_posR.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posR.z += (pos.z - m_posR.z) * CORRECT_POSR;

		// 視点を補正
		m_posV.y += (m_posVDest.y - m_posV.y) * CORRECT_POSV;
		m_posV.z += (m_posVDest.z - m_posV.z) * CORRECT_UPWARD_POSV;

		if (pPlayer->GetJump() == false)
		{ // 地上に着いた時

			// 種類を設定する
			SetType(TYPE_NONE);
		}
	}
}

//=======================
// カメラの先行追跡処理
//=======================
void CCamera::Precedence(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 向き
	CPlayer* pPlayer = CScene::GetPlayer();	// プレイヤーのポインタ

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		// プレイヤーの情報を取得する
		pos = pPlayer->GetPos();		// 位置
		rot = pPlayer->GetRot();		// 向き

		// 目標の注視点を設定する
		m_posRDest = pos;

		// 注視点を補正
		m_posR.x += (m_posRDest.x - m_posR.x) * CORRECT_POSR;
		m_posR.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posR.z += (m_posRDest.z - m_posR.z) * CORRECT_POSR;

		// 視点を補正
		m_posV.x += (m_posVDest.x - m_posV.x) * CORRECT_POSV;
		m_posV.y = m_posR.y;
		m_posV.z += (m_posVDest.z - m_posV.z) * CORRECT_POSV;

		if (pos.x >= m_posV.x)
		{ // 視点に到着したとき

			// 種類を設定する
			SetType(TYPE_NONE);
		}
	}
}

//=======================
// カメラのズームアウト処理
//=======================
void CCamera::ZoomOut(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 向き
	CPlayer* pPlayer = CScene::GetPlayer();	// プレイヤーのポインタ

	// 補正処理
	Correct(m_DisDest, &m_Dis, ZOOM_DIST_CORRECT);

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		// プレイヤーの情報を取得する
		pos = pPlayer->GetPos();		// 位置
		rot = pPlayer->GetRot();		// 向き

		// 目的の注視点を設定する
		m_posRDest.x = pos.x;
		m_posRDest.z = pos.z;

		// 目的の視点を設定する
		m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * -m_Dis;
		m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * -m_Dis;

		// 注視点を補正
		m_posR.x += (m_posRDest.x - m_posR.x) * CORRECT_POSR;
		m_posR.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posR.z += (m_posRDest.z - m_posR.z) * CORRECT_POSR;

		// 視点を補正
		m_posV.x += (m_posVDest.x - m_posV.x) * CORRECT_POSV;
		m_posV.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posV.z += (m_posVDest.z - m_posV.z) * CORRECT_POSV;
	}
}

//=======================
// カメラのズームイン処理
//=======================
void CCamera::ZoomIn(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 向き
	CPlayer* pPlayer = CScene::GetPlayer();	// プレイヤーのポインタ

	// 補正処理
	Correct(m_DisDest, &m_Dis, ZOOM_DIST_CORRECT);

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		// プレイヤーの情報を取得する
		pos = pPlayer->GetPos();		// 位置
		rot = pPlayer->GetRot();		// 向き

		// 目的の注視点を設定する
		m_posRDest.x = pos.x;
		m_posRDest.z = pos.z;

		// 目的の視点を設定する
		m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * -m_Dis;
		m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * -m_Dis;

		// 注視点を補正
		m_posR.x += (m_posRDest.x - m_posR.x) * CORRECT_POSR;
		m_posR.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posR.z += (m_posRDest.z - m_posR.z) * CORRECT_POSR;

		// 視点を補正
		m_posV.x += (m_posVDest.x - m_posV.x) * CORRECT_POSV;
		m_posV.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posV.z += (m_posVDest.z - m_posV.z) * CORRECT_POSV;
	}
}

//=======================
// ステージ投影の処理
//=======================
void CCamera::StartStage(void)
{
	// 距離の補正処理
	Correct(m_DisDest, &m_Dis, STAGE_CORRECT_POS);

	// 目的の注視点を設定する
	m_posRDest.x = m_posR.x + STAGE_CAMERA_MOVE;

	// 目的の視点を設定する
	m_posVDest.x = m_posV.x + STAGE_CAMERA_MOVE;

	// 注視点を補正
	m_posR.x += (m_posRDest.x - m_posR.x) * STAGE_CORRECT_POS;
	m_posR.y += (m_posRDest.y - m_posR.y) * STAGE_CORRECT_POS;
	m_posR.z += (m_posRDest.z - m_posR.z) * STAGE_CORRECT_POS;

	// 視点を補正
	m_posV.x += (m_posVDest.x - m_posV.x) * STAGE_CORRECT_POS;
	m_posV.y += (m_posVDest.y - m_posV.y) * STAGE_CORRECT_POS;
	m_posV.z += (m_posVDest.z - m_posV.z) * STAGE_CORRECT_POS;

	if (CManager::GetFile()->GetGameInfo().GoalPos.x <= m_posR.x)
	{ // ゴールの位置を通り過ぎた場合

		// 走り姿カメラに設定する
		SetType(TYPE_STARTRUN);
	}
}

//=======================
// 走り姿の処理
//=======================
void CCamera::StartRun(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos;		// プレイヤーの位置
	CPlayer* pPlayer = CScene::GetPlayer();		// プレイヤーの情報

	if (pPlayer != nullptr)
	{ // プレイヤーの情報がある場合

		// プレイヤーの位置を取得する
		pos = pPlayer->GetPos();

		// 目的の注視点を設定する
		m_posRDest.x = pos.x;

		// 目的の視点を設定する
		m_posVDest.x = pos.x + m_Dis;

		// 注視点を補正する
		m_posR.x = pos.x;

		// 視点を補正する
		m_posV.x = pos.x + m_Dis;

		if (CManager::GetFile() != nullptr)
		{ // ポインタが NULL じゃない場合

			if (pos.x >= CManager::GetFile()->GetGameInfo().StartPos.x)
			{ // 位置がスタート地点を過ぎた場合

				// 位置を補正する
				pos.x = 0.0f;

				// プレイヤーの位置を設定する
				pPlayer->SetPos(pos);

				// 戻りカメラに設定する
				SetType(TYPE_STARTBACK);
			}

		}
	}
}

//=======================
// 戻りの処理
//=======================
void CCamera::StartBack(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 向き
	CPlayer* pPlayer = CScene::GetPlayer();	// プレイヤーのポインタ
	m_DisDest = CAMERA_DISTANCE;			// 目的の距離

	// 向きの補正処理
	RotCorrect(0.0f, &m_rot.y, 0.02f);

	// 距離の補正処理
	Correct(m_DisDest, &m_Dis, BACK_CORRECT_MAGNI);

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		// プレイヤーの情報を取得する
		pos = pPlayer->GetPos();		// 位置
		rot = pPlayer->GetRot();		// 向き

		// 目的の注視点を設定する
		m_posRDest.x = pos.x;
		m_posRDest.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posRDest.z = pos.z;

		// 目的の視点を設定する
		m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * -m_Dis;
		m_posVDest.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * -m_Dis;

		// 注視点を補正
		m_posR.x += (m_posRDest.x - m_posR.x) * BACK_CORRECT_MAGNI;
		m_posR.y += (m_posRDest.y - m_posR.y) * BACK_CORRECT_MAGNI;
		m_posR.z += (m_posRDest.z - m_posR.z) * BACK_CORRECT_MAGNI;

		// 視点を補正
		m_posV.x += (m_posVDest.x - m_posV.x) * BACK_CORRECT_MAGNI;
		m_posV.y += (m_posVDest.y - m_posV.y) * BACK_CORRECT_MAGNI;
		m_posV.z += (m_posVDest.z - m_posV.z) * BACK_CORRECT_MAGNI;
	}

	if (m_Dis >= BACK_DECISION_DISTANCE)
	{ // 目的の距離に近づいた場合

		// 通常カメラに設定する
		SetType(TYPE_NONE);

		// ゲームの状態をカウントダウンに設定する
		CGame::SetState(CGame::STATE_COUNTDOWN);

		// カウントダウンの生成処理
		CCountdown::Create();
	}
}

//=======================
// カメラの着地処理
//=======================
void CCamera::Land(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 向き
	CPlayer* pPlayer = CScene::GetPlayer();	// プレイヤーのポインタ

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		// プレイヤーの情報を取得する
		pos = pPlayer->GetPos();		// 位置
		rot = pPlayer->GetRot();		// 向き

		if (m_nSwingCount % LANDING_PROCESS_COUNT == 0)
		{ // 揺れカウントが一定数ごとに

			if (m_nSwingCount % 2 == 0)
			{ // カウントが偶数の場合

				// 目的の注視点を設定する
				m_posRDest.y = pos.y + (PLAYER_HEIGHT * 0.5f) - (rand() % 10 + 25);
			}
			else
			{ // カウントが奇数の場合

				// 目的の注視点を設定する
				m_posRDest.y = pos.y + (PLAYER_HEIGHT * 0.5f) + (rand() % 10 + 25);
			}

			// 目的の注視点を設定する
			m_posRDest.x = pos.x;
			m_posRDest.z = pos.z;

			// 目的の視点を設定する
			m_posVDest.x = m_posRDest.x;
			m_posVDest.y = m_posRDest.y;
			m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * -m_Dis;
		}

		// 注視点を補正
		m_posR.x += (m_posRDest.x - m_posR.x) * LANDING_CORRECT_POSR;
		m_posR.y += (m_posRDest.y - m_posR.y) * LANDING_CORRECT_POSR;
		m_posR.z += (m_posRDest.z - m_posR.z) * LANDING_CORRECT_POSR;

		// 視点を補正
		m_posV.x += (m_posVDest.x - m_posV.x) * LANDING_CORRECT_POSV;
		m_posV.y += (m_posVDest.y - m_posV.y) * LANDING_CORRECT_POSV;
		m_posV.z += (m_posVDest.z - m_posV.z) * LANDING_CORRECT_POSV;
	}

	// 揺れカウントを加算する
	m_nSwingCount++;

	if (m_nSwingCount >= LANDING_CHANGE_COUNT)
	{ // カウント数が一定以上になった場合

		// 種類を設定する
		SetType(TYPE_NONE);
	}
}

//=======================
// 壁キックの処理
//=======================
void CCamera::WallKick(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 向き
	CPlayer* pPlayer = CScene::GetPlayer();	// プレイヤーのポインタ

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		// プレイヤーの情報を取得する
		pos = pPlayer->GetPos();		// 位置
		rot = pPlayer->GetRot();		// 向き

		if (m_nSwingCount % LANDING_PROCESS_COUNT == 0)
		{ // 揺れカウントが一定数ごとに

			if (m_nSwingCount % 2 == 0)
			{ // カウントが偶数の場合

				// 目的の距離を設定する
				m_DisDest *= 0.95f;
			}
			else
			{ // カウントが奇数の場合

				// 目的の距離を設定する
				m_DisDest *= 1.05f;
			}

			// 目的の注視点を設定する
			m_posRDest.x = pos.x;
			m_posRDest.y = pos.y + (PLAYER_HEIGHT * 0.5f);
			m_posRDest.z = pos.z;

			// 目的の視点を設定する
			m_posVDest.x = m_posRDest.x;
			m_posVDest.y = m_posRDest.y;
			m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * -m_Dis;
		}

		// 距離の補正処理
		Correct(m_DisDest, &m_Dis, 0.1f);

		// 注視点を補正
		m_posR.x += (m_posRDest.x - m_posR.x) * LANDING_CORRECT_POSR;
		m_posR.y += (m_posRDest.y - m_posR.y) * LANDING_CORRECT_POSR;
		m_posR.z += (m_posRDest.z - m_posR.z) * LANDING_CORRECT_POSR;

		// 視点を補正
		m_posV.x += (m_posVDest.x - m_posV.x) * LANDING_CORRECT_POSV;
		m_posV.y += (m_posVDest.y - m_posV.y) * LANDING_CORRECT_POSV;
		m_posV.z += (m_posVDest.z - m_posV.z) * LANDING_CORRECT_POSV;
	}

	// 揺れカウントを加算する
	m_nSwingCount++;

	if (m_nSwingCount >= LANDING_CHANGE_COUNT)
	{ // カウント数が一定以上になった場合

		// 種類を設定する
		SetType(TYPE_NONE);
	}
}

//=======================
// ブーストカメラの処理
//=======================
void CCamera::Boost(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 向き
	CPlayer* pPlayer = CScene::GetPlayer();	// プレイヤーのポインタ

	// 補正処理
	Correct(m_DisDest, &m_Dis, BOOST_DIST_CORRECT);

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		// プレイヤーの情報を取得する
		pos = pPlayer->GetPos();		// 位置
		rot = pPlayer->GetRot();		// 向き

		// 目的の注視点を設定する
		m_posRDest.x = pos.x;
		m_posRDest.z = pos.z;

		// 目的の視点を設定する
		m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * -m_Dis;
		m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * -m_Dis;

		// 注視点を補正
		m_posR.x += (m_posRDest.x - m_posR.x) * CORRECT_POSR;
		m_posR.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posR.z += (m_posRDest.z - m_posR.z) * CORRECT_POSR;

		// 視点を補正
		m_posV.x += (m_posVDest.x - m_posV.x) * CORRECT_POSV;
		m_posV.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posV.z += (m_posVDest.z - m_posV.z) * CORRECT_POSV;

		if (pPlayer->GetState() != CPlayer::STATE_BOOST)
		{ // ブースト状態以外

			// 通常状態にする
			SetType(TYPE_NONE);
		}
	}
}

//=======================
// 上向きカメラの設定処理
//=======================
void CCamera::SetUpward(void)
{
	// ローカルポインタを宣言
	CObject* pObj = nullptr;						// オブジェクトのポインタ
	CModel* pModel = nullptr;						// モデルのポインタ
	CPlayer* pPlayer = CScene::GetPlayer();			// プレイヤーのポインタ
	float fPosComp;									// 比較用変数
	float fPosTemp = m_posV.y + 800.0f;				// 保存用変数
	bool bPassing = false;							// IF文を通ったかどうか

	// 先頭のオブジェクトのポインタを取得する
	pObj = CObject::GetTop(CObject::PRIORITY_BLOCK);

	while (pObj != nullptr)
	{ // オブジェクトが NULL じゃない限り回す

		if (pObj->GetType() == CObject::TYPE_BLOCK ||
			pObj->GetType() == CObject::TYPE_OBSTACLE ||
			pObj->GetType() == CObject::TYPE_COLLPOLY)
		{ // ブロックまたは、障害物または、当たり判定ポリゴンの場合

			// モデル型にキャスト
			pModel = dynamic_cast<CModel*>(pObj);

			if (pModel != nullptr)
			{ // ポインタが NULL じゃない場合

				if (pModel->GetPos().x + pModel->GetFileData().vtxMax.x >= m_posV.x &&
					pModel->GetPos().x + pModel->GetFileData().vtxMin.x <= m_posV.x &&
					pModel->GetPos().y + pModel->GetFileData().vtxMax.y <= m_posV.y)
				{ // カメラの視点がブロックの横幅以内かつ、現在の視点よりも低い場合

					// 比較用変数に代入する
					fPosComp = pModel->GetPos().y + pModel->GetFileData().vtxMax.y;

					if (bPassing == false)
					{ // 最初だった場合

						// 保存変数に代入する
						fPosTemp = fPosComp;
					}
					else
					{ // 上記以外

						if (pPlayer->GetPos().y - fPosComp >= pPlayer->GetPos().y - fPosTemp)
						{ // 比較した位置の方がプレイヤーに近かった場合

							// 保存変数に代入する
							fPosTemp = fPosComp;
						}
					}
				}
			}
		}

		// 次のオブジェクトを代入する
		pObj = pObj->GetNext();
	}

	// 位置を設定する
	m_posVDest.y = fPosTemp;
}

//=======================
// 先行追跡カメラの設定処理
//=======================
void CCamera::SetPrecedence(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 向き
	CPlayer* pPlayer = CScene::GetPlayer();	// プレイヤーのポインタ

	if (pPlayer != nullptr &&
		pPlayer->GetRight() == true)
	{ // プレイヤーが NULL じゃないかつ、右向きだった場合

		// プレイヤーの情報を取得する
		pos = pPlayer->GetPos();		// 位置
		rot = pPlayer->GetRot();		// 向き

		// 目的の注視点を設定する
		m_posRDest.x = pos.x;
		m_posRDest.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posRDest.z = pos.z;

		// 目的の視点を補正
		m_posVDest.x = pos.x + sinf(rot.y) * PRECEDENCE_POS;
		m_posVDest.y = m_posRDest.y;
		m_posVDest.z = pos.z - m_Dis;
	}
	else
	{ // 上記以外

		// 通常状態にする
		m_type = TYPE_NONE;
	}
}

//=======================
// ズームアウトカメラの設定処理
//=======================
void CCamera::SetZoomOut(void)
{
	// 距離を伸ばす
	m_DisDest = ZOOMOUT_DISTANCE;
}

//=======================
// ズームインカメラの設定処理
//=======================
void CCamera::SetZoomIn(void)
{
	// 距離を伸ばす
	m_DisDest = ZOOMIN_DISTANCE;
}

//=======================
// ステージ投影カメラの設定処理
//=======================
void CCamera::SetStartStage(void)
{
	if (CManager::GetFile() != nullptr)
	{ // ゲームモードかつ、ファイルの情報があった場合

		// カメラの距離を設定する
		m_DisDest = STAGE_CAMERA_DISTANCE;

		{ // 位置の設定処理

			// ローカル変数宣言
			D3DXVECTOR3 posLeft = CManager::GetFile()->GetGameInfo().StageLeftPos;		// ステージの左端

			// 目的の視点を設定する
			m_posV = D3DXVECTOR3(posLeft.x, STAGE_CAMERA_HEIGHT, -m_DisDest);
			m_posVDest = D3DXVECTOR3(posLeft.x, STAGE_CAMERA_HEIGHT, -m_DisDest);

			// 目的の注視点を設定する
			m_posR = D3DXVECTOR3(m_posV.x + STAGE_CAMERA_SHIFT_WIDTH, m_posV.y, 0.0f);
			m_posRDest = D3DXVECTOR3(m_posV.x + STAGE_CAMERA_SHIFT_WIDTH, m_posV.y, 0.0f);
		}
	}
}

//=======================
// 走り姿カメラの設定処理
//=======================
void CCamera::SetStartRun(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 rot;				// 向き
	CPlayer* pPlayer = CScene::GetPlayer();	// プレイヤーのポインタ
	m_Dis = RUN_CAMERA_DISTANCE;	// カメラの距離
	m_rot.y = D3DX_PI * -0.5f;		// 向きを設定する

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		// プレイヤーの情報を取得する
		pos = pPlayer->GetPos();		// 位置
		rot = pPlayer->GetRot();		// 向き

		// 注視点を設定する
		m_posR.x = pos.x;
		m_posR.y = pos.y + 20.0f;
		m_posR.z = pos.z;

		// 視点を設定する
		m_posV.x = pos.x + m_Dis;
		m_posV.y = pos.y + PLAYER_HEIGHT;
		m_posV.z = pos.z;
	}
}

//=======================
// 戻りカメラの設定処理
//=======================
void CCamera::SetStartBack(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 向き
	CPlayer* pPlayer = CScene::GetPlayer();	// プレイヤーのポインタ

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		// プレイヤーの情報を取得する
		pos = pPlayer->GetPos();		// 位置
		rot = pPlayer->GetRot();		// 向き

		// 目的の注視点を設定する
		m_posRDest.x = pos.x;
		m_posRDest.z = pos.z;

		// 目的の視点を設定する
		m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * -m_Dis;
		m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * -m_Dis;
	}
}

//=======================
// 着地カメラの設定処理
//=======================
void CCamera::SetLand(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 向き
	CPlayer* pPlayer = CScene::GetPlayer();	// プレイヤーのポインタ

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		// プレイヤーの情報を取得する
		pos = pPlayer->GetPos();		// 位置
		rot = pPlayer->GetRot();		// 向き

		// 目的の注視点を設定する
		m_posRDest.x = pos.x;
		m_posRDest.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posRDest.z = pos.z;

		// 目的の視点を補正
		m_posVDest.x = pos.x + sinf(rot.y) * m_Dis;
		m_posVDest.y = pos.y + PLAYER_HEIGHT;
		m_posVDest.z = pos.z - m_Dis;
	}

	// 揺れカウントを初期化する
	m_nSwingCount = 0;
}

//=======================
// 壁キックの設定処理
//=======================
void CCamera::SetWallKick(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 向き
	CPlayer* pPlayer = CScene::GetPlayer();	// プレイヤーのポインタ

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		// プレイヤーの情報を取得する
		pos = pPlayer->GetPos();		// 位置
		rot = pPlayer->GetRot();		// 向き

		// 目的の注視点を設定する
		m_posRDest.x = pos.x;
		m_posRDest.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posRDest.z = pos.z;

		// 目的の視点を補正
		m_posVDest.x = pos.x + sinf(rot.y) * m_Dis;
		m_posVDest.y = pos.y + PLAYER_HEIGHT;
		m_posVDest.z = pos.z - m_Dis;
	}

	// 揺れカウントを初期化する
	m_nSwingCount = 0;
}

//=======================
// ブーストカメラの設定処理
//=======================
void CCamera::SetBoost(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 rot;				// 向き
	CPlayer* pPlayer = CScene::GetPlayer();	// プレイヤーのポインタ
	m_DisDest = BOOST_DISTANCE;		// 距離を伸ばす

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		// プレイヤーの情報を取得する
		pos = pPlayer->GetPos();		// 位置
		rot = pPlayer->GetRot();		// 向き

		// 目的の注視点を設定する
		m_posRDest.x = pos.x;
		m_posRDest.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posRDest.z = pos.z;

		// 目的の視点を設定する
		m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * -m_Dis;
		m_posVDest.y = pos.y + PLAYER_HEIGHT * 0.5f;
		m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * -m_Dis;
	}
}