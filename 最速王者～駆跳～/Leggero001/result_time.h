//============================================
//
// リザルト表示用タイマーヘッダー[result_time.h]
// Author：小原立暉
//
//============================================
#ifndef _RESULT_TIME_H_					// このマクロ定義がされていなかったら
#define _RESULT_TIME_H_					// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "result.h"
#include "time.h"

//--------------------------------------------
// クラス定義(タイム)
//--------------------------------------------
class CResultTime : public CTime
{
public:				// 誰でもアクセスできる

	CResultTime();				// コンストラクタ
	~CResultTime();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	// 静的メンバ変数
	static CResultTime* Create(void);		// 生成処理

private:			// 自分だけアクセスできる

	// メンバ関数
	void Calculate(void);		// 計算処理
	void MoveProcess(void);		// 移動処理
	void DotMove(void);			// 点の移動処理
	CResult::RANK DecisionRank(void);		// ランクの決定処理

	// メンバ変数
	D3DXVECTOR3 m_pos;							// 位置
};

#endif