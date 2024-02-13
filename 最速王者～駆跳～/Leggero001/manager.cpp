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
#include "camera.h"
#include "light.h"
#include "fade.h"

//--------------------------------------------
// 静的メンバ変数宣言
//--------------------------------------------
CRenderer* CManager::m_pRenderer = nullptr;				// レンダラーの情報
CInputKeyboard* CManager::m_pInputKeyboard = nullptr;	// キーボードの情報
CInputGamePad* CManager::m_pInputGamePad = nullptr;		// ゲームパッドの情報
CSound* CManager::m_pSound = nullptr;					// サウンドの情報
CFile* CManager::m_pFile = nullptr;						// ファイルの情報
CDebugProc* CManager::m_pDebugProc = nullptr;			// デバッグ表示の情報
CCamera* CManager::m_pCamera = nullptr;					// カメラの情報
CLight* CManager::m_pLight = nullptr;					// ライトの情報
CTexture* CManager::m_pTexture = nullptr;				// テクスチャの情報
CScene* CManager::m_pScene = nullptr;					// シーンの情報
CFade* CManager::m_pFade = nullptr;						// フェードの情報
DWORD CManager::m_nClearTime = 0;						// クリアタイム
int CManager::m_nActionScore = 0;						// アクションスコア
bool CManager::m_bClear = false;						// クリア状況

#ifdef _DEBUG		// デバッグ用

bool CManager::m_bWire = false;									// ワイヤーフレーム状況

#endif

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
	if (m_pFile == nullptr)
	{ // ファイルへのポインタが nullptr の場合

		// ファイルのメモリを確保する
		m_pFile = new CFile;
	}
	else
	{ // ポインタが使われていた場合

		// 警告文
		MessageBox(NULL, "ファイルのポインタが使われている！", "警告！", MB_ICONWARNING);
	}

	if (m_pDebugProc == nullptr)
	{ // デバッグ表示へのポインタが nullptr の場合

		// デバッグ表示のメモリを確保する
		m_pDebugProc = new CDebugProc;
	}
	else
	{ // ポインタが使われていた場合

		// 警告文
		MessageBox(NULL, "デバッグ表示のポインタが使われている！", "警告！", MB_ICONWARNING);
	}

	if (m_pRenderer == nullptr)
	{ // レンダラーへのポインタが NULL の場合

		// レンダラーのメモリを確保する
		m_pRenderer = new CRenderer;
	}
	else
	{ // ポインタが使われていた場合

		// 警告文
		MessageBox(NULL, "レンダラーのポインタが使われている！", "警告！", MB_ICONWARNING);
	}

	if (m_pInputKeyboard == nullptr)
	{ // キーボードへのポインタが NULL の場合

		// キーボードのメモリを確保する
		m_pInputKeyboard = new CInputKeyboard;
	}
	else
	{ // ポインタが使われていた場合

		// 警告文
		MessageBox(NULL, "キーボードのポインタが使われている！", "警告！", MB_ICONWARNING);
	}

	if (m_pInputGamePad == nullptr)
	{ // ゲームパッドへのポインタが NULL の場合

		// ゲームパッドのメモリを確保する
		m_pInputGamePad = new CInputGamePad;
	}
	else
	{ // ポインタが使われていた場合

		// 警告文
		MessageBox(NULL, "ゲームパッドのポインタが使われている！", "警告！", MB_ICONWARNING);
	}

	if (m_pSound == nullptr)
	{ // サウンドへのポインタが NULL の場合

		// サウンドのメモリを確保する
		m_pSound = new CSound;
	}
	else
	{ // ポインタが使われていた場合

		// 警告文
		MessageBox(NULL, "サウンドのポインタが使われている！", "警告！", MB_ICONWARNING);
	}

	if (m_pCamera == nullptr)
	{ // カメラへのポインタが NULL の場合

		// カメラのメモリを確保する
		m_pCamera = new CCamera;
	}
	else
	{ // ポインタが使われていた場合

		// 警告文
		MessageBox(NULL, "カメラのポインタが使われている！", "警告！", MB_ICONWARNING);
	}

	if (m_pLight == nullptr)
	{ // ライトへのポインタが NULL の場合

		// ライトのメモリを確保する
		m_pLight = new CLight;
	}
	else
	{ // ポインタが使われていた場合

		// 警告文
		MessageBox(NULL, "ライトのポインタが使われている！", "警告！", MB_ICONWARNING);
	}

	if (m_pTexture == nullptr)
	{ // テクスチャへのポインタが NULL の場合

		// メモリを確保する
		m_pTexture = new CTexture;
	}
	else
	{ // ポインタが使われていた場合

		// 警告文
		MessageBox(NULL, "テクスチャのポインタが使われている！", "警告！", MB_ICONWARNING);
	}

	if (m_pRenderer != nullptr)
	{ // 確保に成功していた場合

		// 初期化処理
		if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
		{ // 初期化処理が失敗した場合

			// 失敗を返す
			return E_FAIL;
		}
	}
	else
	{ // 確保に失敗していた場合

		// 警告文
		MessageBox(NULL, "レンダラーの確保に失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	if (m_pInputKeyboard != nullptr)
	{ // 確保に成功していた場合

		// 初期化処理
		if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
		{ // 初期化処理が失敗した場合

			// 失敗を返す
			return E_FAIL;
		}
	}
	else
	{ // 確保に失敗していた場合

		// 警告文
		MessageBox(NULL, "キーボードの確保に失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	if (m_pInputGamePad != nullptr)
	{ // 確保に成功していた場合

		// 初期化処理
		if (FAILED(m_pInputGamePad->Init(hInstance, hWnd)))
		{ // 初期化処理が失敗した場合

			// 失敗を返す
			return E_FAIL;
		}
	}
	else
	{ // 確保に失敗していた場合

		// 警告文
		MessageBox(NULL, "ゲームパッドの確保に失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	if (m_pSound != nullptr)
	{ // 確保に成功していた場合

		// 初期化処理
		if (FAILED(m_pSound->Init(hWnd)))
		{ // 初期化処理が失敗した場合

			// 失敗を返す
			return E_FAIL;
		}
	}
	else
	{ // 確保に失敗していた場合

		// 警告文
		MessageBox(NULL, "サウンドの確保に失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	if (m_pDebugProc != nullptr)
	{ // 確保に成功していた場合

		// 初期化処理
		m_pDebugProc->Init();
	}
	else
	{ // 確保に失敗していた場合

		// 警告文
		MessageBox(NULL, "デバッグ表示の確保に失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	if (m_pCamera != nullptr)
	{ // 確保に成功していた場合

		// 初期化処理
		if (FAILED(m_pCamera->Init()))
		{ // カメラの初期化に失敗した場合

			// 失敗を返す
			return E_FAIL;
		}
	}
	else
	{ // 確保に失敗していた場合

		// 警告文
		MessageBox(NULL, "カメラの確保に失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	if (m_pLight != nullptr)
	{ // 確保に成功していた場合

		// 初期化処理
		if (FAILED(m_pLight->Init()))
		{ // ライトの初期化に失敗した場合

			// 失敗を返す
			return E_FAIL;
		}
	}
	else
	{ // 確保に失敗していた場合

		// 警告文
		MessageBox(NULL, "ライトの確保に失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	if (m_pTexture != nullptr)
	{ // 確保に成功していた場合

		// 初期化処理
		if (FAILED(m_pTexture->Init()))
		{ // テクスチャの初期化に失敗した場合

			// 失敗を返す
			return E_FAIL;
		}
	}
	else
	{ // 確保に失敗した場合

		// 警告文
		MessageBox(NULL, "テクスチャの確保に失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// テクスチャの読み込み処理
	if (FAILED(TexLoad()))
	{ // 読み込みに失敗した場合

		// 警告文
		MessageBox(NULL, "テクスチャの読み込みに失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	if (m_pFade == nullptr)
	{ // フェードへのポインタが nullptr の場合

#ifdef _DEBUG

		// フェードのメモリを確保する
		m_pFade = m_pFade->Create(CScene::MODE_LOGO);
#else

		// シーンのメモリを確保する
		m_pFade = m_pFade->Create(CScene::MODE_LOGO);
#endif 

	}
	else
	{ // ポインタが使われていた場合

		// 警告文
		MessageBox(NULL, "フェードのポインタが使われている！", "警告！", MB_ICONWARNING);
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

		// メモリを開放する
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	if (m_pInputKeyboard != nullptr)
	{ // キーボードが NULL じゃない場合

		// 終了処理
		m_pInputKeyboard->Uninit();

		// メモリを開放する
		delete m_pInputKeyboard;
		m_pInputKeyboard = nullptr;
	}

	if (m_pInputGamePad != nullptr)
	{ // ゲームパッドが NULL じゃない場合

		// 終了処理
		m_pInputGamePad->Uninit();

		// メモリを開放する
		delete m_pInputGamePad;
		m_pInputGamePad = nullptr;
	}

	if (m_pSound != nullptr)
	{ // サウンドが NULL じゃない場合

		// 終了処理
		m_pSound->Uninit();

		// メモリを開放する
		delete m_pSound;
		m_pSound = nullptr;
	}

	if (m_pFile != nullptr)
	{ // ファイルが NULL じゃない場合

		// 終了処理
		m_pFile->Uninit();

		// メモリを開放する
		delete m_pFile;
		m_pFile = nullptr;
	}

	if (m_pDebugProc != nullptr)
	{ // デバッグ表示が NULL じゃない場合

		// 終了処理
		m_pDebugProc->Uninit();

		// メモリを開放する
		delete m_pDebugProc;
		m_pDebugProc = nullptr;
	}

	if (m_pCamera != nullptr)
	{ // カメラが NULL じゃない場合

		// 終了処理
		m_pCamera->Uninit();

		// メモリを開放する
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	if (m_pLight != nullptr)
	{ // ライトが NULL じゃない場合

		// 終了処理
		m_pLight->Uninit();

		// メモリを開放する
		delete m_pLight;
		m_pLight = nullptr;
	}

	if (m_pTexture != nullptr)
	{ // テクスチャが NULL じゃない場合

		// テクスチャの破棄処理
		m_pTexture->Unload();

		// メモリを開放する
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	if (m_pFade != nullptr)
	{ // フェードが NULL じゃない場合

		// フェードの破棄処理
		m_pFade->Uninit();

		// メモリを開放する
		delete m_pFade;
		m_pFade = nullptr;
	}

	// シーンへのポインタを NULL にする
	m_pScene = nullptr;
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

	if (m_pScene != nullptr)
	{ // シーンが NULL じゃない場合

		// シーンの描画処理
		m_pScene->Draw();
	}
}

//======================================
// テクスチャの読み込み処理
//======================================
HRESULT CManager::TexLoad(void)
{
	// Xファイルの初期化処理
	CXFile::Init();

	// テクスチャのロード処理
	m_pTexture->Load();

	if (m_pFile != nullptr)
	{ // ファイルのメモリが確保できていた場合

		// ファイルのロード処理
		m_pFile->Load(CFile::TYPE_BLOCK);
		m_pFile->Load(CFile::TYPE_OBSTACLE);
		m_pFile->Load(CFile::TYPE_ITEM);
		m_pFile->Load(CFile::TYPE_CAMERA);
		m_pFile->Load(CFile::TYPE_RESULT);
		m_pFile->Load(CFile::TYPE_GAME);
		m_pFile->Load(CFile::TYPE_TUTORIAL);
	}

	// 成功を返す
	return S_OK;
}

//======================================
// テクスチャの破棄処理
//======================================
void CManager::TexUnload(void)
{
	// Xファイルの終了処理
	CXFile::Uninit();

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
// フェードの取得処理
//======================================
CFade* CManager::GetFade(void)
{
	// フェードの情報を返す
	return m_pFade;
}

//======================================
// クリアタイムの設定処理
//======================================
void CManager::SetClearTime(const DWORD nTime)
{
	// クリアタイムを設定する
	m_nClearTime = nTime;
}

//======================================
// クリアタイムの取得処理
//======================================
DWORD CManager::GetClearTime(void)
{
	// クリアタイムを返す
	return m_nClearTime;
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

	// シーンを NULL にする
	m_pScene = nullptr;

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
// スコアの設定処理
//======================================
void CManager::SetScore(const int nScore)
{
	// スコアを設定する
	m_nActionScore = nScore;
}

//======================================
// スコアの取得処理
//======================================
int CManager::GetScore(void)
{
	//	アクションスコアを返す
	return m_nActionScore;
}

//======================================
// クリア状況の設定処理
//======================================
void CManager::SetClear(const bool bClear)
{
	// クリア状況を設定する
	m_bClear = bClear;
}

//======================================
// クリア状況の取得処理
//======================================
bool CManager::GetClear(void)
{
	// クリア状況を返す
	return m_bClear;
}