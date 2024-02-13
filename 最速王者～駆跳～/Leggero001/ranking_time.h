//============================================
//
// ランキングタイマーヘッダー[ranking_time.h]
// Author：小原立暉
//
//============================================
#ifndef _RANKING_TIME_H_					// このマクロ定義がされていなかったら
#define _RANKING_TIME_H_					// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "time.h"

//--------------------------------------------
// クラス定義(タイム)
//--------------------------------------------
class CRankingTime : public CTime
{
public:				// 誰でもアクセスできる

	CRankingTime();				// コンストラクタ
	~CRankingTime();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void Calculate(void);	// 計算処理

	// 静的メンバ変数
	static CRankingTime* Create(void);		// 生成処理

private:			// 自分だけアクセスできる

	// メンバ変数
	D3DXVECTOR3 m_pos;		// 位置
};

#endif