//============================================
//
// 棘マネージャーヘッダー[needle_manager.h]
// Author：小原立暉
//
//============================================
#ifndef _NEEDLE_MANAGER_H_		// このマクロ定義がされていなかったら
#define _NEEDLE_MANAGER_H_		// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "main.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CNeedle;		// 棘

//--------------------------------------------
// クラス(棘マネージャークラス)
//--------------------------------------------
class CNeedleManager
{
public:				// 誰でもアクセスできる

	CNeedleManager();			// コンストラクタ
	~CNeedleManager();		// デストラクタ

	// メンバ関数
	void Regist(CNeedle* pThis);		// 登録処理
	void Uninit(void);						// 終了処理
	
	CNeedle* GetTop(void);				// 先頭の取得処理
	void Pull(CNeedle* pThis);			// リスト構造の引き抜き処理

	// 静的メンバ関数
	static CNeedleManager* Create(void);		// 生成処理
	static CNeedleManager* Get(void);		// 取得処理

private:			// 自分だけアクセスできる

	// メンバ変数
	CNeedle* m_pTop;		// 先頭のオブジェクト
	int m_nNumAll;			// オブジェクトの総数

	// 静的メンバ変数
	static CNeedleManager* m_pManager;		// マネージャーの変数
};

#endif