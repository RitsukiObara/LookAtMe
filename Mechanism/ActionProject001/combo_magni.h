//============================================
//
// コンボ倍率ヘッダー[combo_magni.h]
// Author：小原立暉
//
//============================================
#ifndef _COMBO_MAGNI_H_					// このマクロ定義がされていなかったら
#define _COMBO_MAGNI_H_					// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "number.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define MAX_COMBO_DIGIT		(3)			// コンボの桁数

//--------------------------------------------
// クラス定義(コンボ倍率)
//--------------------------------------------
class CComboMagni : public CObject
{
public:				// 誰でもアクセスできる

	CComboMagni();			// コンストラクタ
	~CComboMagni();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size, const float fShift);		// 情報の設定処理

	// セット・ゲット関係
	void SetMagni(const int nMagni);	// 倍率の設定処理
	int GetMagni(void) const;			// 倍率の取得処理

	// 静的メンバ変数
	void AddMagni(void);	// スコアの加算処理
	static CComboMagni* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size, const float fShift);		// 生成処理

private:			// 自分だけアクセスできる

	// メンバ関数
	void Calculate(void);			// 計算処理

	// メンバ変数
	CNumber* m_apNumber[MAX_COMBO_DIGIT];		// 番号の情報
	int m_nMagni;					// コンボ倍率
};

#endif