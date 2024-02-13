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
// マクロ定義
//--------------------------------------------
#define COMBO_DIGIT			(3)		// コンボの桁数

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CObject2D;			// オブジェクト2D
class CNumber;				// 番号

//--------------------------------------------
// クラス定義(数値)
//--------------------------------------------
class CCombo : public CObject
{
public:				// 誰でもアクセスできる

	// 列挙型定義(種類)
	typedef enum
	{
		TYPE_WORD = 0,		// 言葉
		TYPE_MULTI,			// ×マーク
		TYPE_MAX			// この列挙型の総数
	}TYPE;

	// 構造体定義(コンボの値)
	typedef struct
	{
		CNumber* pNumber;	// 数字の情報
		bool bDisp;			// 描画状況
	}SCombo;

	CCombo();				// コンストラクタ
	~CCombo();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	// 静的メンバ関数
	static CCombo* Create(void);		// 生成処理

private:			// 自分だけアクセスできる

	// メンバ関数
	void Calculation(void);				// 計算処理
	void DispDecide(void);				// 描画判定処理

	// メンバ変数
	CObject2D* m_apObject2D[TYPE_MAX];	// 2Dポリゴンの情報
	SCombo m_aNumber[COMBO_DIGIT];		// 数値の情報
	int m_nCombo;						// コンボ数
};

#endif