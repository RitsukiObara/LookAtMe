//============================================
//
// 台マネージャーヘッダー[table_manager.h]
// Author：小原立暉
//
//============================================
#ifndef _TABLE_MANAGER_H_		// このマクロ定義がされていなかったら
#define _TABLE_MANAGER_H_		// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "main.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CTable;		// 台

//--------------------------------------------
// クラス(台マネージャークラス)
//--------------------------------------------
class CTableManager
{
public:				// 誰でもアクセスできる

	CTableManager();			// コンストラクタ
	~CTableManager();			// デストラクタ

	// メンバ関数
	void Regist(CTable* pThis);		// 登録処理
	void Uninit(void);				// 終了処理
	
	CTable* GetTop(void);			// 先頭の取得処理
	void Pull(CTable* pThis);		// リスト構造の引き抜き処理

	// 静的メンバ関数
	static CTableManager* Create(void);		// 生成処理
	static CTableManager* Get(void);		// 取得処理

private:			// 自分だけアクセスできる

	// メンバ変数
	CTable* m_pTop;		// 先頭のオブジェクト
	int m_nNumAll;		// オブジェクトの総数

	// 静的メンバ変数
	static CTableManager* m_pManager;		// マネージャーの変数
};

#endif