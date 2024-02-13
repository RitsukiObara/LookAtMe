//==========================================================
//
// デバッグ表示処理 [debugproc.h]
// Author Ibuki Okusada
//
//==========================================================
#ifndef _DEBUGPROC_H_		//このマクロが定義されていない場合
#define _DEBUGPROC_H_

#include "main.h"

//**********************************************************
// マクロ定義
//**********************************************************
#define MAX_DEBUGSTRING	(2048)	//デバッグ表示の最大文字数

//**********************************************************
// クラスの定義
//**********************************************************
class CDebugProc
{
public:		// 誰でもアクセス出来る

	CDebugProc();	// コンストラクタ
	~CDebugProc();	// デストラクタ

	// メンバ関数
	void Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void Print(const char *fmt, ...);	// 設定処理

	// 静的メンバ関数
	static CDebugProc* Create(void);	// 生成処理

private:	// 自分のみアクセスできる

	// メンバ変数
	char m_aStr[MAX_DEBUGSTRING];	//デバッグ表示用の文字列
	bool m_bDisp;		//デバッグ表示のON/OFF

	// 静的メンバ変数
	static LPD3DXFONT m_pFont;	//フォントへのポインタ
};

#endif