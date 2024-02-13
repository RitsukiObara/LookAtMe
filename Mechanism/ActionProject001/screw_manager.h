//============================================
//
// ネジマネージャーヘッダー[screw_manager.h]
// Author：小原立暉
//
//============================================
#ifndef _SCREW_MANAGER_H_		// このマクロ定義がされていなかったら
#define _SCREW_MANAGER_H_		// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "main.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CScrew;		// ネジ

//--------------------------------------------
// クラス(起伏マネージャークラス)
//--------------------------------------------
class CScrewManager
{
public:				// 誰でもアクセスできる

	CScrewManager();			// コンストラクタ
	~CScrewManager();			// デストラクタ

	// メンバ関数
	void Regist(CScrew* pThis);		// 登録処理
	void Uninit(void);						// 終了処理
	
	CScrew* GetTop(void);				// 先頭の取得処理
	void Pull(CScrew* pThis);			// リスト構造の引き抜き処理

	// 静的メンバ関数
	static CScrewManager* Create(void);		// 生成処理
	static CScrewManager* Get(void);		// 取得処理

private:			// 自分だけアクセスできる

	// メンバ変数
	CScrew* m_pTop;		// 先頭のオブジェクト
	int m_nNumAll;			// オブジェクトの総数

	// 静的メンバ変数
	static CScrewManager* m_pManager;		// マネージャーの変数
};

#endif