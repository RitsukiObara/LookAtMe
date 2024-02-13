//============================================
//
// コンボヘッダー[combo.h]
// Author：小原立暉
//
//============================================
#ifndef _COMBO_H_					// このマクロ定義がされていなかったら
#define _COMBO_H_					// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "object.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CComboCircle;		// コンボの円
class CComboScore;		// コンボスコア
class CComboMagni;		// コンボ倍率

//--------------------------------------------
// マクロ定義
//--------------------------------------------

//--------------------------------------------
// クラス定義(コンボ)
//--------------------------------------------
class CCombo : public CObject
{
public:				// 誰でもアクセスできる

	CCombo();					// コンストラクタ
	~CCombo();					// デストラクタ

	// メンバ関数
	HRESULT Init(void);			// 初期化処理
	void Uninit(void);			// 終了処理
	void Update(void);			// 更新処理
	void Draw(void);			// 描画処理

	void AddCombo(const int nScore);	// コンボの加算処理

	// 静的メンバ変数
	static CCombo* Create(void);		// 生成処理

private:			// 自分だけアクセスできる

	// メンバ関数
	HRESULT CreateCombo(void);		// コンボの生成処理
	HRESULT CreateLimit(void);		// コンボ制限の生成処理
	HRESULT CreateComboScore(void);	// コンボスコアの生成処理

	// メンバ変数
	CComboMagni* m_pMagni;			// 倍率
	CComboCircle* m_pTimeLimit;		// コンボ制限
	CComboScore* m_pScore;			// コンボスコア
};

#endif