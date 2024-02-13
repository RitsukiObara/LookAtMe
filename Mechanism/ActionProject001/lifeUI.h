//============================================
//
// 体力UIヘッダー[lifeUI.h]
// Author：小原立暉
//
//============================================
#ifndef _LIFEUI_H_					// このマクロ定義がされていなかったら
#define _LIFEUI_H_					// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "object.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define MAX_LIFE_POLYGON		(3)		// 体力の最大数

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CObject2D;		// 2Dポリゴン

//--------------------------------------------
// クラス定義(体力UI)
//--------------------------------------------
class CLifeUI : public CObject
{
public:				// 誰でもアクセスできる

	CLifeUI();					// コンストラクタ
	~CLifeUI();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);			// 初期化処理
	void Uninit(void);			// 終了処理
	void Update(void);			// 更新処理
	void Draw(void);			// 描画処理

	void SetData(void);			// 情報の設定処理

	// 静的メンバ変数
	static CLifeUI* Create(void);		// 生成処理

private:			// 自分だけアクセスできる

	// メンバ関数
	void SetDataBack(void);				// 背景の生成処理
	void SetDataLife(void);				// 体力の生成処理
	HRESULT CreateBack(void);			// 背景の生成処理
	HRESULT CreateLife(void);			// 体力の生成処理

	// メンバ変数
	CObject2D* m_pBack;						// 背景
	CObject2D* m_apLife[MAX_LIFE_POLYGON];	// 体力
};

#endif