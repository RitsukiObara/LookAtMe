//============================================
//
// 飛行機マネージャーヘッダー[airplane_manager.h]
// Author：小原立暉
//
//============================================
#ifndef _AIRPLANE_MANAGER_H_		// このマクロ定義がされていなかったら
#define _AIRPLANE_MANAGER_H_		// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "main.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CAirplane;		// 飛行機

//--------------------------------------------
// クラス(飛行機マネージャークラス)
//--------------------------------------------
class CAirplaneManager
{
public:				// 誰でもアクセスできる

	CAirplaneManager();			// コンストラクタ
	~CAirplaneManager();			// デストラクタ

	// メンバ関数
	void Regist(CAirplane* pThis);		// 登録処理
	void Uninit(void);						// 終了処理
	
	CAirplane* GetTop(void);				// 先頭の取得処理
	void Pull(CAirplane* pThis);			// リスト構造の引き抜き処理

	// 静的メンバ関数
	static CAirplaneManager* Create(void);		// 生成処理
	static CAirplaneManager* Get(void);		// 取得処理

private:			// 自分だけアクセスできる

	// メンバ変数
	CAirplane* m_pTop;		// 先頭のオブジェクト
	int m_nNumAll;			// オブジェクトの総数

	// 静的メンバ変数
	static CAirplaneManager* m_pManager;		// マネージャーの変数
};

#endif