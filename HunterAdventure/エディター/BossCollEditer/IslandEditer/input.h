//=======================================
//
//入力ヘッダー[input.h]
//Author 小原立暉
//
//=======================================
#ifndef _IMPUT_H_			//このマクロ定義がされていなかったら
#define _INPUT_H_			//2重インクルード防止のマクロを定義する

#include "main.h"
#include<Xinput.h>

#pragma comment(lib,"xinput.lib")

//---------------------------------------
// マクロ定義
//---------------------------------------
#define NUM_KEY_MAX			(256)			// キーの最大数
#define MAX_PLAYER			(4)				// プレイ出来る人数

//---------------------------------------
// クラス定義(CInput)
//---------------------------------------
class CInput
{
public:			// 誰でもアクセス出来る

	CInput();				// コンストラクタ
	~CInput();				// デストラクタ

	// メンバ関数
	HRESULT Init(HINSTANCE hInstance);				// 初期化処理
	virtual void Uninit(void);						// 終了処理
	virtual void Update(void) = 0;					// 更新処理

protected:		// 自分と派生クラスのみアクセスできる

	// メンバ変数
	LPDIRECTINPUTDEVICE8 m_pDevice;				// 入力デバイスへのポインタ

	// 静的メンバ変数
	static LPDIRECTINPUT8 m_pInput;				// DirectInputオブジェクトへのポインタ
};

//---------------------------------------
// キーボードクラス
//---------------------------------------
class CInputKeyboard : public CInput
{
public:			// 誰でもアクセスできる

	CInputKeyboard();		// コンストラクタ
	~CInputKeyboard();		// デストラクタ

	// メンバ関数
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);		// 初期化処理
	void Uninit(void) override;							// 終了処理
	void Update(void) override;							// 更新処理
	bool GetPress(int nKey);							// プレス処理
	bool GetTrigger(int nKey);							// トリガー処理
	bool GetRelease(int nKey);							// リリース処理
	bool GetRepeat(int nKey, int nCount);				// リピート処理

	// 静的メンバ関数
	static CInputKeyboard* Create(HINSTANCE hInstance, HWND hWnd);		// 生成処理

private:		// 自分のみアクセスできる

	// メンバ変数
	BYTE m_aKeyState[NUM_KEY_MAX];						// プレス情報
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];				// トリガー情報
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];				// リリース情報
	int m_nRepeatCount[NUM_KEY_MAX];					// リピートカウント
};

//---------------------------------------
// ジョイパッドクラス
//---------------------------------------
class CInputGamePad : public CInput
{
public:			// 誰でもアクセスできる

	//ゲームパッドのボタン
	enum JOYKEY
	{
		JOYKEY_UP = 0,			//上ボタン
		JOYKEY_DOWN,			//下ボタン
		JOYKEY_LEFT,			//左ボタン
		JOYKEY_RIGHT,			//右ボタン
		JOYKEY_START,			//STARTボタン
		JOYKEY_BACK,			//BACKボタン
		JOYKEY_LEFTSTICK,		//左のスティック押し込み
		JOYKEY_RIGHTSTICK,		//右のスティック押し込み
		JOYKEY_LB,				//LBボタン
		JOYKEY_RB,				//RBボタン
		JOYKEY_PPP1,			//不明
		JOYKEY_PPP2,			//不明
		JOYKEY_A,				//Aボタン
		JOYKEY_B,				//Bボタン
		JOYKEY_X,				//Xボタン
		JOYKEY_Y,				//Yボタン
		JOYKEY_STICKLX,			//Lスティック(X軸)(右に行くほど値は大きい)
		JOYKEY_STICKLY,			//Lスティック(Y軸)(上に行くほど値は大きい)
		JOYKEY_STICKRX,			//Lスティック(X軸)(右に行くほど値は大きい)
		JOYKEY_STICKRY,			//Lスティック(Y軸)(上に行くほど値は大きい)
		JOYKEY_MAX
	};

	// 構造体定義(バイブレーション関係)
	struct SVibrate
	{
		XINPUT_VIBRATION vibration;		// バイブレーションの情報
		int nCountRight;				// 右の震えるカウント
		int nCountLeft;					// 左の震えるカウント
	};

	CInputGamePad();		// コンストラクタ
	~CInputGamePad();		// デストラクタ

	// メンバ関数
	HRESULT Init(HINSTANCE hInstance);				// 初期化処理
	void Uninit(void) override;						// 終了処理
	void Update(void) override;						// 更新処理
	bool GetPress(JOYKEY nKey, int nPlayer);		// プレス処理
	bool GetTrigger(JOYKEY nKey, int nPlayer);		// トリガー情報処理
	bool GetRelease(JOYKEY nKey, int nPlayer);		// リリース情報処理
	bool GetRepeat(JOYKEY nKey, int nPlayer, int nCount);	// リピート処理
	void GetRightVibration(int nPlayer, const WORD strength, const int nCount);		// 右のバイブレーション処理
	void GetLeftVibration(int nPlayer, const WORD strength, const int nCount);		// 左のバイブレーション処理
	bool GetConnect(void);							// 接続判定の取得処理

	// 左スティック関係
	SHORT GetGameStickLXPress(int nPlayer);			// Lスティック(X軸)入力のプレス情報処理
	SHORT GetGameStickLYPress(int nPlayer);			// Lスティック(Y軸)入力のプレス情報処理

	// 右スティック関係
	SHORT GetGameStickRXPress(int nPlayer);			// Rスティック(X軸)入力のプレス情報処理
	SHORT GetGameStickRYPress(int nPlayer);			// Rスティック(Y軸)入力のプレス情報処理

	// 静的メンバ関数
	static CInputGamePad* Create(HINSTANCE hInstance);		// 生成処理

private:		// 誰でもアクセスできる

	// メンバ変数
	XINPUT_STATE m_aPadState[MAX_PLAYER];			// プレス情報
	XINPUT_STATE m_aPadStateTrigger[MAX_PLAYER];	// トリガー情報
	XINPUT_STATE m_aPadStateRelease[MAX_PLAYER];	// リリース情報
	int m_nRepeatCount[MAX_PLAYER][JOYKEY_MAX];		// リピートカウント
	SVibrate m_aVibration[MAX_PLAYER];				// バイブレーションの情報
	bool m_bConnect;								// 接続判定
};

//---------------------------------------
// マウスクラス
//---------------------------------------
class CInputMouse : public CInput
{
public:			// 誰でもアクセスできる

	// 列挙型定義
	typedef enum
	{
		MOUSE_L = 0,	// 左クリック
		MOUSE_R,		// 右クリック
		MOUSE_WHEEL,	// マウスホイール
		MOUSE_CYCLE,	// マウスホイールの回転
		MOUSE_MAX		// この列挙型の総数
	}MOUSE_BUTTON;

	CInputMouse();		// コンストラクタ
	~CInputMouse();		// デストラクタ

	// メンバ関数
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// 初期化処理
	void Uninit(void) override;						// 終了処理
	void Update(void) override;						// 更新処理

	// 入力関係
	bool GetPress(const MOUSE_BUTTON button);		// クリックのプレス処理
	bool GetTrigger(const MOUSE_BUTTON button);		// クリックのトリガー処理
	bool GetRelease(const MOUSE_BUTTON button);		// クリックのリリース情報処理
	D3DXVECTOR3 GetMove(void);						// マウスカーソルの移動量取得処理
	POINT GetPos(void);								// マウスカーソルの位置設定処理

	// 静的メンバ関数
	static CInputMouse* Create(HINSTANCE hInstance, HWND hWnd);		// 生成処理

private:		// 誰でもアクセスできる

	// メンバ変数
	DIMOUSESTATE m_MouseState;			// プレス(ドラッグ)の状態
	DIMOUSESTATE m_MouseStateTrigger;	// トリガーの状態
	DIMOUSESTATE m_MouseStateRelease;	// リリースの状態
	POINT m_MousePos;					// マウスの位置
	D3DXVECTOR3 m_MouseMove;			// マウスの移動量
};

#endif