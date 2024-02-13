//===================================
//
// ランキングヘッダー[ranking_score.h]
// Author 小原立暉
//
//===================================
#ifndef _RANKING_SCORE_H_			// このマクロ定義がされていなかったら
#define _RANKING_SCORE_H_			// 2重インクルード防止のマクロを設定する

//-----------------------------------
// インクルードファイル
//-----------------------------------
#include "object.h"
#include "ranking.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CTime;		// タイム

//-----------------------------------
// マクロ定義
//-----------------------------------

// クラス定義(ランキング)
class CRankingScore : public CObject
{
public:			// 誰でもアクセスできる

	CRankingScore();			// コンストラクタ
	~CRankingScore();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);			// 初期化処理
	void Uninit(void);			// 終了処理
	void Update(void);			// 更新
	void Draw(void);			// 描画処理

	void SetData(void);			// 情報の設定処理
	void RankSwap(int nScore);	// ランキングの入れ替え処理

	// 静的メンバ変数
	static CRankingScore* Create(void);	// ランキング画面の生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	D3DXCOLOR m_col;				// 新記録の色
	D3DXVECTOR3 m_pos;				// 位置
	CTime* m_apTime[MAX_RANKING];	// ランキングのタイム
	int m_nNewRecordRank;			// 新記録の順位
	int m_nSoundCount;				// 音を鳴らすカウント
};

#endif