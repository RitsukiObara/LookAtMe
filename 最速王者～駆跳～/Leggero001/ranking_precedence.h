//===================================
//
// ランキングの順位表示ヘッダー[ranking_precedence.h]
// Author 小原立暉
//
//===================================
#ifndef _RANKING_PRECEDENCE_H_			//このマクロ定義がされていなかったら
#define _RANKING_PRECEDENCE_H_			//2重インクルード防止のマクロを設定する

//-----------------------------------
// インクルードファイル
//-----------------------------------
#include "ranking.h"
#include "object.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CObject2D;	// オブジェクト2D

// クラス定義(ランキング)
class CRankingPrece : public CObject
{
public:			// 誰でもアクセスできる

	CRankingPrece();					// コンストラクタ
	~CRankingPrece();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);			// ランキング画面の初期化処理
	void Uninit(void);			// ランキング画面の終了処理
	void Update(void);			// ランキング画面の更新
	void Draw(void);			// ランキング画面の描画処理

	// 静的メンバ関数
	static CRankingPrece* Create(void);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	CObject2D* m_apRank[MAX_RANKING];	// ランク(順位)
};

#endif