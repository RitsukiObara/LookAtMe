//============================================
//
// 順位UIヘッダー[ranking_rank.h]
// Author：小原立暉
//
//============================================
#ifndef _RANKING_RANK_H_					// このマクロ定義がされていなかったら
#define _RANKING_RANK_H_					// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "object.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CObject2D;		// 2Dポリゴン

//--------------------------------------------
// クラス定義(順位)
//--------------------------------------------
class CRankUI : public CObject
{
public:				// 誰でもアクセスできる

	CRankUI();				// コンストラクタ
	~CRankUI();				// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;		// 初期化処理
	void Uninit(void) override;			// 終了処理
	void Update(void) override;			// 更新処理
	void Draw(void) override;			// 描画処理

	void SetData(void);					// 情報の設定処理

	// 静的メンバ関数
	static CRankUI* Create(void);		// 生成処理

private:			// 自分だけアクセスできる

	// 定数
	static const int MAX_RANK = 5;		// 順位の最大数

	// メンバ変数
	CObject2D* m_apRankUI[MAX_RANK];	// 順位UIの情報
};

#endif