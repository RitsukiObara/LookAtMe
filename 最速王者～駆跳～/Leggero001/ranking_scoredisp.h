//===================================
//
// スコアの文字表示ヘッダー[ranking_scoredisp.h]
// Author 小原立暉
//
//===================================
#ifndef _RANKING_SCOREDISP_H_			//このマクロ定義がされていなかったら
#define _RANKING_SCOREDISP_H_			//2重インクルード防止のマクロを設定する

//-----------------------------------
// インクルードファイル
//-----------------------------------
#include "object2D.h"

//-----------------------------------
// クラス定義(スコアの文字)
//-----------------------------------
class CRankingScoreDisp : public CObject2D
{
public:			// 誰でもアクセスできる

	CRankingScoreDisp();		// コンストラクタ
	~CRankingScoreDisp();		// デストラクタ

	// メンバ関数
	HRESULT Init(void);			// スコアの文字の初期化処理
	void Uninit(void);			// スコアの文字の終了処理
	void Update(void);			// スコアの文字の更新
	void Draw(void);			// スコアの文字の描画処理

	// 静的メンバ関数
	static CRankingScoreDisp* Create(void);	// 生成処理

private:		// 自分だけアクセスできる

};

#endif