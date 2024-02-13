//============================================
//
// マネージャーのヘッダー[manager.h]
// Author：小原立暉
//
//============================================
#ifndef _MANAGER_H_			//このマクロ定義がされていなかったら
#define _MANAGER_H_			//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "main.h"
#include "scene.h"
#include "camera.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CRenderer;		// レンダラー
class CInputKeyboard;	// キーボード入力
class CInputGamePad;	// ゲームパッド
class CSound;			// サウンド
class CFile;			// ファイル
class CDebugProc;		// デバッグ表示
class CCamera;			// カメラ
class CLight;			// ライト
class CEditAll;			// エディットオール
class CTexture;			// テクスチャ
class CFade;			// フェード

//--------------------------------------------
// クラス(マネージャークラス)
//--------------------------------------------
class CManager
{
public:						// 誰でもアクセスできる

	CManager();				// コンストラクタ
	~CManager();			// デストラクタ

	// メンバ関数
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);		// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

	HRESULT TexLoad(void);						// テクスチャの読み込み処理
	void TexUnload(void);						// テクスチャの破棄処理

	// 静的メンバ関数(セット・ゲット)
	static CRenderer* GetRenderer(void);			// レンダラーの取得処理
	static CInputKeyboard* GetInputKeyboard(void);	// キーボードの取得処理
	static CInputGamePad* GetInputGamePad(void);	// ゲームパッドの取得処理
	static CSound* GetSound(void);					// サウンドの取得処理
	static CFile* GetFile(void);					// ファイルの取得処理
	static CDebugProc* GetDebugProc(void);			// デバッグ表示の取得処理
	static CCamera* GetCamera(void);				// カメラ表示の取得処理
	static CLight* GetLight(void);					// ライトの取得処理
	static CTexture* GetTexture(void);				// テクスチャの取得処理
	static CFade* GetFade(void);					// フェードの取得処理

	static void SetClearTime(const DWORD nTime);	// クリアタイムの設定処理
	static DWORD GetClearTime(void);				// クリアタイムの取得処理

	static void SetMode(const CScene::MODE mode);	// モードの設定処理
	static CScene::MODE GetMode(void);				// モードの取得処理

	static void SetScore(const int nScore);			// スコアの設定処理
	static int GetScore(void);						// スコアの取得処理

	static void SetClear(const bool bClear);		// クリア状況の設定処理
	static bool GetClear(void);						// クリア状況の取得処理

private:					// 自分だけアクセスできる

	// 静的メンバ変数(最初から必要な物を配置しておく)
	static CRenderer* m_pRenderer;					// レンダラーの情報
	static CInputKeyboard* m_pInputKeyboard;		// キーボードの情報
	static CInputGamePad* m_pInputGamePad;			// ゲームパッドの情報
	static CSound* m_pSound;						// サウンドの情報
	static CFile* m_pFile;							// ファイルの情報
	static CDebugProc* m_pDebugProc;				// デバッグ表示の情報
	static CCamera* m_pCamera;						// カメラの情報
	static CLight* m_pLight;						// ライトの情報
	static CTexture* m_pTexture;					// テクスチャの情報
	static CScene* m_pScene;						// シーンの情報
	static CFade* m_pFade;							// フェードの情報
	static DWORD m_nClearTime;						// クリアタイム
	static int m_nActionScore;						// アクションスコア
	static bool m_bClear;							// クリア状況

#ifdef _DEBUG		// デバッグ時のみ

	static bool m_bWire;						// ワイヤーフレーム状況

#endif
};

#endif