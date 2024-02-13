//============================================
//
// アクションUIヘッダー[actionUI.h]
// Author：小原立暉
//
//============================================
#ifndef _ACTIONUI_H_					// このマクロ定義がされていなかったら
#define _ACTIONUI_H_					// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "object.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CObject2D;

//--------------------------------------------
// クラス定義(アクションUI)
//--------------------------------------------
class CActionUI : public CObject
{
public:				// 誰でもアクセスできる

	// 列挙型定義(ポリゴン)
	typedef enum
	{
		POLY_GROUND = 0,		// 背景
		POLY_ACT,				// アクション名
		POLY_MAX				// この列挙型の総数
	}POLY;

	// 列挙型定義(種類)
	typedef enum
	{
		TYPE_WALLKICK = 0,		// 壁キック
		TYPE_LANDTURBO,			// 着地ターボ
		TYPE_SUPERJUMP,			// 超ジャンプ
		TYPE_LAMPJUMP,			// 街灯ジャンプ
		TYPE_ATTACK,			// 攻撃
		TYPE_MAX				// この列挙型の総数
	}TYPE;

	// 列挙型定義(状態)
	typedef enum
	{
		STATE_APPEAR = 0,		// 出現状態
		STATE_DISP,				// 表示状態
		STATE_SMALLER,			// 縮小状態
		STATE_MAX				// この列挙型の総数
	}STATE;

	CActionUI();				// コンストラクタ
	~CActionUI();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);			// 初期化処理
	void Uninit(void);			// 終了処理
	void Update(void);			// 更新処理
	void Draw(void);			// 描画処理

	void SetData(const TYPE type, const int nScore);	// 情報の設定処理

	// 静的メンバ関数
	static CActionUI* Create(const TYPE type, const int nScore);		// 生成処理

private:			// 自分だけアクセスできる

	// メンバ関数
	void SizeChange(const int nCnt);	// サイズ変化処理
	void AppearChange(const int nCnt);	// 出現状態変化処理
	void DispChange(void);				// 表示状態変化処理
	void Rotation(const int nCnt);		// 向き変化処理

	void GroundBound(void);				// 背景のバウンド処理
	void GroundChange(void);			// 背景の状態変化処理
	void GroundDispProcess(void);		// 背景の表示状態時処理

	// メンバ変数
	CObject2D* m_apActUI[POLY_MAX];		// アクションUIの情報
	D3DXVECTOR3 m_sizeDest;				// 目標のサイズ
	D3DXVECTOR3 m_GroundMove;			// 背景のサイズの移動量
	STATE m_state;						// 状態
	int m_nStateCount;					// 状態カウント
	int m_nScore;						// 得点

	// 静的メンバ変数
	static const char* m_apTexturename[TYPE_MAX];	// テクスチャのパス名
	static const D3DXVECTOR3 m_aSize[TYPE_MAX];		// それぞれの大きさ
};

#endif