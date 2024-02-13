//============================================
//
// ゲームスコアヘッダー[game_score.h]
// Author：小原立暉
//
//============================================
#ifndef _GAME_SCORE_H_					// このマクロ定義がされていなかったら
#define _GAME_SCORE_H_					// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "score.h"

//--------------------------------------------
// クラス定義(スコア)
//--------------------------------------------
class CGameScore : public CScore
{
public:				// 誰でもアクセスできる

	CGameScore();			// コンストラクタ
	~CGameScore();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size);		// 情報の設定処理

	// 静的メンバ関数
	static CGameScore* Get(void);		// スコアの取得処理
	static CGameScore* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size);		// 生成処理

private:			// 自分だけアクセスできる

	// 静的メンバ変数
	static CGameScore* m_pGameScore;	// スコアの情報
};

#endif