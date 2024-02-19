//============================================
//
// リザルトスコアヘッダー[result_score.h]
// Author：小原立暉
//
//============================================
#ifndef _RESULT_SCORE_H_					// このマクロ定義がされていなかったら
#define _RESULT_SCORE_H_					// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "score.h"

//--------------------------------------------
// クラス定義(リザルトスコア)
//--------------------------------------------
class CResultScore : public CScore
{
public:				// 誰でもアクセスできる

	CResultScore();			// コンストラクタ
	~CResultScore();		// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(void);		// 情報の設定処理

	// 静的メンバ関数
	static CResultScore* Create(void);		// 生成処理

private:			// 自分だけアクセスできる

};

#endif