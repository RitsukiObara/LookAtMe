//============================================
//
// マキナ草マネージャーヘッダー[macchina_manager.h]
// Author：小原立暉
//
//============================================
#ifndef _MACCHINA_MANAGER_H_		// このマクロ定義がされていなかったら
#define _MACCHINA_MANAGER_H_		// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "main.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CMacchina;		// マキナ草

//--------------------------------------------
// クラス(マキナ草マネージャークラス)
//--------------------------------------------
class CMacchinaManager
{
public:				// 誰でもアクセスできる

	CMacchinaManager();			// コンストラクタ
	~CMacchinaManager();		// デストラクタ

	// メンバ関数
	void Regist(CMacchina* pThis);		// 登録処理
	void Uninit(void);						// 終了処理
	
	CMacchina* GetTop(void);				// 先頭の取得処理
	void Pull(CMacchina* pThis);			// リスト構造の引き抜き処理

	// 静的メンバ関数
	static CMacchinaManager* Create(void);		// 生成処理
	static CMacchinaManager* Get(void);		// 取得処理

private:			// 自分だけアクセスできる

	// メンバ変数
	CMacchina* m_pTop;		// 先頭のオブジェクト
	int m_nNumAll;			// オブジェクトの総数

	// 静的メンバ変数
	static CMacchinaManager* m_pManager;		// マネージャーの変数
};

#endif