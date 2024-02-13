//============================================
//
// 敵マネージャーヘッダー[enemy_manager.h]
// Author：小原立暉
//
//============================================
#ifndef _ENEMY_MANAGER_H_		// このマクロ定義がされていなかったら
#define _ENEMY_MANAGER_H_		// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "main.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CEnemy;		// 敵

//--------------------------------------------
// クラス(敵マネージャークラス)
//--------------------------------------------
class CEnemyManager
{
public:				// 誰でもアクセスできる

	CEnemyManager();			// コンストラクタ
	~CEnemyManager();			// デストラクタ

	// メンバ関数
	void Regist(CEnemy* pThis);		// 登録処理
	void Uninit(void);						// 終了処理
	
	CEnemy* GetTop(void);				// 先頭の取得処理
	void Pull(CEnemy* pThis);			// リスト構造の引き抜き処理

	// 静的メンバ関数
	static CEnemyManager* Create(void);		// 生成処理
	static CEnemyManager* Get(void);		// 取得処理

private:			// 自分だけアクセスできる

	// メンバ変数
	CEnemy* m_pTop;		// 先頭のオブジェクト
	int m_nNumAll;			// オブジェクトの総数

	// 静的メンバ変数
	static CEnemyManager* m_pManager;		// マネージャーの変数
};

#endif