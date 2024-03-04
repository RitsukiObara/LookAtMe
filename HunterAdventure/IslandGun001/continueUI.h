//============================================
//
// コンティニューUI表示ヘッダー[continueUI.h]
// Author：小原立暉
//
//============================================
#ifndef _CONTINUE_UI_H_					// このマクロ定義がされていなかったら
#define _CONTINUE_UI_H_					// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "object.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CObject2D;		// 2Dポリゴン
class CNumber;			// 番号

//--------------------------------------------
// クラス定義(コンティニューUI)
//--------------------------------------------
class CContinueUI : public CObject
{
public:				// 誰でもアクセスできる

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_SCREEN = 0,	// 背景
		TYPE_GROUND,		// 下地
		TYPE_QUERY,			// 質問
		TYPE_COST,			// コスト
		TYPE_YES,			// YESの選択肢
		TYPE_NO,			// NOの選択肢
		TYPE_MAX			// この列挙型の総数
	};

	// 列挙型定義(選択肢)
	enum SELECT
	{
		SELECT_YES = 0,		// YESの選択肢
		SELECT_NO,			// NOの選択肢
		SELECT_MAX			// この列挙型の総数
	};

	CContinueUI();				// コンストラクタ
	~CContinueUI();				// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;		// 初期化処理
	void Uninit(void) override;			// 終了処理
	void Update(void) override;			// 更新処理
	void Draw(void) override;			// 描画処理

	void SetData(void);					// 情報の設定処理

	// 静的メンバ関数
	static CContinueUI* Create(void);	// 生成処理

	// 静的メンバ関数
	static void SubScoreReset(void);	// 得点の減算量のリセット処理
	static int GetSubScore(void);		// 得点の減算量の取得処理

	// 定数定義
	static const int NUM_COSTDIGIT = 4;		// 消費スコアの桁数

private:			// 自分だけアクセスできる

	// メンバ関数
	void Select(void);		// 選択処理
	bool Decide(void);		// 決定処理
	void Continue(void);	// コンティニュー処理

	// メンバ変数
	CObject2D* m_apObject[TYPE_MAX];		// ポリゴンの情報
	CNumber* m_apNumber[NUM_COSTDIGIT];		// 番号
	SELECT m_select;		// 選択肢

	// 静的メンバ変数
	static int m_nSubScore;		// 得点の減算量 
};

#endif