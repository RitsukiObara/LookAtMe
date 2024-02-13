//============================================
//
// 着地UIヘッダー[landUI.h]
// Author：小原立暉
//
//============================================
#ifndef _LANDUI_H_					// このマクロ定義がされていなかったら
#define _LANDUI_H_					// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "object2D.h"

//--------------------------------------------
// クラス定義(数値)
//--------------------------------------------
class CLandUI : public CObject2D
{
public:				// 誰でもアクセスできる

	// 列挙型定義(種類)
	typedef enum
	{
		TYPE_BAD = 0,		// BAD
		TYPE_GOOD,			// GOOD
		TYPE_MAX			// この列挙型の総数
	}TYPE;

	// 列挙型定義(状態)
	typedef enum
	{
		STATE_APPEAR = 0,	// 出現
		STATE_DISP,			// 表示
		STATE_DELETE,		// 消失
		STATE_MAX			// この列挙型の総数
	}STATE;

	CLandUI();				// コンストラクタ
	~CLandUI();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(TYPE type);		// 情報の設定処理

	// 静的メンバ関数
	static CLandUI* Create(TYPE type);		// 生成処理

private:			// 自分だけアクセスできる

	// メンバ関数
	void Move(void);			// 移動処理
	void DispDecision(void);	// 表示状態判定処理

	// メンバ変数
	TYPE m_type;				// 種類
	STATE m_state;				// 状態
	int m_nStateCount;			// 状態カウント

	// 静的メンバ変数
	static const char* m_apTexturename[TYPE_MAX];	// テクスチャのパス名
};

#endif