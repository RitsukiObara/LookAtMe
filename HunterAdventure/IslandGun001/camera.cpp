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
#include "camera.h"
#include "game.h"
#include "tutorial.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "useful.h"

#include "player.h"
#include "player_tutorial.h"
#include "light.h"
#include "boss.h"

//-------------------------------------------
// 定数定義
//-------------------------------------------
namespace
{
	// カメラ全体
	const float ASPECT_RATIO = 80.0f;			// 視野角
	const float MIN_DISTANCE = 50.0f;			// 距離の最小値
	const float MAX_DISTANCE = 8000.0f;			// 距離の最大値
	const float DRAW_MIN_Z = 10.0f;				// Z軸の最小値
	const float DRAW_MAX_Z = 80000.0f;			// Z軸の最大値

	// 向き関係
	const float ROT_Y_SPEED = 0.04f;			// Y軸の回転の速度
	const float ROTATION_SPEED = 0.05f;			// 回り込み処理を行う基準のモデルの速度
	const float ROTATION_ROT = 0.02f;			// カメラの角度の補正倍率

	// 位置・距離関係
	const float POS_SPEED = 30.0f;				// 移動速度
	const float DIS_SPEED = 16.0f;				// 距離の移動量
	const float CAMERA_DISTANCE = 600.0f;		// カメラの距離
	const float CORRECT_POSR = 0.22f;			// 注視点の補正倍率
	const float CORRECT_POSV = 0.20f;			// 視点の補正倍率

	// 特殊カメラ関係
	const float TITLE_HEIGHT = 4000.0f;			// タイトルカメラの高さ
	const float TITLE_DISTANCE = 8000.0f;		// タイトルカメラの距離
	const float TITLE_ADD_ROT = 0.005f;			// タイトルカメラの向きの加算数

	const float POSR_SHIFT_Y = 320.0f;			// 注視点のずらす幅(Y軸)
	const float POSR_SHIFT = 100.0f;			// 注視点のずらす幅

	const D3DXVECTOR3 START_POSV = D3DXVECTOR3(-5000.0f, 2500.0f, 15000.0f);		// スタートカメラの視点

	const float BOSS_CLOSER_RANGE = 3000.0f;	// ボス寄りカメラの距離
	const float BOSS_CLOSER_HEIGHT = 2200.0f;	// ボス寄りカメラの高さ
	const float BOSS_CLOSER_CORRECT = 0.08f;	// ボス寄りカメラの補正係数

	const float BOSS_HOWLING_HEIGHT_POSR = 1700.0f;		// ボス雄たけびカメラの注視点の高さ
	const float BOSS_HOWLING_RANGE_POSV = 3000.0f;		// ボス雄たけびカメラの視点の距離
	const float BOSS_HOWLING_ROT_SHIFT = 0.45f;			// ボス雄たけびカメラの向きのずらす量
	const float BOSS_HOWLING_CORRECT = 0.1f;			// ボス雄たけびカメラの補正係数

	const float BOSS_DESTROY_RANGE = 3500.0f;	// ボス寄りカメラの距離
	const float BOSS_DESTROY_HEIGHT = 1000.0f;	// ボス寄りカメラの高さ
	const float BOSS_DESTROY_CORRECT = 0.05f;	// ボス寄りカメラの補正係数

	const float GAMEOVER_ROT = D3DX_PI * 0.6f;			// ゲームオーバーカメラの向き
	const float GAMEOVER_INIT_DISTANCE = 800.0f;		// ゲームオーバーカメラの初期距離
	const float GAMEOVER_DISTANCE = 650.0f;				// ゲームオーバーカメラの距離
	const float GAMEOVER_SUB_DISTANCE = 2.0f;			// ゲームオーバーカメラの距離の減算量
	const float GAMEOVER_DEST_ROT = D3DX_PI * 0.85f;	// ゲームオーバーカメラの向きの目的地
	const float GAMEOVER_ROT_MAGNI = 0.05f;				// ゲームオーバーカメラの向きの倍率

	const float RESULT_HEIGHT = 4000.0f;				// リザルトカメラの高さ
	const float RESULT_DEPTH = -5000.0f;				// リザルトカメラの奥行
}

//=======================
// コンストラクタ
//=======================
CCamera::CCamera()
{
	// 全ての情報をクリアする
	m_posV = NONE_D3DXVECTOR3;						// 視点
	m_posVDest = m_posV;							// 目的の視点
	m_posR = NONE_D3DXVECTOR3;						// 注視点
	m_posRDest = m_posR;							// 目的の注視点
	m_VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// 上方向ベクトル
	m_rot = NONE_D3DXVECTOR3;						// 向き
	ZeroMemory(&m_viewport, sizeof(D3DVIEWPORT9));	// ビューポート
	m_type = TYPE_NONE;								// 種類
	m_Dis = 0.0f;									// 距離
	m_bControl = false;								// 操作状況

	m_vibrate.nextType = TYPE_NONE;	// 次のタイプ
	m_vibrate.nElapseCount = 0;		// 経過カウント
	m_vibrate.nSwingCount = 0;		// 揺れのカウント
	m_vibrate.nSwingRange = 0;		// 揺れの範囲
	m_vibrate.nFinishCount = 0;		// 終了カウント
	m_vibrate.fCorrect = 0.0f;		// 補正係数
	m_vibrate.bDown = false;		// 下状況
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
	m_viewport.X = (DWORD)0.0f;			// 描画する画面の左上X座標
	m_viewport.Y = (DWORD)0.0f;			// 描画する画面の左上Y座標
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
	// メモリを開放する
	delete this;
}

//=======================
// カメラの更新処理
//=======================
void CCamera::Update(void)
{
	D3DLIGHT9 light = CManager::Get()->GetLight()->GetLightCamera();
	D3DXVECTOR3 vecDir = NONE_D3DXVECTOR3;

	// ライトの方向を設定
	vecDir = m_posR - m_posV;

	// ベクトルを正規化する
	D3DXVec3Normalize(&vecDir, &vecDir);

	// ライトの方向を適用する
	light.Direction = vecDir;

	// ライトの情報を適用する
	CManager::Get()->GetLight()->SetLightCamera(light);

	switch (CManager::Get()->GetMode())
	{
	case CScene::MODE_TITLE:		// タイトル

		// タイトル処理
		Title();

		break;

	case CScene::MODE_TUTORIAL:		// チュートリアル

		// チュートリアルシーン処理
		Tutorial();

		break;

	case CScene::MODE_GAME:			// ゲームモード

		if (CGame::IsPause() == false)
		{ // ポーズ中以外の場合

			// 種類ごとの処理
			TypeProcess();
		}
		else
		{ // 上記以外

// デバッグモード
#ifdef _DEBUG

// ポーズ中のカメラ操作
#if 0
			if (m_bControl == true)
			{ // 捜査状況が true の場合

				// 操作処理
				Control();
			}
#endif

#endif
		}

		break;

	case CScene::MODE_RESULT:		// リザルト

		// リザルト処理
		Result();

		break;

	case CScene::MODE_RANKING:		// ランキング

		// ランキング処理
		Ranking();

		break;

	default:					// 上記以外

		// 情報のリセット処理
		Reset();

		break;
	}

#if 0

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_0) == true)
	{ // スペースキーを押した場合

		// カメラの情報のリセット処理
		Reset();
	}

#endif

	//// 目的の注視点を設定する
	//m_posRDest.x = pos.x;
	//m_posRDest.z = pos.z;

	//// 目的の視点を設定する
	//m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * -m_Dis;
	//m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * -m_Dis;

	//// 目的の向きを設定する
	//m_rotDest = 0.0f;
}

//=======================
// カメラの設定処理
//=======================
void CCamera::Set(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

// デバッグ用
#ifdef _DEBUG

		// プロジェクションマトリックスを作成(透視投影)
	D3DXMatrixPerspectiveFovLH
	(
		&m_mtxProjection,								// プロジェクションマトリックス
		D3DXToRadian(ASPECT_RATIO),						// 視野角
		(float)m_viewport.Width / (float)m_viewport.Height,	// 画面のアスペクト比
		DRAW_MIN_Z,										// Z値の最小値
		DRAW_MAX_Z										// Z値の最大値
	);

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
// ビューポートの設定処理
//=======================
void CCamera::SetViewport(const D3DVIEWPORT9& viewport)
{
	// ビューポート情報を設定する
	m_viewport = viewport;
}

//=======================
// ビューポートの設定処理
//=======================
D3DVIEWPORT9 CCamera::GetViewport(void) const
{
	// ビューポート情報を返す
	return m_viewport;
}

//=======================
// 振動関係の設定処理
//=======================
void CCamera::SetVibrate(const SVibrate vib)
{
	// 振動関係の情報を設定する
	m_vibrate = vib;
}

//=======================
// 振動関係の取得処理
//=======================
CCamera::SVibrate CCamera::GetVibrate(void) const
{
	// 振動関係の情報を返す
	return m_vibrate;
}

//=======================
// 種類の設定処理
//=======================
void CCamera::SetType(const TYPE type)
{
	// 種類を設定する
	m_type = type;

	if (m_type == TYPE_GAMEOVER)
	{ // ゲームオーバーになったとき

		// 向きを設定する
		m_rot.x = GAMEOVER_ROT;

		// 長さを設定する
		m_Dis = GAMEOVER_INIT_DISTANCE;
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
// カメラの操作状況の切り替え処理
//=======================
void CCamera::ChangeControl(void)
{
	// 操作状況を切り替える
	m_bControl = m_bControl ? false : true;
}

//=======================
// 生成処理
//=======================
CCamera* CCamera::Create(void)
{
	// ローカルオブジェクトを生成
	CCamera* pCamera = nullptr;		// カメラのインスタンスを生成

	if (pCamera == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pCamera = new CCamera;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pCamera != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pCamera->Init()))
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

	// カメラのポインタを返す
	return pCamera;
}

//=======================
// カメラの情報のリセット処理
//=======================
void CCamera::Reset(void)
{
	// 全ての値をリセットする
	m_Dis = CAMERA_DISTANCE;					// 視点と注視点の距離
	m_posV = D3DXVECTOR3(0.0f, 0.0f, -m_Dis);	// 視点の位置
	m_posVDest = m_posV;						// 目的の視点
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 注視点の位置
	m_posRDest = m_posR;						// 目的の注視点
	m_VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 上方向ベクトル
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	m_type = TYPE_NONE;							// 種類
	m_bControl = false;							// 操作状況

	m_vibrate.nextType = TYPE_NONE;	// 次のタイプ
	m_vibrate.nElapseCount = 0;		// 経過カウント
	m_vibrate.nSwingCount = 0;		// 揺れのカウント
	m_vibrate.nSwingRange = 0;		// 揺れの範囲
	m_vibrate.nFinishCount = 0;		// 終了カウント
	m_vibrate.fCorrect = 0.0f;		// 補正係数
	m_vibrate.bDown = false;		// 下状況
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
	if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_UP) == true ||
		CManager::Get()->GetInputGamePad()->GetGameStickRYPress(0) > 0)
	{ // Wキーを押した場合

		if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_RIGHT) == true ||
			CManager::Get()->GetInputGamePad()->GetGameStickRXPress(0) > 0)
		{ // Dキーを押した場合

			m_posR.x += sinf(D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
			m_posV.x += sinf(D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
			
			m_posR.z += cosf(D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
			m_posV.z += cosf(D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
		}
		else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_LEFT) == true ||
			CManager::Get()->GetInputGamePad()->GetGameStickRXPress(0) < 0)
		{ // Aキーを押した場合

			m_posR.x += sinf(-D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
			m_posV.x += sinf(-D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;

			m_posR.z += cosf(-D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
			m_posV.z += cosf(-D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
		}
		else
		{ // Wキーのみを押した場合

			m_posR.x += sinf(m_rot.y) * POS_SPEED;
			m_posV.x += sinf(m_rot.y) * POS_SPEED;

			m_posR.z += cosf(m_rot.y) * POS_SPEED;
			m_posV.z += cosf(m_rot.y) * POS_SPEED;
		}
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_DOWN) == true ||
		CManager::Get()->GetInputGamePad()->GetGameStickRYPress(0) < 0)
	{ // Sキーを押した場合

		if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_RIGHT) == true ||
			CManager::Get()->GetInputGamePad()->GetGameStickRXPress(0) > 0)
		{ // Dキーを押した場合

			m_posR.x += sinf(D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
			m_posV.x += sinf(D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;

			m_posR.z += cosf(D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
			m_posV.z += cosf(D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
		}
		else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_LEFT) == true ||
			CManager::Get()->GetInputGamePad()->GetGameStickRXPress(0) < 0)
		{ // Aキーを押した場合

			m_posR.x += sinf(-D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
			m_posV.x += sinf(-D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;

			m_posR.z += cosf(-D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
			m_posV.z += cosf(-D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
		}
		else
		{ // Sキーのみを押した場合

			m_posR.x -= sinf(m_rot.y) * POS_SPEED;
			m_posV.x -= sinf(m_rot.y) * POS_SPEED;

			m_posR.z -= cosf(m_rot.y) * POS_SPEED;
			m_posV.z -= cosf(m_rot.y) * POS_SPEED;
		}
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_RIGHT) == true ||
		CManager::Get()->GetInputGamePad()->GetGameStickRXPress(0) > 0)
	{ // Dキーを押した場合

		m_posR.x += sinf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;
		m_posV.x += sinf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;

		m_posR.z += cosf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;
		m_posV.z += cosf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_LEFT) == true ||
		CManager::Get()->GetInputGamePad()->GetGameStickRXPress(0) < 0)
	{ // Aキーを押した場合

		m_posR.x -= sinf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;
		m_posV.x -= sinf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;

		m_posR.z += cosf(D3DX_PI * 0.5f - m_rot.y) * POS_SPEED;
		m_posV.z += cosf(D3DX_PI * 0.5f - m_rot.y) * POS_SPEED;
	}
}

//=======================
// カメラの視点操作処理
//=======================
void CCamera::MovePosV(void)
{
	if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_Y) == true)
	{ // Yキーを押した場合
		
		// 視点を上に動かす
		m_posV.y += POS_SPEED;
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_N) == true)
	{ // Nキーを押した場合

		// 視点を下に動かす
		m_posV.y -= POS_SPEED;
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_Z) == true)
	{ // Zキーを押した場合

		// 視点を右に回す
		m_rot.y -= ROT_Y_SPEED;

		// カメラの視点を更新する
		m_posV.x = m_posR.x + sinf(m_rot.y) * -m_Dis;
		m_posV.z = m_posR.z + cosf(m_rot.y) * -m_Dis;
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_C) == true)
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
	if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_T) == true)
	{ // Tキーを押した場合

		// 注視点を上に動かす
		m_posR.y += POS_SPEED;
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_B) == true)
	{ // Bキーを押した場合

		// 注視点を下に動かす
		m_posR.y -= POS_SPEED;
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_Q) == true)
	{ // Qキーを押した場合

		// 注視点を右に回す
		m_rot.y -= ROT_Y_SPEED;

		// 注視点を更新する
		m_posR.x = m_posV.x + sinf(m_rot.y) * m_Dis;
		m_posR.z = m_posV.z + cosf(m_rot.y) * m_Dis;
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_E) == true)
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
	if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{ // 左SHIFTキーを押している場合

		if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_U) == true)
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
		else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_M) == true)
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
// マウスでのカメラ操作処理
//=======================
void CCamera::MouseControl(void)
{
	// マウスでの注視点操作処理
	MousePosR();

	// マウスでの視点操作処理
	MousePosV();

	// マウスでの視点・注視点操作処理
	MousePosMove();

	CManager::Get()->GetDebugProc()->Print("向き：%f %f %f\n", m_rot.x, m_rot.y, m_rot.z);
}

//=======================
// 視点操作処理
//=======================
void CCamera::MousePosV(void)
{
	// 移動量を取得する
	D3DXVECTOR3 move = CManager::Get()->GetInputMouse()->GetMove();

	if (CManager::Get()->GetInputMouse()->GetPress(CInputMouse::MOUSE_L) == true &&
		CManager::Get()->GetInputMouse()->GetPress(CInputMouse::MOUSE_R) == false)
	{ // 左クリックしていた場合

		// 視点のY軸を加算する
		m_posV.y += move.y * 5.0f;

		// 向きを加算する
		m_rot.y += move.x * 0.01f;

		// 向きの正規化
		useful::RotNormalize(&m_rot.y);

		// カメラの視点を更新する
		m_posV.x = m_posR.x + sinf(m_rot.y) * -m_Dis;
		m_posV.z = m_posR.z + cosf(m_rot.y) * -m_Dis;
	}
}

//=======================
// 注視点操作処理
//=======================
void CCamera::MousePosR(void)
{
	// 移動量を取得する
	D3DXVECTOR3 move = CManager::Get()->GetInputMouse()->GetMove();

	if (CManager::Get()->GetInputMouse()->GetPress(CInputMouse::MOUSE_R) == true &&
		CManager::Get()->GetInputMouse()->GetPress(CInputMouse::MOUSE_L) == false)
	{ // 右クリックしていた場合

		// 注視点のY軸を設定する
		m_posR.y += move.y * -5.0f;

		// 向きを加算する
		m_rot.y += move.x * 0.01f;

		// 向きの正規化
		useful::RotNormalize(&m_rot.y);

		// カメラの注視点を更新する
		m_posR.x = m_posV.x + sinf(m_rot.y) * m_Dis;
		m_posR.z = m_posV.z + cosf(m_rot.y) * m_Dis;
	}
}

//=======================
// 視点・注始点操作処理
//=======================
void CCamera::MousePosMove(void)
{
	// 移動量を取得する
	D3DXVECTOR3 move = CManager::Get()->GetInputMouse()->GetMove();

	if (CManager::Get()->GetInputMouse()->GetPress(CInputMouse::MOUSE_L) == true &&
		CManager::Get()->GetInputMouse()->GetPress(CInputMouse::MOUSE_R) == true)
	{ // 左クリックしていた場合

		// カメラの視点を更新する
		m_posV.x += sinf(m_rot.y - (D3DX_PI * 0.5f)) * (move.x * 5.0f);
		m_posV.z += cosf(m_rot.y) * (move.y * 5.0f);

		// カメラの注視点を更新する
		m_posR.x += sinf(m_rot.y - (D3DX_PI * 0.5f)) * (move.x * 5.0f);
		m_posR.z += cosf(m_rot.y) * (move.y * 5.0f);
	}
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
// カメラの種類ごとの処理
//=======================
void CCamera::TypeProcess(void)
{
	switch (m_type)
	{
	case CCamera::TYPE_NONE:			// 通常

		// 追跡処理
		Chase();

		break;

	case CCamera::TYPE_VIBRATE:			// 振動

		// 振動カメラ処理
		Vibrate();

		break;

	case CCamera::TYPE_START:			// スタート状態

		// スタート処理
		Start();

		break;

	case CCamera::TYPE_PLAYERAPPEAR:	// プレイヤー登場状態

		// プレイヤー登場処理
		PlayerAppear();

		break;

	case CCamera::TYPE_BOSSCLOSER:		// ボス寄り

		// ボス近づき処理
		BossCloser();

		break;

	case CCamera::TYPE_BOSSHOWLING:		// ボス雄たけび状態

		// ボス雄たけび処理
		BossHowling();

		break;

	case CCamera::TYPE_BOSSDESTROY:		// ボス死亡状態

		// ボスの死亡状態
		BossDestroy();

		break;

	case CCamera::TYPE_GAMEOVER:		// ゲームオーバー状態

		// ゲームオーバー処理
		GameOver();

		break;

	default:

		// 停止
		assert(false);

		break;
	}
}

//=======================
// タイトルカメラ
//=======================
void CCamera::Title(void)
{
	// 注視点を設定する
	m_posR = NONE_D3DXVECTOR3;

	// 視点を設定する
	m_posV.x = m_posR.x + sinf(m_rot.y) * TITLE_DISTANCE;
	m_posV.y = m_posR.y + TITLE_HEIGHT;
	m_posV.z = m_posR.z + cosf(m_rot.y) * TITLE_DISTANCE;

	// 向きを加算する
	m_rot.y += TITLE_ADD_ROT;

	// 向きの正規化
	useful::RotNormalize(&m_rot.y);
}

//=======================
// カメラの追跡処理
//=======================
void CCamera::Chase(void)
{
	// ローカル変数宣言
	CPlayer* pPlayer = CGame::GetPlayer();	// プレイヤーのポインタ

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		// プレイヤーの情報を取得する
		D3DXVECTOR3 pos = pPlayer->GetPos();			// 位置

		// 目的の注視点を設定する
		m_posRDest.x = pos.x + sinf(m_rot.y) * POSR_SHIFT;
		m_posRDest.y = pos.y + POSR_SHIFT_Y;
		m_posRDest.z = pos.z + cosf(m_rot.y) * POSR_SHIFT;

		// 目的の視点を設定する
		m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * sinf(m_rot.x) * -m_Dis;
		m_posVDest.y = m_posRDest.y + cosf(m_rot.x) * -m_Dis;
		m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * sinf(m_rot.x) * -m_Dis;

		// 注視点を補正
		m_posR.x += (m_posRDest.x - m_posR.x) * CORRECT_POSR;
		m_posR.y += (m_posRDest.y - m_posR.y) * CORRECT_POSR;
		m_posR.z += (m_posRDest.z - m_posR.z) * CORRECT_POSR;

		// 視点を補正
		m_posV.x += (m_posVDest.x - m_posV.x) * CORRECT_POSV;
		m_posV.y += (m_posVDest.y - m_posV.y) * CORRECT_POSV;
		m_posV.z += (m_posVDest.z - m_posV.z) * CORRECT_POSV;
	}
}

//=======================
// チュートリアルシーン処理
//=======================
void CCamera::Tutorial(void)
{
	// ローカル変数宣言
	CTutorialPlayer* pPlayer = CTutorial::GetPlayer();	// プレイヤーのポインタ

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		// プレイヤーの情報を取得する
		D3DXVECTOR3 pos = pPlayer->GetPos();			// 位置

		// 目的の注視点を設定する
		m_posRDest.x = pos.x + sinf(m_rot.y) * POSR_SHIFT;
		m_posRDest.y = pos.y + POSR_SHIFT_Y;
		m_posRDest.z = pos.z + cosf(m_rot.y) * POSR_SHIFT;

		// 目的の視点を設定する
		m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * sinf(m_rot.x) * -m_Dis;
		m_posVDest.y = m_posRDest.y + cosf(m_rot.x) * -m_Dis;
		m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * sinf(m_rot.x) * -m_Dis;

		// 注視点を補正
		m_posR.x += (m_posRDest.x - m_posR.x) * CORRECT_POSR;
		m_posR.y += (m_posRDest.y - m_posR.y) * CORRECT_POSR;
		m_posR.z += (m_posRDest.z - m_posR.z) * CORRECT_POSR;

		// 視点を補正
		m_posV.x += (m_posVDest.x - m_posV.x) * CORRECT_POSV;
		m_posV.y += (m_posVDest.y - m_posV.y) * CORRECT_POSV;
		m_posV.z += (m_posVDest.z - m_posV.z) * CORRECT_POSV;
	}
}

//=======================
// カメラの振動処理
//=======================
void CCamera::Vibrate(void)
{
	if (m_vibrate.nElapseCount % m_vibrate.nSwingCount == 0)
	{ // 揺れカウントが一定数ごとに

		float fVib = (float)(rand() % m_vibrate.nSwingRange + (m_vibrate.nSwingRange * 0.5f));

		if (m_vibrate.bDown == true)
		{ // 下状況が true の場合

			// 目的の注視点を設定する
			m_posRDest.y -= fVib;
		}
		else
		{ // 上記以外

			// 目的の注視点を設定する
			m_posRDest.y += fVib;
		}

		// 下状況を切り替える
		m_vibrate.bDown = !m_vibrate.bDown;
	}

	// 注視点を補正
	m_posR.x += (m_posRDest.x - m_posR.x) * m_vibrate.fCorrect;
	m_posR.y += (m_posRDest.y - m_posR.y) * m_vibrate.fCorrect;
	m_posR.z += (m_posRDest.z - m_posR.z) * m_vibrate.fCorrect;

	// 視点を補正
	m_posV.x += (m_posVDest.x - m_posV.x) * m_vibrate.fCorrect;
	m_posV.y += (m_posVDest.y - m_posV.y) * m_vibrate.fCorrect;
	m_posV.z += (m_posVDest.z - m_posV.z) * m_vibrate.fCorrect;

	// 経過カウントを加算する
	m_vibrate.nElapseCount++;

	if (m_vibrate.nElapseCount >= m_vibrate.nFinishCount)
	{ // 終了カウントになった場合

		// 経過カウントを0にする
		m_vibrate.nElapseCount = 0;

		// 次の状態に設定する
		SetType(m_vibrate.nextType);
	}
}

//=======================
// スタート処理
//=======================
void CCamera::Start(void)
{
	// ローカル変数宣言
	CPlayer* pPlayer = CGame::GetPlayer();	// プレイヤーのポインタ

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		// 目的の注視点を設定する
		m_posRDest = pPlayer->GetPos();

		// 目的の視点を設定する
		m_posVDest = START_POSV;

		// 注視点を設定
		m_posR = m_posRDest;

		// 視点を設定
		m_posV = m_posVDest;
	}
}

//=======================
// プレイヤー登場処理
//=======================
void CCamera::PlayerAppear(void)
{
	// ローカル変数宣言
	CPlayer* pPlayer = CGame::GetPlayer();	// プレイヤーのポインタ

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		// 位置を取得する
		D3DXVECTOR3 pos = pPlayer->GetPos();

		// 目的の注視点を設定する
		m_posRDest.x = pos.x;
		m_posRDest.y = pos.y + POSR_SHIFT_Y;
		m_posRDest.z = pos.z;

		// 注視点を設定
		m_posR = m_posRDest;
	}
}

//=======================
// ボス寄り処理
//=======================
void CCamera::BossCloser(void)
{
	// ローカル変数宣言
	CBoss* pBoss = nullptr;		// ボスの情報
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 向き

	if (CBoss::GetList().IsEmpty() == false)
	{ // リストが 空欄じゃない場合

		// ボスの情報を取得する
		pBoss = CBoss::GetList().GetTop();
	}

	if (pBoss != nullptr)
	{ // ボスが NULL じゃない場合

		// 位置と向きを取得する
		pos = pBoss->GetPos();
		rot = pBoss->GetRot();

		// 目的の注視点を設定する
		m_posRDest.x = pos.x;
		m_posRDest.y = pos.y + BOSS_CLOSER_HEIGHT;
		m_posRDest.z = pos.z;

		// 目的の視点を設定する
		m_posVDest.x = m_posRDest.x + sinf(rot.y) * BOSS_CLOSER_RANGE;
		m_posVDest.y = m_posRDest.y;
		m_posVDest.z = m_posRDest.z + cosf(rot.y) * BOSS_CLOSER_RANGE;

		// 注視点を補正
		m_posR.x += (m_posRDest.x - m_posR.x) * BOSS_CLOSER_CORRECT;
		m_posR.y += (m_posRDest.y - m_posR.y) * BOSS_CLOSER_CORRECT;
		m_posR.z += (m_posRDest.z - m_posR.z) * BOSS_CLOSER_CORRECT;

		// 視点を補正
		m_posV.x += (m_posVDest.x - m_posV.x) * BOSS_CLOSER_CORRECT;
		m_posV.y += (m_posVDest.y - m_posV.y) * BOSS_CLOSER_CORRECT;
		m_posV.z += (m_posVDest.z - m_posV.z) * BOSS_CLOSER_CORRECT;
	}
}

//=======================
// ボス雄たけび処理
//=======================
void CCamera::BossHowling(void)
{
	// ローカル変数宣言
	CBoss* pBoss = nullptr;		// ボスの情報
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 向き

	if (CBoss::GetList().IsEmpty() == false)
	{ // リストが 空欄じゃない場合

		// ボスの情報を取得する
		pBoss = CBoss::GetList().GetTop();
	}

	if (pBoss != nullptr)
	{ // ボスが NULL じゃない場合

		// 位置と向きを取得する
		pos = pBoss->GetPos();
		rot = pBoss->GetRot();

		// 目的の注視点を設定する
		m_posRDest.x = pos.x;
		m_posRDest.y = pos.y + BOSS_HOWLING_HEIGHT_POSR;
		m_posRDest.z = pos.z;

		// 目的の視点を設定する
		m_posVDest.x = m_posRDest.x + sinf(rot.y + BOSS_HOWLING_ROT_SHIFT) * BOSS_HOWLING_RANGE_POSV;
		m_posVDest.y = pos.y;
		m_posVDest.z = m_posRDest.z + cosf(rot.y + BOSS_HOWLING_ROT_SHIFT) * BOSS_HOWLING_RANGE_POSV;

		// 注視点を補正
		m_posR.x += (m_posRDest.x - m_posR.x) * BOSS_HOWLING_CORRECT;
		m_posR.y += (m_posRDest.y - m_posR.y) * BOSS_HOWLING_CORRECT;
		m_posR.z += (m_posRDest.z - m_posR.z) * BOSS_HOWLING_CORRECT;

		// 視点を補正
		m_posV.x += (m_posVDest.x - m_posV.x) * BOSS_HOWLING_CORRECT;
		m_posV.y += (m_posVDest.y - m_posV.y) * BOSS_HOWLING_CORRECT;
		m_posV.z += (m_posVDest.z - m_posV.z) * BOSS_HOWLING_CORRECT;
	}
}

//=======================
// ボスの死亡状態
//=======================
void CCamera::BossDestroy(void)
{
	// ローカル変数宣言
	CBoss* pBoss = nullptr;		// ボスの情報
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 向き

	if (CBoss::GetList().IsEmpty() == false)
	{ // リストが 空欄じゃない場合

		// ボスの情報を取得する
		pBoss = CBoss::GetList().GetTop();
	}

	if (pBoss != nullptr)
	{ // ボスが NULL じゃない場合

		// 位置と向きを取得する
		pos = pBoss->GetPos();
		rot = pBoss->GetRot();

		// 目的の注視点を設定する
		m_posRDest.x = pos.x;
		m_posRDest.y = pos.y + BOSS_DESTROY_HEIGHT;
		m_posRDest.z = pos.z;

		// 目的の視点を設定する
		m_posVDest.x = m_posRDest.x + sinf(rot.y) * BOSS_DESTROY_RANGE;
		m_posVDest.y = m_posRDest.y;
		m_posVDest.z = m_posRDest.z + cosf(rot.y) * BOSS_DESTROY_RANGE;

		// 注視点を補正
		m_posR.x += (m_posRDest.x - m_posR.x) * BOSS_DESTROY_CORRECT;
		m_posR.y += (m_posRDest.y - m_posR.y) * BOSS_DESTROY_CORRECT;
		m_posR.z += (m_posRDest.z - m_posR.z) * BOSS_DESTROY_CORRECT;

		// 視点を補正
		m_posV.x += (m_posVDest.x - m_posV.x) * BOSS_DESTROY_CORRECT;
		m_posV.y += (m_posVDest.y - m_posV.y) * BOSS_DESTROY_CORRECT;
		m_posV.z += (m_posVDest.z - m_posV.z) * BOSS_DESTROY_CORRECT;
	}
}

//=======================
// ゲームオーバー処理
//=======================
void CCamera::GameOver(void)
{
	// ローカル変数宣言
	CPlayer* pPlayer = CGame::GetPlayer();	// プレイヤーのポインタ

	if (pPlayer != nullptr)
	{ // プレイヤーが NULL じゃない場合

		// プレイヤーの情報を取得する
		D3DXVECTOR3 pos = pPlayer->GetPos();			// 位置
		D3DXVECTOR3 rot = pPlayer->GetRot();			// 向き

		// 注視点を補正
		m_posR.x = pos.x;
		m_posR.y = pos.y;
		m_posR.z = pos.z;

		// 視点を補正
		m_posV.x = m_posR.x + sinf(rot.y) * sinf(m_rot.x) * m_Dis;
		m_posV.y = m_posR.y + cosf(m_rot.x) * -m_Dis;
		m_posV.z = m_posR.z + cosf(rot.y) * sinf(m_rot.x) * m_Dis;

		// 均等な補正処理
		useful::FrameCorrect(GAMEOVER_DISTANCE, &m_Dis, GAMEOVER_SUB_DISTANCE);

		// 向きの補正処理
		useful::Correct(GAMEOVER_DEST_ROT, &m_rot.x, GAMEOVER_ROT_MAGNI);
	}
}

//=======================
// リザルトカメラ
//=======================
void CCamera::Result(void)
{
	// 注視点を設定する
	m_posR = NONE_D3DXVECTOR3;

	// 視点を設定する
	m_posV.x = m_posR.x;
	m_posV.y = m_posR.y + RESULT_HEIGHT;
	m_posV.z = m_posR.z + RESULT_DEPTH;
}

//=======================
// ランキングカメラ
//=======================
void CCamera::Ranking(void)
{
	// 注視点を設定する
	m_posR = NONE_D3DXVECTOR3;

	// 視点を設定する
	m_posV.x = m_posR.x;
	m_posV.y = m_posR.y + RESULT_HEIGHT;
	m_posV.z = m_posR.z + RESULT_DEPTH;
}