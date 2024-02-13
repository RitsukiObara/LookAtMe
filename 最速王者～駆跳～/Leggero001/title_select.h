//============================================
//
// タイトルの選択肢ヘッダー[title_select.h]
// Author：小原立暉
//
//============================================
#ifndef _TITLE_SELECT_H_			// このマクロ定義がされていなかったら
#define _TITLE_SELECT_H_			// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "object2D.h"

//--------------------------------------------
// クラス定義(タイトル)
//--------------------------------------------
class CTitleSelect : public CObject2D
{
public:				// 誰でもアクセスできる

	CTitleSelect();			// コンストラクタ
	~CTitleSelect();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	// 静的メンバ関数
	static CTitleSelect* Create(void);		// 生成処理

private:			// 自分だけアクセスできる

	// メンバ変数
	float m_fAlpha;		// 透明度
	float m_fAlphaAdd;	// 透明度の加算数
	float m_fAlphaDest;	// 目的の透明度
};

#endif