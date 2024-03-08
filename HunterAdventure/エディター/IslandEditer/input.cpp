//=========================================
//
//入力のメイン処理[input.cpp]
//Author 小原立暉
//
//=========================================
#include "manager.h"
#include "debugproc.h"
#include "input.h"

//-----------------------------------------
// 静的メンバ変数宣言
//-----------------------------------------
LPDIRECTINPUT8 CInput::m_pInput = nullptr;				// DirectInputオブジェクトへのポインタ

//======================================================ここからCInputの処理======================================================

//==================================
// コンストラクタ
//==================================
CInput::CInput()
{
	// 全ての値をクリアする
	m_pDevice = nullptr;								// デバイスへのポインタ
}

//==================================
// デストラクタ
//==================================
CInput::~CInput()
{

}

//==================================
// 初期化処理
//==================================
HRESULT CInput::Init(HINSTANCE hInstance)
{
	if (m_pInput == nullptr)
	{ // オブジェクトへのポインタが NULL の場合

		// DirectInputオブジェクトの生成
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{ // オブジェクトの生成に失敗した場合

			// 警告文
			MessageBox(NULL, "DirectInputオブジェクトの生成に失敗！", "警告！", MB_ICONWARNING);

			// 停止
			assert(false);

			// 失敗を返す
			return E_FAIL;
		}
	}

	// 成功を返す
	return S_OK;
}

//==================================
// 終了処理
//==================================
void CInput::Uninit(void)
{
	//入力デバイスの破棄
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();					//キーボードへのアクセス権を放棄

		m_pDevice->Release();

		m_pDevice = nullptr;
	}

	//DirectInputオブジェクトの放棄
	if (m_pInput != nullptr)
	{
		m_pInput->Release();

		m_pInput = nullptr;
	}
}

//======================================================ここからCInputKeyboardの処理======================================================

//==================================
// コンストラクタ
//==================================
CInputKeyboard::CInputKeyboard()
{
	// 全ての値をクリアする
	for (int nCnt = 0; nCnt < NUM_KEY_MAX; nCnt++)
	{
		// キー情報
		ZeroMemory(&m_aKeyState[nCnt], sizeof(BYTE));
	}
}

//==================================
// デストラクタ
//==================================
CInputKeyboard::~CInputKeyboard()
{

}

//==================================
// 初期化処理
//==================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// CInputの初期化処理
	CInput::Init(hInstance);

	//入力デバイス(キーボード)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{ // 生成に失敗した場合

		// 警告文
		MessageBox(NULL, "キーボードの入力デバイスの生成に失敗！", "警告！", MB_ICONWARNING);

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{ // 設定に失敗した場合

		// 警告文
		MessageBox(NULL, "キーボードのデータフォーマット設定に失敗！", "警告！", MB_ICONWARNING);

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{ // 設定に失敗した場合

		// 警告文
		MessageBox(NULL, "キーボードの協調モード設定に失敗！", "警告！", MB_ICONWARNING);

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	m_pDevice->Acquire();

	// 成功を返す
	return S_OK;
}

//==================================
// 終了処理
//==================================
void CInputKeyboard::Uninit(void)
{
	// CInputの終了処理
	CInput::Uninit();

	// メモリを開放する
	delete this;
}

//==================================
// 更新処理
//==================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];						//キーボードの入力情報
	int nCntKey;

	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{ // 取得に成功した場合

		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];		//キーボードのトリガー情報を保存

			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey];	//キーボードのリリース情報を保存

			m_aKeyState[nCntKey] = aKeyState[nCntKey];			//キーボードのプレス情報を保存
		}
	}
	else
	{ // 取得に失敗した場合

		//キーボードへのアクセス権を獲得
		m_pDevice->Acquire();
	}
}

//======================================
// プレス情報を獲得
//======================================
bool CInputKeyboard::GetPress(int nKey)
{
	// プレス状況を返す
	return(m_aKeyState[nKey] & 0x80) ? true : false;
}

//======================================
// トリガー処理
//======================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	// トリガー状況を返す
	return(m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//======================================
// リリース処理
//======================================
bool CInputKeyboard::GetRelease(int nKey)
{
	// リリース状況を返す
	return(m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//======================================
// リピート処理
//======================================
bool CInputKeyboard::GetRepeat(int nKey, int nCount)
{
	if (CInputKeyboard::GetPress(nKey) == true)
	{ // キーを押している場合

		// カウントを加算する
		m_nRepeatCount[nKey]++;
	}
	else
	{ // キーを押していない場合

		// カウントを加算する
		m_nRepeatCount[nKey] = 0;
	}

	if (m_nRepeatCount[nKey] >= nCount)
	{ // リピートカウントがカウント数を超えた場合

		// カウントを0にする
		m_nRepeatCount[nKey] = 0;

		// true を返す
		return true;
	}
	else
	{ // 上記以外

		// false を返す
		return false;
	}
}

//======================================
// 生成処理
//======================================
CInputKeyboard* CInputKeyboard::Create(HINSTANCE hInstance, HWND hWnd)
{
	// ローカルオブジェクトを生成
	CInputKeyboard* pInput = nullptr;		// 入力のインスタンスを生成

	if (pInput == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pInput = new CInputKeyboard;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pInput != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pInput->Init(hInstance, hWnd)))
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

	// 入力のポインタを返す
	return pInput;
}

//======================================================ここからCInputGamePadの処理======================================================

//==================================
// コンストラクタ
//==================================
CInputGamePad::CInputGamePad()
{
	// 全ての値をクリアする
	ZeroMemory(&m_aPadState[0], sizeof(m_aPadState));					// プレス情報
	ZeroMemory(&m_aPadStateTrigger[0], sizeof(m_aPadStateTrigger));		// トリガー情報
	ZeroMemory(&m_aPadStateRelease[0], sizeof(m_aPadStateRelease));		// リリース情報
	ZeroMemory(&m_nRepeatCount[0][0], sizeof(m_nRepeatCount));			// リピートカウント
	ZeroMemory(&m_aVibration[0], sizeof(m_aVibration));					// バイブレーション情報
	m_bConnect = false;													// 接続判定
}

//==================================
// デストラクタ
//==================================
CInputGamePad::~CInputGamePad()
{

}

//==================================
// 初期化処理
//==================================
HRESULT CInputGamePad::Init(HINSTANCE hInstance)
{
	// CInputの初期化処理
	CInput::Init(hInstance);

	// ゲームパッド設定を有効にする
	XInputEnable(true);

	// ゲームパッドのプレス情報を用意する
	memset(&m_aPadState[0], 0, sizeof(m_aPadState));

	// ゲームパッドのトリガー情報を用意する
	memset(&m_aPadStateTrigger[0], 0, sizeof(m_aPadStateTrigger));

	// ゲームパッドのリリース情報を用意する
	memset(&m_aPadStateRelease[0], 0, sizeof(m_aPadStateRelease));

	// リピートカウントを用意する
	memset(&m_nRepeatCount[0][0], 0, sizeof(m_nRepeatCount));

	// バイブレーション情報を用意する
	memset(&m_aVibration[0], 0, sizeof(m_aVibration));

	// 接続判定を初期化する
	m_bConnect = false;

	// 失敗を返す
	return S_OK;
}

//==================================
// 終了処理
//==================================
void CInputGamePad::Uninit(void)
{
	// CInputの終了処理
	CInput::Uninit();

	// 接続判定を false にする
	m_bConnect = false;

	//ゲームパッド設定を無効にする
	XInputEnable(false);

	// メモリを開放する
	delete this;
}

//==================================
// 更新処理
//==================================
void CInputGamePad::Update(void)
{
	XINPUT_STATE aGamePadState[MAX_PLAYER];			// ゲームパッドの入力情報
	bool bConnect = false;							// 接続されているかどうか

	for (int nCntGP = 0; nCntGP < MAX_PLAYER; nCntGP++)
	{//それぞれのゲームパッド分設定する

		//入力デバイスからデータを取得
		if (XInputGetState(nCntGP, &aGamePadState[nCntGP]) == ERROR_SUCCESS)
		{
			//ゲームパッドのトリガー情報を保存
			m_aPadStateTrigger[nCntGP].Gamepad.wButtons = ~m_aPadState[nCntGP].Gamepad.wButtons & aGamePadState[nCntGP].Gamepad.wButtons;

			//ゲームパッドのリリース情報を保存
			m_aPadStateRelease[nCntGP].Gamepad.wButtons = (m_aPadState[nCntGP].Gamepad.wButtons ^ aGamePadState[nCntGP].Gamepad.wButtons) & ~aGamePadState[nCntGP].Gamepad.wButtons;

			//ゲームパッドのプレス情報を保存
			m_aPadState[nCntGP] = aGamePadState[nCntGP];

			// 接続されている
			bConnect = true;

			if (m_aVibration[nCntGP].nCountRight > 0)
			{ // 右のカウントが0超過の場合

				// カウントを減算する
				m_aVibration[nCntGP].nCountRight--;
			}
			else
			{ // 上記以外
				
				// カウントを0にする
				m_aVibration[nCntGP].nCountRight = 0;

				// 右のバイブレーションの強さを0にする
				m_aVibration[nCntGP].vibration.wRightMotorSpeed = 0;
			}

			if (m_aVibration[nCntGP].nCountLeft > 0)
			{ // 左のカウントが0超過の場合

				// カウントを減算する
				m_aVibration[nCntGP].nCountLeft--;
			}
			else
			{ // 上記以外
				
				// カウントを0にする
				m_aVibration[nCntGP].nCountLeft = 0;

				// 左のバイブレーションの強さを0にする
				m_aVibration[nCntGP].vibration.wLeftMotorSpeed = 0;
			}

			// バイブレーションの情報を渡す
			XInputSetState(nCntGP, &m_aVibration[nCntGP].vibration);
		}
	}

	// 接続判定を代入する
	m_bConnect = bConnect;
}

//======================================
// プレス情報を獲得
//======================================
bool CInputGamePad::GetPress(JOYKEY nKey, int nPlayer)
{
	return(m_aPadState[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//======================================
// トリガー情報処理
//======================================
bool CInputGamePad::GetTrigger(JOYKEY nKey, int nPlayer)
{
	return(m_aPadStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//======================================
// リリース情報処理
//======================================
bool CInputGamePad::GetRelease(JOYKEY nKey, int nPlayer)
{
	return(m_aPadStateRelease[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//======================================
// リピート処理
//======================================
bool CInputGamePad::GetRepeat(JOYKEY nKey, int nPlayer, int nCount)
{
	if (CInputGamePad::GetPress(nKey, nPlayer) == true)
	{ // キーを押している場合

		// カウントを加算する
		m_nRepeatCount[nPlayer][nKey]++;
	}
	else
	{ // キーを押していない場合

		// カウントを加算する
		m_nRepeatCount[nPlayer][nKey] = 0;
	}

	if (m_nRepeatCount[nPlayer][nKey] >= nCount)
	{ // リピートカウントがカウント数を超えた場合

		// カウントを0にする
		m_nRepeatCount[nPlayer][nKey] = 0;

		// true を返す
		return true;
	}

	// false を返す
	return false;
}

//======================================
// 右のバイブレーション処理
//======================================
void CInputGamePad::GetRightVibration(int nPlayer, const WORD strength, const int nCount)
{
	// 右のバイブレーションを ON にする
	m_aVibration[nPlayer].vibration.wRightMotorSpeed = strength;

	// カウントを設定する
	m_aVibration[nPlayer].nCountRight = nCount;

	// バイブレーションの情報を渡す
	XInputSetState(nPlayer, &m_aVibration[nPlayer].vibration);
}

//======================================
// 左のバイブレーション処理
//======================================
void CInputGamePad::GetLeftVibration(int nPlayer, const WORD strength, const int nCount)
{
	// 右のバイブレーションを ON にする
	m_aVibration[nPlayer].vibration.wLeftMotorSpeed = strength;

	// カウントを設定する
	m_aVibration[nPlayer].nCountLeft = nCount;

	// バイブレーションの情報を渡す
	XInputSetState(nPlayer, &m_aVibration[nPlayer].vibration);
}

//======================================
// 接続されている場合
//======================================
bool CInputGamePad::GetConnect(void)
{
	// 接続判定を返す
	return m_bConnect;
}

//======================================
//Lスティック(X軸)入力のプレス情報処理
//======================================
SHORT CInputGamePad::GetGameStickLXPress(int nPlayer)
{
	return m_aPadState[nPlayer].Gamepad.sThumbLX;
}

//======================================
//Lスティック(Y軸)入力のプレス情報処理
//======================================
SHORT CInputGamePad::GetGameStickLYPress(int nPlayer)
{
	return m_aPadState[nPlayer].Gamepad.sThumbLY;
}

//======================================
//Rスティック(X軸)入力のプレス情報処理
//======================================
SHORT CInputGamePad::GetGameStickRXPress(int nPlayer)
{
	return m_aPadState[nPlayer].Gamepad.sThumbRX;
}

//======================================
//Rスティック(Y軸)入力のプレス情報処理
//======================================
SHORT CInputGamePad::GetGameStickRYPress(int nPlayer)
{
	return m_aPadState[nPlayer].Gamepad.sThumbRY;
}

//======================================
// 生成処理
//======================================
CInputGamePad* CInputGamePad::Create(HINSTANCE hInstance)
{
	// ローカルオブジェクトを生成
	CInputGamePad* pInput = nullptr;		// 入力のインスタンスを生成

	if (pInput == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pInput = new CInputGamePad;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pInput != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pInput->Init(hInstance)))
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

	// 入力のポインタを返す
	return pInput;
}

//======================================================ここからCInputMouseの処理======================================================

//======================================
// コンストラクタ
//======================================
CInputMouse::CInputMouse()
{
	// 全ての値をクリアする
	ZeroMemory(&m_MouseState, sizeof(m_MouseState));
	ZeroMemory(&m_MouseStateTrigger, sizeof(m_MouseStateTrigger));
	ZeroMemory(&m_MouseStateRelease, sizeof(m_MouseStateRelease));
	ZeroMemory(&m_MousePos, sizeof(m_MousePos));
	ZeroMemory(&m_MouseMove, sizeof(m_MouseMove));
}

//======================================
// デストラクタ
//======================================
CInputMouse::~CInputMouse()
{

}

//======================================
// 初期化処理
//======================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// CInputの初期化処理
	CInput::Init(hInstance);

	// 入力デバイス(キーボード)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{ // 生成に失敗した場合

		// 警告文
		MessageBox(NULL, "マウスの入力デバイスの生成に失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
	{ // 設定に失敗した場合

		// 警告文
		MessageBox(NULL, "マウスのデータフォーマット設定に失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{ // 設定に失敗した場合

		// 警告文
		MessageBox(NULL, "マウスの協調モード設定に失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// マウスへのアクセス権を獲得
	if (FAILED(m_pDevice->Acquire()))
	{ // 設定に失敗した場合

		// 警告文
		MessageBox(NULL, "マウスのアクセス権の取得に失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// ポーリング開始
	if (FAILED(m_pDevice->Poll()))
	{ // 設定に失敗した場合

		// 警告文
		MessageBox(NULL, "マウスのポーリングに失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	ZeroMemory(&m_MouseState, sizeof(m_MouseState));					// マウスのプレス情報
	ZeroMemory(&m_MouseStateTrigger, sizeof(m_MouseStateTrigger));		// マウスのトリガー情報
	ZeroMemory(&m_MouseStateRelease, sizeof(m_MouseStateRelease));		// マウスのリリース情報
	ZeroMemory(&m_MousePos, sizeof(m_MousePos));						// マウスの位置
	ZeroMemory(&m_MouseMove, sizeof(m_MouseMove));						// マウスの移動量
	
	// 失敗を返す
	return S_OK;
}

//======================================
// 終了処理
//======================================
void CInputMouse::Uninit(void)
{
	// 終了処理
	CInput::Uninit();

	// メモリを開放する
	delete this;
}

//======================================
// 更新処理
//======================================
void CInputMouse::Update(void)
{
	// ローカル変数宣言
	DIMOUSESTATE MouseState;			// マウスの入力情報

	// マウスの状態を取得する
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &MouseState)))
	{ // 取得に成功した場合

		for (int nCntButton = 0; nCntButton < MOUSE_MAX; nCntButton++)
		{
			// マウスのトリガー情報を保存
			m_MouseStateTrigger.rgbButtons[nCntButton] = ~m_MouseState.rgbButtons[nCntButton] & MouseState.rgbButtons[nCntButton];

			//ゲームパッドのリリース情報を保存
			m_MouseStateRelease.rgbButtons[nCntButton] = (m_MouseState.rgbButtons[nCntButton] ^ MouseState.rgbButtons[nCntButton]) & ~MouseState.rgbButtons[nCntButton];

			// マウスのプレス情報を保存する
			m_MouseState.rgbButtons[nCntButton] = MouseState.rgbButtons[nCntButton];
		}

		// マウスカーソルの位置取得処理
		GetCursorPos(&m_MousePos);

		// スクリーン座標をクライアント座標に変換する
		ScreenToClient(FindWindowA(CLASS_NAME, WINDOW_NAME), &m_MousePos);

		// マウスの移動量を設定する
		m_MouseMove = D3DXVECTOR3((float)MouseState.lX, (float)MouseState.lY, (float)MouseState.lZ);

		//// カーソルを表示しない
		//ShowCursor(FALSE);

		//// 画面サイズの構造体を生成する
		//RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

		//// カーソルを画面にとどめる
		//ClipCursor(&rect);
	}
	else
	{ // 上記以外

		// アクセス権を取得する
		m_pDevice->Acquire();
	}
}

//======================================
// クリックのプレス処理
//======================================
bool CInputMouse::GetPress(const MOUSE_BUTTON button)
{
	return (m_MouseState.rgbButtons[button] & 0x80) ? true : false;
}

//======================================
// クリックのトリガー処理
//======================================
bool CInputMouse::GetTrigger(const MOUSE_BUTTON button)
{
	return (m_MouseStateTrigger.rgbButtons[button] & 0x80) ? true : false;
}

//======================================
// クリックのリリース情報処理
//======================================
bool CInputMouse::GetRelease(const MOUSE_BUTTON button)
{
	return (m_MouseStateRelease.rgbButtons[button] & 0x80) ? true : false;
}

//======================================
// マウスカーソルの移動量取得処理
//======================================
D3DXVECTOR3 CInputMouse::GetMove(void)
{
	// 移動量を返す
	return m_MouseMove;
}

//======================================
// マウスカーソルの位置設定処理
//======================================
POINT CInputMouse::GetPos(void)
{
	// 位置を返す
	return m_MousePos;
}

//======================================
// 生成処理
//======================================
CInputMouse* CInputMouse::Create(HINSTANCE hInstance, HWND hWnd)
{
	// ローカルオブジェクトを生成
	CInputMouse* pInput = nullptr;		// 入力のインスタンスを生成

	if (pInput == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pInput = new CInputMouse;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pInput != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pInput->Init(hInstance, hWnd)))
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

	// 入力のポインタを返す
	return pInput;
}