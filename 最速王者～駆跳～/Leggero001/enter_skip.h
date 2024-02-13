//============================================
//
// ENTERスキップヘッダー[enter_skip.h]
// Author：小原立暉
//
//============================================
#ifndef _ENTER_SKIP_H_			// このマクロ定義がされていなかったら
#define _ENTER_SKIP_H_			// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "object2D.h"

//--------------------------------------------
// クラス定義(スキップ表示)
//--------------------------------------------
class CEnterSkip : public CObject2D
{
public:				// 誰でもアクセスできる

	CEnterSkip();			// コンストラクタ
	~CEnterSkip();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(void);		// 情報の設定処理

	// 静的メンバ関数
	static CEnterSkip* Create(void);		// 生成処理

private:			// 自分だけアクセスできる

};

#endif