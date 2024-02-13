//============================================
//
// ネジUIヘッダー[screwUI.h]
// Author：小原立暉
//
//============================================
#ifndef _SCREWUI_H_					// このマクロ定義がされていなかったら
#define _SCREWUI_H_					// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "object.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CObject2D;		// 2Dポリゴン
class CNumber;			// 数字

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define MAX_SCREW_DIGIT		(2)			// ネジの個数

//--------------------------------------------
// クラス定義(スコア)
//--------------------------------------------
class CScrewUI : public CObject
{
public:				// 誰でもアクセスできる

	CScrewUI();					// コンストラクタ
	~CScrewUI();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);			// 初期化処理
	void Uninit(void);			// 終了処理
	void Update(void);			// 更新処理
	void Draw(void);			// 描画処理

	void SetData(void);			// 情報の設定処理
	void SetDataScore(void);	// スコアの情報の設定処理
	void SetDataBack(void);		// 背景の情報の設定処理

	// セット・ゲット関係
	void SetScore(const int nScore);	// スコアの設定処理
	int GetScore(void) const;			// スコアの取得処理

	// 静的メンバ変数
	void AddScore(const int nScore);	// スコアの加算処理
	static CScrewUI* Create(void);		// 生成処理

private:			// 自分だけアクセスできる

	// メンバ関数
	HRESULT CreateScore(void);	// スコアの生成処理
	HRESULT CreateBack(void);	// 背景の生成処理
	void Calculate(void);		// 計算処理

	// メンバ変数
	CObject2D* m_pBack;		// 背景
	CNumber *m_apNumber[MAX_SCREW_DIGIT];		// 番号の情報
	int m_nScore;			// スコア
};

#endif