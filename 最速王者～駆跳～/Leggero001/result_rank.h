//============================================
//
// リザルトランクヘッダー[result_rank.h]
// Author：小原立暉
//
//============================================
#ifndef _RESULT_RANK_H_					// このマクロ定義がされていなかったら
#define _RESULT_RANK_H_					// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "model.h"
#include "result.h"

//--------------------------------------------
// クラス定義(タイム)
//--------------------------------------------
class CResultRank : public CModel
{
public:				// 誰でもアクセスできる

	CResultRank();		// コンストラクタ
	~CResultRank();		// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3 pos, const CResult::RANK rank);			// 情報の設定処理

	// 静的メンバ変数
	static CResultRank* Create(const D3DXVECTOR3 pos, const CResult::RANK rank);		// 生成処理

private:			// 自分だけアクセスできる

};

#endif