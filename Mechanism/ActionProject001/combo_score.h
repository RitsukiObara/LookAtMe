//============================================
//
// コンボスコアヘッダー[score.h]
// Author：小原立暉
//
//============================================
#ifndef _COMBO_SCORE_H_					// このマクロ定義がされていなかったら
#define _COMBO_SCORE_H_					// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "number.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define MAX_COMBO_SCORE_DIGIT		(6)		// コンボスコアの桁数

//--------------------------------------------
// クラス定義(コンボスコア)
//--------------------------------------------
class CComboScore : public CObject
{
public:				// 誰でもアクセスできる

	CComboScore();			// コンストラクタ
	~CComboScore();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size, const float fShift);		// 情報の設定処理

	// セット・ゲット関係
	void SetScore(const int nScore);	// スコアの設定処理
	int GetScore(void) const;			// スコアの取得処理

	// 静的メンバ変数
	void AddScore(const int nScore);	// スコアの加算処理
	static CComboScore* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size, const float fShift);		// 生成処理

private:			// 自分だけアクセスできる

	// メンバ関数
	void Calculate(void);			// 計算処理

	// メンバ変数
	CNumber *m_apNumber[MAX_COMBO_SCORE_DIGIT];		// 番号の情報
	int m_nScore;			// スコア
	int m_nDispScore;		// 表示用スコア
};

#endif