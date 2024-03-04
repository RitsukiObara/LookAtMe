//============================================
//
// ランクスコアヘッダー[rank_score.h]
// Author：小原立暉
//
//============================================
#ifndef _RANK_SCORE_H_					// このマクロ定義がされていなかったら
#define _RANK_SCORE_H_					// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "score.h"

//--------------------------------------------
// クラス定義(ランクスコア)
//--------------------------------------------
class CRankScore : public CScore
{
public:				// 誰でもアクセスできる

	CRankScore();			// コンストラクタ
	~CRankScore();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const int nScore);		// 情報の設定処理

	// 静的メンバ関数
	static CRankScore* Create(const D3DXVECTOR3& pos, const int nScore);		// 生成処理

private:			// 自分だけアクセスできる

};

#endif