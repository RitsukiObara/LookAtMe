//============================================
//
// カウントダウンヘッダー[countdown.h]
// Author：小原立暉
//
//============================================
#ifndef _COUNTDOWN_H_					// このマクロ定義がされていなかったら
#define _COUNTDOWN_H_					// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "object2D.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CLocus2D;			// 残像

//--------------------------------------------
// クラス定義(カウントダウン)
//--------------------------------------------
class CCountdown : public CObject2D
{
public:				// 誰でもアクセスできる

	// 列挙型定義(テクスチャ)
	typedef enum
	{
		TEX_NUMBER = 0,			// 数字
		TEX_START,				// スタート
		TEX_MAX					// この列挙型の総数
	}TEX;

	// 列挙型定義(状態)
	typedef enum
	{
		STATE_COUNT = 0,		// カウント状態
		STATE_START,			// スタート状態
		STATE_MAX				// この列挙型の総数
	}STATE;

	CCountdown();				// コンストラクタ
	~CCountdown();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(void);		// 情報の設定処理
	void Count(void);		// カウントダウン処理
	bool SearchLocus(void);	// 残像捜索処理
	void LocusSize(void);	// 残像のサイズ設定処理

	// 静的メンバ関数
	static CCountdown* Create(void);	// 生成処理

private:			// 自分だけアクセスできる

	// メンバ変数
	CLocus2D* m_pLocus;			// 残像のポインタ
	STATE m_state;				// 状態
	int m_nCountDown;			// スタートまでのタイム
	int m_nIntervalCount;		// 間隔のカウント
	float m_fAlpha;				// 透明度

	// 静的メンバ変数
	static const char* m_apTexturename[TEX_MAX];	// テクスチャのパス名
};

#endif