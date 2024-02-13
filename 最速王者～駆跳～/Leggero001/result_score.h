//============================================
//
// スコアヘッダー[result_score.h]
// Author：小原立暉
//
//============================================
#ifndef _RESULT_SCORE_H_					// このマクロ定義がされていなかったら
#define _RESULT_SCORE_H_					// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "number.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define MAX_RESULTSCORE_DIGIT		(6)			// スコアの桁数

//--------------------------------------------
// クラス定義(タイム)
//--------------------------------------------
class CResultScore : public CObject
{
public:				// 誰でもアクセスできる

	CResultScore();			// コンストラクタ
	~CResultScore();		// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	// 静的メンバ変数
	static CResultScore* Create(void);		// 生成処理

private:			// 自分だけアクセスできる

	// メンバ関数
	void Calculate(void);	// 計算処理
	void MoveProcess(void);	// 移動処理
	CResult::RANK DecisionRank(void);		// ランクの決定処理

	// メンバ変数
	D3DXVECTOR3 m_pos;								// 位置
	CNumber *m_apNumber[MAX_RESULTSCORE_DIGIT];		// 番号の情報
	int m_nScore;			// スコア
};

#endif