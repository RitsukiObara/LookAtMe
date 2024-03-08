//===========================================
//
// マルチカメラのメイン処理[multi_camera.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "multi_camera.h"

//-------------------------------------------
// 無名名前空間
//-------------------------------------------
namespace
{
	const float VIEWPORT_LEFTUP_X[MAX_CAMERA] =			// カメラの左上X座標
	{
		0.0f,
		SCREEN_WIDTH * 0.5f,
		0.0f,
		SCREEN_WIDTH * 0.5f
	};
	const float VIEWPORT_LEFTUP_Y[MAX_CAMERA] =			// カメラの左上Y座標
	{
		0.0f,
		0.0f,
		SCREEN_HEIGHT * 0.5f,
		SCREEN_HEIGHT * 0.5f
	};
	const int CAMERA_WIDTH = (int)(SCREEN_WIDTH * 0.5f);		// カメラの幅
	const int CAMERA_HEIGHT = (int)(SCREEN_HEIGHT * 0.5f);		// カメラの高さ
}

//=======================
// コンストラクタ
//=======================
CMultiCamera::CMultiCamera()
{
	// 全ての情報をクリアする
	for (int nCnt = 0; nCnt < MAX_CAMERA; nCnt++)
	{
		m_aCamera[nCnt] = nullptr;		// カメラの情報
	}
}

//=======================
// デストラクタ
//=======================
CMultiCamera::~CMultiCamera()
{

}

//=======================
// カメラの初期化処理
//=======================
HRESULT CMultiCamera::Init(void)
{
	// ビューポートの変数を宣言
	D3DVIEWPORT9 viewport;

	// 全ての情報をクリアする
	for (int nCnt = 0; nCnt < MAX_CAMERA; nCnt++)
	{
		if (m_aCamera[nCnt] == nullptr)
		{ // カメラが NULL じゃない場合

			// カメラを生成する
			m_aCamera[nCnt] = new CCamera;

			// 初期化処理
			m_aCamera[nCnt]->Init();

			// ビューポートの設定処理
			viewport.X = (DWORD)VIEWPORT_LEFTUP_X[nCnt];	// 描画する画面の左上X座標
			viewport.Y = (DWORD)VIEWPORT_LEFTUP_Y[nCnt];	// 描画する画面の左上Y座標
			viewport.Width = CAMERA_WIDTH;					// 描画する画面の幅
			viewport.Height = CAMERA_HEIGHT;				// 描画する画面の高さ
			viewport.MinZ = 0.0f;							// クリップボリュームの最小値
			viewport.MaxZ = 1.0f;							// クリップボリュームの最大値

			// ビューポートの情報を設定する
			m_aCamera[nCnt]->SetViewport(viewport);
		}
	}

	// 成功を返す
	return S_OK;
}

//=======================
// カメラの終了処理
//=======================
void CMultiCamera::Uninit(void)
{
	// 全ての情報をクリアする
	for (int nCnt = 0; nCnt < MAX_CAMERA; nCnt++)
	{
		if (m_aCamera[nCnt] != nullptr)
		{ // カメラが NULL じゃない場合

			// 終了処理
			m_aCamera[nCnt]->Uninit();

			// カメラのメモリを解放する
			delete m_aCamera[nCnt];
			m_aCamera[nCnt] = nullptr;
		}
	}
}

//=======================
// カメラの更新処理
//=======================
void CMultiCamera::Update(void)
{
	for (int nCnt = 0; nCnt < MAX_CAMERA; nCnt++)
	{
		if (m_aCamera[nCnt] != nullptr)
		{ // カメラが NULL じゃない場合

			// カメラの更新処理
			m_aCamera[nCnt]->Update();
		}
	}
}

//=======================
// 設定処理
//=======================
void CMultiCamera::Set(const int nIdx)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// デバイスの取得
	D3DVIEWPORT9 viewport = m_aCamera[nIdx]->GetViewport();			// ビューポート情報を取得

	// ビューポートの設定
	pDevice->SetViewport(&viewport);

	// 設定処理
	m_aCamera[nIdx]->Set();
}

//=======================
// カメラの取得処理
//=======================
CCamera* CMultiCamera::GetCamera(const int nIdx)
{
	// カメラの情報を返す
	return m_aCamera[nIdx];
}