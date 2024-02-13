//============================================
//
// 褒め言葉ヘッダー[praiseUI.h]
// Author：小原立暉
//
//============================================
#ifndef _PRAISEUI_H_					// このマクロ定義がされていなかったら
#define _PRAISEUI_H_					// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "object2D.h"

//--------------------------------------------
// クラス定義(数値)
//--------------------------------------------
class CPraiseUI : public CObject2D
{
public:				// 誰でもアクセスできる

	// 列挙型定義(種類)
	typedef enum
	{
		TYPE_AMAZING = 0,		// アメイジング
		TYPE_GREAT,				// グレート
		TYPE_EXCITING,			// エキサイティング
		TYPE_MAX				// この列挙型の総数
	}TYPE;

	// 列挙型定義(状態)
	typedef enum
	{
		STATE_APPEAR = 0,		// 出現状態
		STATE_MOVE,				// 移動状態
		STATE_MAX				// この列挙型の総数
	}STATE;

	CPraiseUI();				// コンストラクタ
	~CPraiseUI();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const int nScore);		// 情報の設定処理

	// 静的メンバ関数
	static CPraiseUI* Create(const int nScore);		// 生成処理

private:			// 自分だけアクセスできる

	// メンバ関数
	void Appear(void);		// 出現処理
	void Move(void);		// 移動処理
	void Cycle(void);		// 回転処理
	void ScaleDown(void);	// 縮小処理

	// メンバ変数
	D3DXVECTOR3 m_posDest;	// 目的の位置
	D3DXVECTOR3 m_move;		// 移動量
	D3DXVECTOR3 m_sizeDest;	// 目的のサイズ
	D3DXVECTOR3 m_scaling;	// サイズの拡大量
	STATE m_state;			// 状態
	int m_nStateCount;		// 状態カウント
	int m_nScore;			// 得点

	// 静的メンバ変数
	static const char* m_apTexturename[TYPE_MAX];	// テクスチャのパス名
};

#endif