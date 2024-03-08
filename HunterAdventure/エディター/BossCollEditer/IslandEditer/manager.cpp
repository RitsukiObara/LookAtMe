//============================================
//
// マネージャーのメイン処理[manager.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "file.h"
#include "debugproc.h"
#include "texture.h"
#include "objectX.h"
#include "camera.h"
#include "light.h"
#include "fade.h"

//--------------------------------------------
// 静的メンバ変数宣言
//--------------------------------------------
CManager* CManager::m_pManager = nullptr;			// マネージャーの情報

//=========================================
// コンストラクタ
//=========================================
CManager::CManager()
{

}

//=========================================
// デストラクタ
//=========================================
CManager::~CManager()
{

}

//=========================================
//初期化処理
//=========================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	if (m_pRenderer == nullptr)
	{ // レンダラーへのポインタが NULL の場合

		// レンダラーの生成処理
		m_pRenderer = CRenderer::Create(hWnd, bWindow);
	}
	else
	{ // ポインタが使われていた場合

		// 停止
		assert(false);
	}

	if (m_pFile == nullptr)
	{ // ファイルへのポインタが nullptr の場合

		// ファイルの生成処理
		m_pFile = CFile::Create();
	}
	else
	{ // ポインタが使われていた場合

		// 停止
		assert(false);
	}

	if (m_pDebugProc == nullptr)
	{ // デバッグ表示へのポインタが nullptr の場合

		// デバッグ表示の生成処理
		m_pDebugProc = CDebugProc::Create();
	}
	else
	{ // ポインタが使われていた場合

		// 停止
		assert(false);
	}

	if (m_pInputKeyboard == nullptr)
	{ // キーボードへのポインタが NULL の場合

		// キーボードの生成処理
		m_pInputKeyboard = CInputKeyboard::Create(hInstance, hWnd);
	}
	else
	{ // ポインタが使われていた場合

		// 停止
		assert(false);
	}

	if (m_pInputGamePad == nullptr)
	{ // ゲームパッドへのポインタが NULL の場合

		// ゲームパッドの生成処理
		m_pInputGamePad = CInputGamePad::Create(hInstance);
	}
	else
	{ // ポインタが使われていた場合

		// 停止
		assert(false);
	}

	if (m_pInputMouse == nullptr)
	{ // マウスへのポインタが NULL の場合

		// マウスの生成処理
		m_pInputMouse = CInputMouse::Create(hInstance, hWnd);
	}
	else
	{ // ポインタが使われていた場合

		// 停止
		assert(false);
	}

	if (m_pSound == nullptr)
	{ // サウンドへのポインタが NULL の場合

		// サウンドの生成処理
		m_pSound = CSound::Create(hWnd);
	}
	else
	{ // ポインタが使われていた場合

		// 停止
		assert(false);
	}

	if (m_pCamera == nullptr)
	{ // カメラへのポインタが NULL の場合

		// カメラの生成処理
		m_pCamera = CCamera::Create();
	}
	else
	{ // ポインタが使われていた場合

		// 停止
		assert(false);
	}

	if (m_pLight == nullptr)
	{ // ライトへのポインタが NULL の場合

		// ライトの生成処理
		m_pLight = CLight::Create();
	}
	else
	{ // ポインタが使われていた場合

		// 停止
		assert(false);
	}

	if (m_pTexture == nullptr)
	{ // テクスチャへのポインタが NULL の場合

		// テクスチャの生成処理
		m_pTexture = CTexture::Create();
	}
	else
	{ // ポインタが使われていた場合

		// 停止
		assert(false);
	}

	if (m_pXFile == nullptr)
	{ // Xファイルへのポインタが NULL の場合

		// Xファイルの生成処理
		m_pXFile = CXFile::Create();
	}
	else
	{ // 上記以外

		// 停止
		assert(false);
	}

	// テクスチャの読み込み処理
	if (FAILED(TexLoad()))
	{ // 読み込みに失敗した場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	if (m_pFade == nullptr)
	{ // フェードへのポインタが nullptr の場合

#ifdef _DEBUG

		// フェードのメモリを確保する
		m_pFade = m_pFade->Create(CScene::MODE_GAME);
#else

		// シーンのメモリを確保する
		m_pFade = m_pFade->Create(CScene::MODE_GAME);
#endif 

	}
	else
	{ // ポインタが使われていた場合

		// 停止
		assert(false);
	}

	// 成功を返す
	return S_OK;
}

//=============================================
//終了処理
//=============================================
void CManager::Uninit(void)
{
	if (m_pSound != nullptr)
	{ // サウンドのメモリが確保されていた場合

		// サウンドの停止
		m_pSound->Stop();
	}

	// テクスチャの破棄処理
	TexUnload();

	if (m_pRenderer != nullptr)
	{ // レンダラーが NULL じゃない場合

		// 終了処理
		m_pRenderer->Uninit();
		m_pRenderer = nullptr;
	}

	if (m_pInputKeyboard != nullptr)
	{ // キーボードが NULL じゃない場合

		// 終了処理
		m_pInputKeyboard->Uninit();
		m_pInputKeyboard = nullptr;
	}

	if (m_pInputGamePad != nullptr)
	{ // ゲームパッドが NULL じゃない場合

		// 終了処理
		m_pInputGamePad->Uninit();
		m_pInputGamePad = nullptr;
	}

	if (m_pInputMouse != nullptr)
	{ // マウスが NULL じゃない場合

		// 終了処理
		m_pInputMouse->Uninit();
		m_pInputMouse = nullptr;
	}

	if (m_pSound != nullptr)
	{ // サウンドが NULL じゃない場合

		// 終了処理
		m_pSound->Uninit();
		m_pSound = nullptr;
	}

	if (m_pFile != nullptr)
	{ // ファイルが NULL じゃない場合

		// 終了処理
		m_pFile->Uninit();
		m_pFile = nullptr;
	}

	if (m_pDebugProc != nullptr)
	{ // デバッグ表示が NULL じゃない場合

		// 終了処理
		m_pDebugProc->Uninit();
		m_pDebugProc = nullptr;
	}

	if (m_pCamera != nullptr)
	{ // カメラが NULL じゃない場合

		// 終了処理
		m_pCamera->Uninit();
		m_pCamera = nullptr;
	}

	if (m_pLight != nullptr)
	{ // ライトが NULL じゃない場合

		// 終了処理
		m_pLight->Uninit();
		m_pLight = nullptr;
	}

	if (m_pTexture != nullptr)
	{ // テクスチャが NULL じゃない場合

		// テクスチャの破棄処理
		m_pTexture->Unload();
		m_pTexture = nullptr;
	}

	if (m_pXFile != nullptr)
	{ // Xファイルが NULL じゃない場合

		// Xファイルの破棄処理
		m_pXFile->Unload();
		m_pXFile = nullptr;
	}

	if (m_pFade != nullptr)
	{ // フェードが NULL じゃない場合

		// フェードの破棄処理
		m_pFade->Uninit();
		m_pFade = nullptr;
	}

	if (m_pScene != nullptr)
	{ // シーンが NULL じゃない場合

		// シーンを破棄する
		m_pScene->Uninit();
		m_pScene = nullptr;
	}

	// マネージャーのメモリを解放する
	delete m_pManager;
	m_pManager = nullptr;
}

//======================================
//更新処理
//======================================
void CManager::Update(void)
{
	if (m_pFade != nullptr)
	{ // フェードが NULL じゃない場合

		// フェードの更新
		m_pFade->Update();
	}

	if (m_pInputKeyboard != nullptr)
	{ // キーボードが NULL じゃない場合

		// キーボードの更新
		m_pInputKeyboard->Update();
	}

	if (m_pInputGamePad != nullptr)
	{ // ゲームパッドが NULL じゃない場合

		// ゲームパッドの更新
		m_pInputGamePad->Update();
	}

	if (m_pInputMouse != nullptr)
	{ // マウスが NULL じゃない場合

		// マウスの更新
		m_pInputMouse->Update();
	}

	if (m_pCamera != nullptr)
	{ // カメラが NULL じゃない場合

		// カメラの更新
		m_pCamera->Update();
	}

	if (m_pLight != nullptr)
	{ // ライトが NULL じゃない場合

		// ライトの更新
		m_pLight->Update();
	}

#ifdef _DEBUG		// デバッグ用

	if (m_pDebugProc != nullptr)
	{ // デバッグ表示が NULL じゃない場合

		// デバッグ表示の更新
		m_pDebugProc->Update();
	}

	if (m_pInputKeyboard->GetTrigger(DIK_F6) == true)
	{ // F6キーを押した場合

		// ワイヤーフレームのON/OFF
		m_bWire = m_bWire ? false : true;

		if (m_bWire == true)
		{ // ワイヤーモードONの場合

			// ワイヤーフレームをONにする
			m_pRenderer->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		}
		else
		{ // ワイヤーモードOFFの場合

			// ワイヤーフレームをOFFにする
			m_pRenderer->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
	}

#endif

	if (m_pScene != nullptr)
	{ // シーンが NULL じゃない場合

		// シーンの更新処理
		m_pScene->Update();
	}

	//if (m_pRenderer != nullptr)
	//{ // レンダラーが NULL じゃない場合

	//	// 更新処理
	//	m_pRenderer->Update();
	//}

	m_pDebugProc->Print("モード：%d", m_pScene->GetMode());
}

//======================================
//描画処理
//======================================
void CManager::Draw(void)
{
	if (m_pRenderer != nullptr)
	{ // レンダラーが NULL じゃない場合

		// 描画処理
		m_pRenderer->Draw();
	}
}

//======================================
// テクスチャの読み込み処理
//======================================
HRESULT CManager::TexLoad(void)
{
	// Xファイルのロード処理
	m_pXFile->Load();

	// テクスチャのロード処理
	m_pTexture->Load();

	// 成功を返す
	return S_OK;
}

//======================================
// テクスチャの破棄処理
//======================================
void CManager::TexUnload(void)
{
	// 全てのオブジェクトの破棄処理
	CObject::ReleaseAll();
}

//======================================
// レンダラーの取得処理
//======================================
CRenderer* CManager::GetRenderer(void)
{
	// レンダラーの情報を返す
	return m_pRenderer;
}

//======================================
// キーボードの取得処理
//======================================
CInputKeyboard* CManager::GetInputKeyboard(void)
{
	// キーボードの情報を返す
	return m_pInputKeyboard;
}

//======================================
// ゲームパッドの取得処理
//======================================
CInputGamePad* CManager::GetInputGamePad(void)
{
	// ゲームパッドの情報を返す
	return m_pInputGamePad;
}

//======================================
// マウスの取得処理
//======================================
CInputMouse* CManager::GetInputMouse(void)
{
	// マウスの情報を返す
	return m_pInputMouse;
}

//======================================
// サウンドの取得処理
//======================================
CSound* CManager::GetSound(void)
{
	// サウンドの情報を返す
	return m_pSound;
}

//======================================
// ファイルの取得処理
//======================================
CFile* CManager::GetFile(void)
{
	// ファイルの情報を返す
	return m_pFile;
}

//======================================
// デバッグ表示の取得処理
//======================================
CDebugProc* CManager::GetDebugProc(void)
{
	// デバッグ表示の情報を返す
	return m_pDebugProc;
}

//======================================
// カメラ表示の取得処理
//======================================
CCamera* CManager::GetCamera(void)
{
	// カメラの情報を返す
	return m_pCamera;
}

//======================================
// ライトの取得処理
//======================================
CLight* CManager::GetLight(void)
{
	// ライトの情報を返す
	return m_pLight;
}

//======================================
// テクスチャの取得処理
//======================================
CTexture* CManager::GetTexture(void)
{
	// テクスチャの情報を返す
	return m_pTexture;
}

//======================================
// シーンの取得処理
//======================================
CScene* CManager::GetScene(void)
{
	// シーンの情報を返す
	return m_pScene;
}

//======================================
// フェードの取得処理
//======================================
CFade* CManager::GetFade(void)
{
	// フェードの情報を返す
	return m_pFade;
}

//======================================
// モードの設定処理
//======================================
void CManager::SetMode(const CScene::MODE mode)
{
	if (m_pSound != nullptr)
	{ // サウンドが NULL じゃない場合

		// サウンドの停止処理
		m_pSound->Stop();
	}

	// 全てのオブジェクトの破棄処理
	CObject::ReleaseAll();

	if (m_pScene != nullptr)
	{ // シーンが NULL じゃない場合

		// シーンを消去する
		m_pScene->Uninit();
		m_pScene = nullptr;
	}

	if (m_pScene == nullptr)
	{ // シーンが NULL じゃない場合

		// 生成処理
		m_pScene = m_pScene->Create(mode);
	}
}

//======================================
// モードの取得処理
//======================================
CScene::MODE CManager::GetMode(void)
{
	// モードを返す
	return m_pScene->GetMode();
}

//======================================
// Xファイルの取得処理
//======================================
CXFile* CManager::GetXFile(void)
{
	// Xファイルの情報を返す
	return m_pXFile;
}

//======================================
// マネージャーの生成処理
//======================================
CManager* CManager::Create(void)
{
	if (m_pManager == nullptr)
	{ // マネージャーが NULL の場合

		// マネージャーを生成する
		m_pManager = new CManager;

		// マネージャーの情報を返す
		return m_pManager;
	}
	else
	{ // 上記以外

		// マネージャーの情報を返す
		return m_pManager;
	}
}

//======================================
// マネージャーの取得処理
//======================================
CManager* CManager::Get(void)
{
	if (m_pManager != nullptr)
	{ // マネージャーの NULL じゃない場合

		// マネージャーの情報を返す
		return m_pManager;
	}
	else
	{ // 上記以外

		// 停止
		assert(false);

		// マネージャーを生成する
		return CManager::Create();
	}
}